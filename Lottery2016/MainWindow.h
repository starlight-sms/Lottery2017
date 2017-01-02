#pragma once

class Scene;
class MainWindowRes;
#include "DxRes.h"

class MainWindow : public CFrameWnd
{
public:
	MainWindow();

private:
	DECLARE_MESSAGE_MAP()

	void Update();

	void OnKeyUp(UINT, UINT, UINT);
	void OnSize(UINT, int, int);
	void OnLottery(UINT id);
	void OnStatus(UINT id);
	int OnCreate(LPCREATESTRUCT);
	void OnLotteryClear();
	LRESULT OnDraw2D(WPARAM, LPARAM);

	LRESULT CreateDeviceResources(WPARAM, LPARAM);

	void CreateScene(int count, int itemId, const std::vector<int>& personIds);
		
	CMenu _menu;	
	size_t GetLotteryId();
	std::vector<std::unique_ptr<Scene>> _scenes;

	std::unique_ptr<MainWindowRes> _dxRes;
};

class MainWindowRes : public DxRes
{
public:
	std::vector<CD2DBitmap*> LotteryBitmaps;
	CD2DSolidColorBrush* Blue;

	virtual void CreateDeviceResources(CHwndRenderTarget * target) override;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget * target) override;
};