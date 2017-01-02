#pragma once

class DxRes
{
public:
	void CreateDeviceResources(CHwndRenderTarget *target);
	void CreateDeviceSizeResources(CHwndRenderTarget *target);

	std::vector<CD2DBitmap*> LotteryBitmaps;
	std::vector<CD2DBitmap*> PersonBitmaps;
	CD2DSolidColorBrush* Blue;
	CD2DTextFormat* TextFormat = nullptr;
};