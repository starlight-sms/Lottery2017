#include "pch.h"
#include "LuckyScene.h"
#include <Box2D\Box2D.h>
#include "DxRes.h"
#include "Person.h"
#include "Items.h"

using namespace std;
using D2D1::ColorF;

LuckyScene::LuckyScene(int itemId, std::vector<int> luckyPersonIds) :
	_itemId(itemId),
	_luckyPersonIds(luckyPersonIds)
{
}

void LuckyScene::Update()
{
}

void LuckyScene::Render(CHwndRenderTarget * target, DxRes* dxRes)
{
	if (_luckyPersonIds.size() == 0)
	{
		RenderNoLuckyPerson(target, dxRes);
		return;
	}
	auto windowSize = target->GetSize();
	auto whRate = windowSize.width / windowSize.height;
	auto maxCol = (int)ceil(sqrt(_luckyPersonIds.size() * whRate));
	auto maxRow = (int)ceil(1.0 * _luckyPersonIds.size() / maxCol);

	auto gridSize = Divide(windowSize, (float)maxCol, (float)maxRow);
	auto lastCol = _luckyPersonIds.size() - (maxRow - 1) * maxCol;
	auto lastGridSize = Divide(windowSize, (float)lastCol, (float)maxRow);

	int col = 0, row = 0;
	for (auto id : _luckyPersonIds)
	{
		auto grid = (int)row < maxRow - 1 ? gridSize : lastGridSize;
		auto topLeft = Multiple(grid, (float)col, (float)row);
		auto bmp = dxRes->PersonBitmaps[id];

		auto realSize = GetDisplaySize(bmp->GetSize(), grid);
		auto rect = GetDrawCenterRect(topLeft, grid, realSize);
		target->DrawBitmap(bmp, rect);

		CString str = GetAllPerson()[id].Name;
		target->DrawTextW(str, rect, dxRes->GetColorBrush(target, ColorF::Purple), dxRes->HeaderTextFormat);

		row += ++col / maxCol;
		col = col % maxCol;
	}
}

void LuckyScene::RenderNoLuckyPerson(CHwndRenderTarget * target, DxRes * dxRes)
{
	auto size = target->GetSize();
	auto rect = CD2DRectF{ 0.f, 0.f, size.width, size.height };

	CString str;
	str.Format(L"%s 目前无人中奖。", GetItems()[_itemId].Name);
	target->DrawTextW(str, rect, dxRes->GetColorBrush(target, ColorF::Yellow), dxRes->H1TextFormat);
}

void LuckyScene::KeyUp(UINT key)
{
}
