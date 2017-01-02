#include "pch.h"
#include "Box2dScene.h"
#include <DirectXMath.h>

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
		_borders.push_back(CreateBorder(H, 0, 0));
		_borders.push_back(CreateBorder(0, H, P2));
		_borders.push_back(CreateBorder(A, H, P2));
		_borders.push_back(CreateBorder(H, A, 0));
	}

	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
	}
}

void Box2dScene::CreateDeviceResources(CHwndRenderTarget * target)
{
	_black = new CD2DSolidColorBrush(target, ColorF(ColorF::Black));
}

void Box2dScene::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	_borderGeometry = new RectangleGeometry(target,
	{
		-RelativeSize / 2.0f,
		-BorderWidth / 2.0f,
		RelativeSize / 2.0f,
		BorderWidth / 2.0f
	});
}

void Box2dScene::Update()
{
	_world.Step(1 / 60.0f, 6, 2);
}

void Box2dScene::Render(CHwndRenderTarget * target)
{
	if (!_show) return;

	auto size = target->GetSize();
	auto scale = 0.1f * min(size.width, size.height);

	for (auto border : _borders)
	{
		auto pos = border->GetPosition();
		auto angle = border->GetAngle();
		auto shape = (b2PolygonShape*)border->GetFixtureList()->GetShape();

		target->SetTransform(
			Matrix3x2F::Rotation(XMConvertToDegrees(angle)) *
			Matrix3x2F::Translation(pos.x, pos.y) *
			Matrix3x2F::Scale(scale, scale));

		target->FillGeometry(_borderGeometry, _black);
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

b2Body * Box2dScene::CreateBorder(float x, float y, float angle)
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
