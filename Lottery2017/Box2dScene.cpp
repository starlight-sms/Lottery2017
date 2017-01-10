#include "pch.h"
#include "Box2dScene.h"
#include <DirectXMath.h>
#include "Person.h"

#pragma comment(lib, "d2d1")

using namespace std;
using D2D1::ColorF;
using D2D1::Matrix3x2F;
using namespace DirectX;

CD2DPointF ToD2DPoint(const b2Vec2& v);

Box2dScene::Box2dScene(int count, int itemId, const std::vector<int>& personIds) :
	LotteryScene(count, itemId, personIds),
	_world{ b2Vec2{0, 10} },
	_state{ State::Pending }
{
	{
		auto H = RelativeSize / 2;
		auto A = RelativeSize;
		auto P2 = XM_PIDIV2;
		_borders.push_back(CreateBorderBody(H, 0, 0, A));
		_borders.push_back(CreateBorderBody(0, H, P2, A));
		_borders.push_back(CreateBorderBody(A, H, P2, A));
		_borders.push_back(CreateBorderBody(H, A, 0, A));
	}

	for (auto id : personIds)
	{
		_personBodies.push_back(CreatePersonBody(id));
	}
}

void Box2dScene::Update()
{
	if (_state == State::Started)
	{
		_world.Step(1 / 30.0f, 6, 2);
		if (++_updateCount == 100)
		{
			_world.SetGravity({ 0, 0 });
		}
	}
	else if (_state == State::Triggled)
	{
		_world.Step(1 / 300.0f, 6, 2);
		FindLuckyPersons();
	}
	else if (_state == State::Completed)
	{
		++_updateCount;
	}
}

void Box2dScene::Render(CHwndRenderTarget * target, DxRes* dxRes)
{
	if (_state == State::Pending) return;

	auto size = target->GetSize();
	auto scale = 0.1f * min(size.width, size.height);

	for (auto border : _borders)
	{
		PreRenderBody(target, border, scale);
		auto length = (INT_PTR)border->GetUserData() / 100.0f;
		auto shape = (b2PolygonShape*)border->GetFixtureList()->GetShape();
		target->FillGeometry(
			dxRes->GetOrCreateBorderGeometry(target, length, BorderWidth),
			dxRes->GetColorBrush(target, _borderColor));
	}

	auto H = PersonSize / 2;
	for (auto personBody : _personBodies)
	{
		PreRenderBody(target, personBody, scale);
		auto shape = (b2PolygonShape*)personBody->GetFixtureList()->GetShape();
		auto userId = (int)personBody->GetUserData();
		auto brush = dxRes->PersonBrushes[userId];
		target->FillRectangle({ -H, -H, H, H }, brush);

		auto color = dxRes->GetColorBrush(target, _luckyColor);
		if (_luckyPersonIds.find(userId) != _luckyPersonIds.end())
		{
			target->DrawRectangle({ -H, -H, H, H }, color, 3 / scale);

			target->SetTransform(Matrix3x2F::Identity());

			auto person = GetAllPerson()[userId];
			auto X = personBody->GetPosition().x * scale;
			auto Y = personBody->GetPosition().y * scale;

			CD2DRectF nameRect{
				X - scale * 1.5f,
				Y - 20,
				X, Y + 300 };
			target->DrawTextW(person.Name, nameRect, color, dxRes->HeaderTextFormat);

			CD2DRectF notesRect{
				X - scale * 4,
				Y,
				X,Y + 300 };
			target->DrawTextW(person.Notes, notesRect, color, dxRes->TextFormat);
		}
	}

	target->SetTransform(Matrix3x2F::Identity());
	if (_state == State::Completed)
	{
		ShowWinner(target, dxRes);
	}
}

void Box2dScene::KeyUp(UINT key)
{
	if (key == VK_SPACE)
	{
		if (_state == State::Pending)
		{
			_state = State::Started;
		}
		else if (_state == State::Started)
		{
			EnterTriggerMode();
		}
	}
}

