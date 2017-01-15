#pragma once

class RectangleGeometry;

const float PersonSize = 0.45f;

class DxRes
{
public:
	void CreateDeviceResources(CHwndRenderTarget *target);
	void CreateDeviceSizeResources(CHwndRenderTarget *target);
	void ClearDeviceResources();

	CD2DSolidColorBrush* GetColorBrush(CHwndRenderTarget*, D2D1::ColorF::Enum color);
	RectangleGeometry* GetOrCreateBorderGeometry(CHwndRenderTarget* target, float w, float h);

	CD2DTextFormat *HeaderTextFormat, *TextFormat, *H1TextFormat;
	std::vector<CD2DBitmap*> LotteryBitmaps;
	CD2DBitmap* Background;
	std::unordered_map<int, CD2DBitmapBrush *> PersonBrushes;
	std::unordered_map<float, RectangleGeometry*> BorderGeometries;
	std::unordered_map<int, CD2DBitmap*> PersonBitmaps;
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