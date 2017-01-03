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
	_count(count),
	_itemId(count),
	_allPersonIds(personIds),
	_world{ b2Vec2{0, 5} }
{
	{
		auto H = RelativeSize / 2;
		auto A = RelativeSize;
		auto P2 = XM_PIDIV2;
		_borders.push_back(CreateBorderBody(H, 0, 0));
		_borders.push_back(CreateBorderBody(0, H, P2));
		_borders.push_back(CreateBorderBody(A, H, P2));
		_borders.push_back(CreateBorderBody(H, A, 0));
	}

	for (auto id : personIds)
	{
		_personBodies.push_back(CreatePersonBody(id));
	}
}

void Box2dScene::CreateDeviceResources(CHwndRenderTarget * target)
{
	_borderBrush = new CD2DSolidColorBrush(target, ColorF(ColorF::Yellow));
	for (auto i : _allPersonIds)
	{
		_personBrushes[i] = new CD2DBitmapBrush(target, GetAllPerson()[i].ResourceId, L"Person");
		HR(_personBrushes[i]->Create(target));
	}
}

void Box2dScene::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	auto size = target->GetSize();
	_scale = 0.1f * min(size.width, size.height);
	_borderGeometry = new RectangleGeometry(target,
	{
		-RelativeSize / 2.0f,
		-BorderWidth / 2.0f,
		RelativeSize / 2.0f,
		BorderWidth / 2.0f
	});

	for (auto i : _allPersonIds)
	{
		auto bitmapSize = _personBrushes[i]->GetBitmap()->GetPixelSize();
		auto minEdge = std::min(bitmapSize.width, bitmapSize.height);
		auto transform =
			Matrix3x2F::Translation(
				-float(bitmapSize.width - minEdge) / 2,
				-float(bitmapSize.height - minEdge) / 2) *
			Matrix3x2F::Scale(CircleSize / minEdge / 2, CircleSize / minEdge / 2);
		_personBrushes[i]->SetTransform(&transform);
	}
}

void Box2dScene::Update()
{
	_world.Step(1 / 30.0f, 6, 2);
	if (++_updateCount == 150)
	{
		_world.SetGravity({ 0, 0 });
	}
}

void Box2dScene::Render(CHwndRenderTarget * target)
{
	if (!_show) return;

	auto size = target->GetSize();

	for (auto border : _borders)
	{
		PreRenderBody(target, border);
		auto shape = (b2PolygonShape*)border->GetFixtureList()->GetShape();
		target->FillGeometry(_borderGeometry, _borderBrush);
	}
	for (auto person : _personBodies)
	{
		PreRenderBody(target, person);
		auto shape = (b2CircleShape*)person->GetFixtureList()->GetShape();
		CD2DEllipse ellipse{ {0.f, 0.f}, {shape->m_radius, shape->m_radius} };
		auto userId = (int)person->GetUserData();
		target->FillEllipse(ellipse, _personBrushes[userId]);
	}
	target->SetTransform(Matrix3x2F::Identity());
}

void Box2dScene::KeyUp(UINT key)
{
	if (key == 'S')
	{
		_show = !_show;
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

b2Body * Box2dScene::CreateBorderBody(float x, float y, float angle)
{
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_kinematicBody;
	groundBodyDef.position.Set(x, y);
	groundBodyDef.angle = angle;

	b2PolygonShape groundBox;
	groundBox.SetAsBox(RelativeSize, BorderWidth);

	auto body = _world.CreateBody(&groundBodyDef);
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
	body->SetUserData((void*)personId);

	b2CircleShape shape;
	shape.m_radius = CircleSize;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 25;
	fixtureDef.friction = 0;
	fixtureDef.restitution = 1;
	body->CreateFixture(&fixtureDef);

	return body;
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