void Box2dScene::ShowWinner(CHwndRenderTarget * target, DxRes * dxRes)
{
	auto windowSize = target->GetSize();
	auto whRate = windowSize.width / windowSize.height;
	auto maxCol = (int)ceil(sqrt(_requiredCount * whRate));
	auto maxRow = (int)ceil(1.0 * _requiredCount / maxCol);

	auto gridSize = Divide(windowSize, (float)maxCol, (float)maxRow);
	auto lastCol = _requiredCount - (maxRow - 1) * maxCol;
	auto lastGridSize = Divide(windowSize, (float)lastCol, (float)maxRow);

	int col = 0, row = 0;
	for (auto id : _luckyPersonIds)
	{
		auto grid = (int)row < maxRow - 1 ? gridSize : lastGridSize;
		auto topLeft = Multiple(grid, (float)col, (float)row);
		auto bmp = dxRes->PersonBitmaps[id];

		auto realSize = GetDisplaySize(bmp->GetSize(), grid);
		auto rect = GetDrawCenterRect(topLeft, grid, realSize);
		target->DrawBitmap(bmp, rect, b2Clamp(_updateCount / 200.0f, 0.f, 1.0f));

		CString str = GetAllPerson()[id].Name;
		target->DrawTextW(str, rect, dxRes->GetColorBrush(target, ColorF::Red), dxRes->HeaderTextFormat);
		CString notes(L"\r\n");
		notes.Append(GetAllPerson()[id].Notes);
		target->DrawTextW(notes, rect, dxRes->GetColorBrush(target, ColorF::OrangeRed), dxRes->TextFormat);

		row += ++col / maxCol;
		col = col % maxCol;
	}
}

void Box2dScene::PreRenderBody(CHwndRenderTarget* target, b2Body * body, float scale)
{
	auto pos = body->GetPosition();
	auto angle = body->GetAngle();

	target->SetTransform(
		Matrix3x2F::Rotation(XMConvertToDegrees(angle)) *
		Matrix3x2F::Translation(pos.x, pos.y) *
		Matrix3x2F::Scale(scale, scale));
}

b2Body * Box2dScene::CreateBorderBody(float x, float y, float angle, float length)
{
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_kinematicBody;
	groundBodyDef.position.Set(x, y);
	groundBodyDef.angle = angle;

	b2PolygonShape groundBox;
	groundBox.SetAsBox(length / 2.0f, BorderWidth / 2.0f);

	auto body = _world.CreateBody(&groundBodyDef);
	body->SetUserData((void*)INT_PTR(length * 100));
	body->CreateFixture(&groundBox, 0);
	return body;
}

b2Body * Box2dScene::CreatePersonBody(int personId)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	static uniform_real<float> ur{ 1, 9 };
	bodyDef.position.Set(ur(_rd), ur(_rd));

	auto body = _world.CreateBody(&bodyDef);
	body->SetUserData((void*)(INT_PTR)personId);

	b2PolygonShape shape;
	static uniform_real<float> Rrd;
	auto H = PersonSize / 2;
	shape.SetAsBox(H, H);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 5;
	fixtureDef.friction = 0;
	fixtureDef.restitution = 1;
	body->CreateFixture(&fixtureDef);

	return body;
}

void Box2dScene::FindLuckyPersons()
{
	auto luckyBoard = *_borders.rbegin();
	auto item = luckyBoard->GetContactList();

	std::vector<b2Body*> bodies;
	while (item != nullptr)
	{
		auto personBody = item->other;
		auto userId = (int)(INT_PTR)personBody->GetUserData();
		_luckyPersonIds.insert(userId);
		bodies.push_back(personBody);

		if (_luckyPersonIds.size() == _requiredCount)
		{
			CompleteAndSave();
			break;
		}
		item = item->next;
	}

	for (auto body : bodies) body->SetType(b2_staticBody);
}

void Box2dScene::CompleteAndSave()
{
	_state = State::Completed;
	_updateCount = 0;
	Save();
}

void Box2dScene::EnterTriggerMode()
{
	_state = State::Triggled;
	auto A = RelativeSize;
	auto H = A / 2;
	auto P2 = XM_PIDIV2;
	auto A61 = A / 6;
	auto A65 = A61 * 5;
	auto A3 = A / 3;
	auto rightBorder = _borders.begin() + 2;
	_world.DestroyBody(*rightBorder);
	_borders.erase(rightBorder);

	_borders.push_back(CreateBorderBody(A, A61, P2, A3));
	_borders.push_back(CreateBorderBody(A, A65, P2, A3));
	_borders.push_back(CreateBorderBody(A + H, H, P2, A*0.9f));
}

CD2DPointF ToD2DPoint(const b2Vec2 & v)
{
	return CD2DPointF(v.x, v.y);
}

RectangleGeometry::RectangleGeometry(CRenderTarget * target, const D2D1_RECT_F rect, bool isAutoDestroy) :
	CD2DGeometry(target, isAutoDestroy),
	_rect(rect)
{
}

HRESULT RectangleGeometry::Create(CRenderTarget * pRenderTarget)
{
	auto fac = AfxGetD2DState()->GetDirect2dFactory();
	ID2D1RectangleGeometry* gem;
	auto ok = fac->CreateRectangleGeometry(&_rect, &gem);
	Attach(gem);
	return ok;
}
