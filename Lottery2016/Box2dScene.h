#pragma once
#include "FlashImageScene.h"

class RectangleGeometry final : public CD2DGeometry
{
public:
	RectangleGeometry(CRenderTarget* target, const D2D1_RECT_F rect, bool isAutoDestroy = true);
	virtual HRESULT Create(CRenderTarget * pRenderTarget) override;

private:
	D2D1_RECT_F _rect;
};

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

	std::vector<b2Body*> _borders;

	const std::vector<int> _allPersonIds;
	b2World _world;

	b2Body* CreateBorder(float x, float y, float angle);

	// dxres
	CD2DSolidColorBrush *_black;
	RectangleGeometry* _borderGeometry;
};