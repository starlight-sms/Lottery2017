#pragma once

class FlashImageScene;
class DxRes;

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
		
	CMenu _menu;	
	size_t GetLotteryId();
	std::unique_ptr<FlashImageScene> _scene;

	std::unique_ptr<DxRes> _dxRes;
};