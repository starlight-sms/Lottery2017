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

void Box2dScene::CreateDeviceResources(CHwndRenderTarget * target)
{
	__super::CreateDeviceResources(target);
	for (auto i : _allPersonIds)
	{
		_personBrushes[i] = new CD2DBitmapBrush(target, GetAllPerson()[i].ResourceId, L"Person");
		HR(_personBrushes[i]->Create(target));
	}
}

void Box2dScene::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	__super::CreateDeviceSizeResources(target);
	auto size = target->GetSize();
	_scale = 0.1f * min(size.width, size.height);

	for (auto i : _allPersonIds)
	{
		auto bitmapSize = _personBrushes[i]->GetBitmap()->GetPixelSize();
		auto minEdge = (float)std::min(bitmapSize.width, bitmapSize.height);
		auto transform =
			Matrix3x2F::Translation(
				-float(bitmapSize.width - minEdge) / 2,
				-float(bitmapSize.height - minEdge) / 2) *
			Matrix3x2F::Translation(-minEdge / 2, -minEdge / 2) *
			Matrix3x2F::Scale(PersonSize / minEdge, PersonSize / minEdge);
		_personBrushes[i]->SetTransform(&transform);
	}
}

void Box2dScene::Update()
{
	if (_state == State::Started)
	{
		_world.Step(1 / 30.0f, 6, 2);
		if (++_updateCount == 150)
		{
			_world.SetGravity({ 0, 0 });
		}
	}
	else if (_state == State::Triggled)
	{
		_world.Step(1 / 300.0f, 6, 2);
		FindLuckyPersons();
	}
}

void Box2dScene::Render(CHwndRenderTarget * target)
{
	if (_state == State::Pending) return;

	auto size = target->GetSize();

	for (auto border : _borders)
	{
		PreRenderBody(target, border);
		auto length = (INT_PTR)border->GetUserData() / 100.0f;
		auto shape = (b2PolygonShape*)border->GetFixtureList()->GetShape();
		target->FillGeometry(GetOrCreateBorderGeometry(target, length), GetColorBrush(target, _borderColor));
	}
	for (auto person : _personBodies)
	{
		PreRenderBody(target, person);
		auto shape = (b2PolygonShape*)person->GetFixtureList()->GetShape();
		auto userId = (int)person->GetUserData();
		auto brush = _personBrushes[userId];
		auto H = PersonSize / 2;
		target->FillRectangle({ -H, -H, H, H }, brush);

		if (_luckyPersonIds.find(userId) != _luckyPersonIds.end())
		{
			target->DrawRectangle({ -H, -H, H, H }, GetColorBrush(target, _luckyColor), 3 / _scale);
		}
	}
	target->SetTransform(Matrix3x2F::Identity());
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

void Box2dScene::PreRenderBody(CHwndRenderTarget* target, b2Body * body)
{
	auto pos = body->GetPosition();
	auto angle = body->GetAngle();

	target->SetTransform(
		Matrix3x2F::Rotation(XMConvertToDegrees(angle)) *
		Matrix3x2F::Translation(pos.x, pos.y) *
		Matrix3x2F::Scale(_scale, _scale));
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
	_borders.push_back(CreateBorderBody(A + H, H, P2, A));
}

RectangleGeometry * Box2dScene::GetOrCreateBorderGeometry(CHwndRenderTarget* target, float length)
{
	if (_borderGeometries.find(length) == _borderGeometries.end())
	{
		_borderGeometries[length] = new RectangleGeometry(target,
		{
			-length / 2.0f,
			-BorderWidth / 2.0f,
			length / 2.0f,
			BorderWidth / 2.0f
		});
		HR(_borderGeometries[length]->Create(target));
	}
	return _borderGeometries[length];
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
