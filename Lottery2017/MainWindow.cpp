#include "pch.h"
#include "MainWindow.h"
#include "Items.h"
#include "Resources\resource.h"
#include "Person.h"
#include "FlashImageScene.h"
#include "LuckyScene.h"
#include "Box2dScene.h"
#include "DxRes.h"

using D2D1::ColorF;
using D2D1::Matrix3x2F;
using namespace std;

const UINT ControlCombox = 1;
const UINT MenuLotteryStart = 2;
const UINT MenuLotteryLast() { return MenuLotteryStart + GetItems().size(); }
const UINT MenuStatusStart() { return MenuLotteryLast() + 1; };
const UINT MenuStatusLast() { return MenuStatusStart() + GetItems().size(); };
const UINT MenuStatusClear() { return MenuStatusLast() + 1; };

BEGIN_MESSAGE_MAP(MainWindow, CFrameWnd)
	ON_COMMAND_RANGE(MenuLotteryStart, MenuLotteryLast(), OnLottery)
	ON_COMMAND_RANGE(MenuStatusStart(), MenuStatusLast(), OnStatus)
	ON_COMMAND(MenuStatusClear(), OnLotteryClear)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, OnDraw2D)
	ON_REGISTERED_MESSAGE(AFX_WM_RECREATED2DRESOURCES, CreateDeviceResources)
	ON_WM_KEYUP()
END_MESSAGE_MAP()

MainWindow::MainWindow()
{
	Create(nullptr, L"Lottery 2017", WS_OVERLAPPEDWINDOW, { 100, 100, 800, 600 });
}

void MainWindow::Update()
{
	for (auto& scene : _scenes)
	{
		scene->Update();
	}
}

void MainWindow::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	for (auto& scene : _scenes)
	{
		scene->KeyUp(nChar);
	}
}

void MainWindow::OnSize(UINT nType, int cx, int cy)
{
	if (nType != SIZE_MINIMIZED && GetRenderTarget())
	{
		_dxRes.CreateDeviceSizeResources(GetRenderTarget());
	}
}

void MainWindow::OnLottery(UINT id)
{
	auto itemId = id - MenuLotteryStart;
	auto item = GetItems()[itemId];
	auto unluckyIds = GetUnluckyPersonIds(itemId);
	auto needCount = item.Count;

	if (unluckyIds.empty())
	{
		MessageBoxW(L"人数不够");
		return;
	}

	if (needCount > (int)unluckyIds.size())
	{
		CString str;
		str.Format(L"剩余人数(%d)不足抽(%s)所需要的(%d)人,是否调整为抽(%d)人并继续?",
			unluckyIds.size(), item.Name, needCount, unluckyIds.size());
		if (MessageBoxW(str, nullptr, MB_YESNO) == IDNO)
		{
			return;
		}
		needCount = (int)unluckyIds.size();
	}

	auto luckyCount = GetLuckyPersonIds(itemId).size();
	if (luckyCount > 0)
	{
		CString str;
		str.Format(L"已有 %d 人抽中 %s, 是否继续?", luckyCount, item.Name);
		if (MessageBoxW(str, nullptr, MB_YESNO) == IDNO)
		{
			return;
		}
	}

	_menu.CheckMenuRadioItem(MenuLotteryStart, MenuLotteryLast(), id, MF_BYCOMMAND);

	_scenes.clear();
	CreateScene(needCount, itemId, unluckyIds);
}

void MainWindow::OnStatus(UINT id)
{
	auto itemId = id - MenuStatusStart();
	auto ids = GetLuckyPersonIds(itemId);
	_scenes.clear();
	CreateScene(-1, itemId, ids);
}

int MainWindow::OnCreate(LPCREATESTRUCT cs)
{
	// setup menu
	_menu.LoadMenuW(IDR_MENU1);
	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		auto item = GetItems()[i];
		auto lotteryMenu = _menu.GetSubMenu(1);
		auto statusMenu = _menu.GetSubMenu(2);

		CString str;
		str.Format(L"%s(&%d)", item.Name, i + 1);
		lotteryMenu->AppendMenuW(MF_STRING, MenuLotteryStart + i, str);
		statusMenu->AppendMenuW(MF_STRING, MenuStatusStart() + i, str);
	}
	_menu.GetSubMenu(1)->RemoveMenu(0, 0);
	_menu.GetSubMenu(2)->RemoveMenu(0, 0);
	_menu.GetSubMenu(2)->AppendMenuW(MF_SEPARATOR);
	_menu.GetSubMenu(2)->AppendMenuW(MF_STRING, MenuStatusClear(), L"清除状态(&C)");
	_menu.CheckMenuRadioItem(MenuLotteryStart, MenuLotteryLast(), MenuLotteryStart, MF_BYCOMMAND);
	SetMenu(&_menu);

	// d2d
	EnableD2DSupport();
	_dxRes.CreateDeviceResources(GetRenderTarget());
	_dxRes.CreateDeviceSizeResources(GetRenderTarget());

	CreateScene(GetItems()[0].Count, 0, GetUnluckyPersonIds(0));

	return TRUE;
}

void MainWindow::OnLotteryClear()
{
	if (MessageBoxW(L"确定要清空抽奖状态吗?", nullptr, MB_YESNO) == IDYES)
	{
		DeleteLuckyPersons();
		MessageBoxW(L"所有抽奖状态已清除。");
	}
}

LRESULT MainWindow::OnDraw2D(WPARAM, LPARAM lparam)
{
	Update();

	auto target = (CHwndRenderTarget*)lparam;
	ASSERT_VALID(target);

	RECT rect;
	GetClientRect(&rect);
	CD2DRectF d2dRect{ (float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom };

	target->DrawBitmap(_dxRes.Background, d2dRect);

	{
		auto bmp = _dxRes.LotteryBitmaps[GetLotteryId()];
		auto width = d2dRect.right - d2dRect.left;
		auto height = d2dRect.bottom - d2dRect.top;
		auto scale = 1.f / 3;
		target->SetTransform(
			Matrix3x2F::Scale(scale, scale) *
			Matrix3x2F::Translation(width * (1 - scale) - 10, height * (1 - scale) - 10)
		);
		target->DrawBitmap(bmp, d2dRect);
		target->SetTransform(Matrix3x2F::Identity());
	}

	for (auto& scene : _scenes)
	{
		scene->Render(target, &_dxRes);
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString str;
	str.Format(L"%d", st.wMilliseconds);
	target->DrawTextW(str, d2dRect, _dxRes.GetColorBrush(target, ColorF::Blue));

	return TRUE;
}

LRESULT MainWindow::CreateDeviceResources(WPARAM, LPARAM lparam)
{
	_dxRes.ClearDeviceResources();
	_dxRes.CreateDeviceResources((CHwndRenderTarget*)lparam);
	return 0;
}

void MainWindow::CreateScene(int count, int itemId, const std::vector<int>& personIds)
{
	if (count == -1)
	{
		_scenes.emplace_back(make_unique<LuckyScene>(itemId, personIds));
	}
	else if (itemId != 3)
	{
		_scenes.emplace_back(make_unique<Box2dScene>(count, itemId, personIds));
	}
	else
	{
		_scenes.emplace_back(make_unique<FlashImageScene>(count, itemId, personIds));
	}
}

size_t MainWindow::GetLotteryId()
{
	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		if (_menu.GetMenuState(UINT(i + MenuLotteryStart), MF_BYCOMMAND) & MF_CHECKED)
		{
			return i;
		}
	}
	return 0;
}