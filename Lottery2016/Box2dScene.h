#pragma once
#include "FlashImageScene.h"

struct Person;
class RectangleGeometry;

class Box2dScene final : public Scene
{
public:
	Box2dScene(int count, int itemId, const std::vector<int>& personIds);
	virtual void CreateDeviceResources(CHwndRenderTarget * target) override;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget * target) override;
	virtual void Update() override;
	virtual void Render(CHwndRenderTarget * target) override;
	virtual void KeyUp(UINT key) override;

private:
	const float RelativeSize = 10.0f;
	const float BorderWidth = RelativeSize / 100;
	const int _count;
	const int _itemId;
	bool _show = false;

	std::mt19937 _rd{ std::random_device{}() };

	const std::vector<int> _allPersonIds;

	// mist
	void PreRenderBody(CHwndRenderTarget* target, b2Body * body);

	// b2 related
	std::vector<b2Body*> _borders;
	std::vector<b2Body*> _personBodies;
	b2World _world;
	int _updateCount = 0;
	const float CircleSize = 0.25;

	b2Body* CreateBorderBody(float x, float y, float angle);
	b2Body* CreatePersonBody(int personId);

	// dxres
	float _scale;
	CD2DSolidColorBrush *_borderBrush;
	std::unordered_map<int, CD2DBitmapBrush *> _personBrushes;
	RectangleGeometry* _borderGeometry;
};

class RectangleGeometry final : public CD2DGeometry
{
public:
	RectangleGeometry(CRenderTarget* target, const D2D1_RECT_F rect, bool isAutoDestroy = true);
	virtual HRESULT Create(CRenderTarget * pRenderTarget) override;

private:
	D2D1_RECT_F _rect;
};