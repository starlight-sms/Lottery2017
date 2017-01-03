#include "pch.h"
#include "MainWindow.h"
#include "Items.h"
#include "Resources\resource.h"
#include "Person.h"
#include "FlashImageScene.h"
#include "Box2dScene.h"
#include "DxRes.h"

using D2D1::ColorF;
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
	Create(nullptr, L"Lottery 2016", WS_OVERLAPPEDWINDOW, { 100, 100, 800, 600 });
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
		CreateDeviceSizeResources(GetRenderTarget());
	}
}

void MainWindow::OnLottery(UINT id)
{
	auto itemId = id - MenuLotteryStart;
	auto item = GetItems()[itemId];
	auto unluckyIds = GetUnluckyPersonIds();
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
	if (ids.size() == 0)
	{
		CString str;
		str.Format(L"%s 目前无人获奖。", GetItems()[itemId].Name);
		MessageBoxW(str);
	}
	else
	{
		auto path = CreateLuckyStatusFile(itemId, ids);
		ShellExecuteW(GetSafeHwnd(), nullptr, path, nullptr, nullptr, SW_SHOW);
	}
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
	CreateDeviceResources(GetRenderTarget());
	CreateDeviceSizeResources(GetRenderTarget());

	CreateScene(GetItems()[0].Count, 0, GetUnluckyPersonIds());

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

	target->DrawBitmap(LotteryBitmaps[GetLotteryId()], d2dRect);

	for (auto& scene : _scenes)
	{
		scene->Render(target);
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString str;
	str.Format(L"%d", st.wMilliseconds);
	target->DrawTextW(str, d2dRect, _brush);

	return TRUE;
}

LRESULT MainWindow::CreateDeviceResources(WPARAM, LPARAM lparam)
{
	CreateDeviceResources((CHwndRenderTarget*)lparam);
	return 0;
}

void MainWindow::CreateScene(int count, int itemId, const std::vector<int>& personIds)
{
	{
		_scenes.emplace_back(make_unique<FlashImageScene>(count, itemId, personIds));
		(*_scenes.rbegin())->CreateDeviceResources(GetRenderTarget());
		(*_scenes.rbegin())->CreateDeviceSizeResources(GetRenderTarget());
	}

	{
		_scenes.emplace_back(make_unique<Box2dScene>(count, itemId, personIds));
		(*_scenes.rbegin())->CreateDeviceResources(GetRenderTarget());
		(*_scenes.rbegin())->CreateDeviceSizeResources(GetRenderTarget());
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

void MainWindow::CreateDeviceResources(CHwndRenderTarget * target)
{
	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		LotteryBitmaps.push_back(new CD2DBitmap(target, GetItems()[i].ResourceId, L"Item"));
		HR((*LotteryBitmaps.rbegin())->Create(target));
	}

	_brush = new CD2DSolidColorBrush(target, ColorF(ColorF::Yellow));
}

void MainWindow::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	for (auto & scene : _scenes)
	{
		scene->CreateDeviceSizeResources(target);
	}
}