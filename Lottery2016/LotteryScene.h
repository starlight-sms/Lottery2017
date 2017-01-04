#pragma once

#include "DxRes.h"

class Scene : public DxRes
{
public:
	virtual void Update() = 0;
	virtual void Render(CHwndRenderTarget* target) = 0;
	virtual void KeyUp(UINT key) = 0;
};

class LotteryScene : public Scene
{
public:
	LotteryScene(int count, int itemId, std::vector<int> personIds);

	// Í¨¹ý Scene ¼Ì³Ð
	virtual void CreateDeviceSizeResources(CHwndRenderTarget * target) override;
	virtual void CreateDeviceResources(CHwndRenderTarget * target) override;
protected:
	CD2DSizeF Divide(CD2DSizeF v, float x, float y);
	CD2DSizeF Multiple(CD2DSizeF v, float x, float y);
	CD2DSizeF GetDisplaySize(const CD2DSizeF& inputSize, const CD2DSizeF& scaleSize);
	CD2DRectF GetDrawCenterRect(const CD2DSizeF& topLeft, const CD2DSizeF& gridSize, const CD2DSizeF& realSize);

	const void Save();
	
	const int _itemId;
	const int _requiredCount;
	std::vector<int> _allPersonIds;
	std::unordered_set<int> _luckyPersonIds;
	std::mt19937 _rd;

	// d2d res
	CD2DTextFormat *_headerTextFormat, *_textFormat;
	CD2DSolidColorBrush* GetColorBrush(CHwndRenderTarget*, D2D1::ColorF::Enum color);

private:
	std::unordered_map<D2D1::ColorF::Enum, CD2DSolidColorBrush*> _brushCache;
};

class RectangleGeometry final : public CD2DGeometry
{
public:
	RectangleGeometry(CRenderTarget* target, const D2D1_RECT_F rect, bool isAutoDestroy = true);
	virtual HRESULT Create(CRenderTarget * pRenderTarget) override;

private:
	D2D1_RECT_F _rect;
};