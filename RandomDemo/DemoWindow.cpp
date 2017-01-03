#include "pch.h"
#include "DemoWindow.h"
#include "resource.h"
#include "DemoScene.h"

using namespace std;
using D2D1::ColorF;

BEGIN_MESSAGE_MAP(DemoWindow, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, OnDraw2D)
	ON_COMMAND_RANGE(ID_TEST_RAND, ID_TEST_MINSTD, OnIdSelected)
END_MESSAGE_MAP()

DemoWindow::DemoWindow()
{
	Create(nullptr, L"Random Demo Window");
}

int DemoWindow::OnCreate(LPCREATESTRUCT)
{
	_menu.LoadMenuW(IDR_MENU1);
	SetMenu(&_menu);

	EnableD2DSupport();
	
	return 0;
}

void DemoWindow::OnSize(UINT type, int, int)
{
	if (type != SIZE_MINIMIZED && GetRenderTarget())
	{
		for (auto & scene : _scenes)
		{
			scene->CreateDeviceSizeResources(GetRenderTarget());
		}
	}
}

LRESULT DemoWindow::OnDraw2D(WPARAM, LPARAM lparam)
{
	auto target = (CHwndRenderTarget*)lparam;
	target->Clear(ColorF(ColorF::CornflowerBlue));
	for (auto & scene : _scenes)
	{
		scene->Update();
		scene->Render(target);
	}
	return true;
}

void DemoWindow::OnIdSelected(UINT id)
{
	_scenes.clear();
	CreateScene(id);
}

void DemoWindow::CreateScene(int id)
{
	_scenes.emplace_back(RandomDemoScene::Make(id));
	(*_scenes.rbegin())->CreateDeviceResources(GetRenderTarget());
	(*_scenes.rbegin())->CreateDeviceSizeResources(GetRenderTarget());
}
