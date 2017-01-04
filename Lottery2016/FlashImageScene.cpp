#include "pch.h"
#include "FlashImageScene.h"
#include <Box2D\Box2D.h>
#include "DxRes.h"
#include "Person.h"

using namespace std;
using D2D1::ColorF;

FlashImageScene::FlashImageScene(int count, int itemId, std::vector<int> persons) :
	LotteryScene(count, itemId, persons)
{
	if (_allPersonIds.size() < (size_t)_requiredCount)
	{
		throw make_exception_ptr("allPersionIds size must >= count.");
	}
}

void FlashImageScene::Update()
{
	if (_started)
	{
		shuffle(begin(_allPersonIds), end(_allPersonIds), _rd);
		_luckyPersonIds.clear();
		for (int i = 0; i < _requiredCount; ++i)
		{
			_luckyPersonIds.insert(_allPersonIds[i]);
		}
	}	
}

void FlashImageScene::Render(CHwndRenderTarget * target)
{
	auto windowSize = target->GetSize();
	auto whRate = windowSize.width / windowSize.height;
	auto maxCol = (int)ceil(sqrt(_requiredCount * whRate));
	auto maxRow = (int)ceil(1.0 * _requiredCount / maxCol);
		
	auto gridSize = Divide(windowSize, (float)maxCol, (float)maxRow);
	auto lastCol = _requiredCount - (maxRow - 1) * maxCol;
	auto lastGridSize = Divide(windowSize, (float)lastCol, (float)maxRow);

	int col = 0, row = 0;
	for (auto id : _luckyPersonIds)
	{
		auto grid = (int)row < maxRow - 1 ? gridSize : lastGridSize;
		auto topLeft = Multiple(grid, (float)col, (float)row);
		auto bmp = _personBitmaps[id];
		
		auto realSize = GetDisplaySize(bmp->GetSize(), grid);
		auto rect = GetDrawCenterRect(topLeft, grid, realSize);
		target->DrawBitmap(bmp, rect);

		CString str = GetAllPerson()[id].Name;
		target->DrawTextW(str, rect, GetColorBrush(target, ColorF::Red), _headerTextFormat);
		if (!_started)
		{
			CString notes(L"\r\n");
			notes.Append(GetAllPerson()[id].Notes);
			target->DrawTextW(notes, rect, GetColorBrush(target, ColorF::OrangeRed), _textFormat);
		}		

		row += ++col / maxCol;
		col = col % maxCol;
	}
}

void FlashImageScene::KeyUp(UINT key)
{
	if (key == VK_SPACE)
	{
		if (_started)
		{
			_started = false;
			Save();
		}
		else if (_luckyPersonIds.size() == 0)
		{
			_started = true;
		}
		else
		{
			AfxGetMainWnd()->MessageBoxW(L"此奖项已抽奖完成，可在以下位置开始新的抽奖：\n菜单->抽奖->选择奖项。");
		}
	}
}

void FlashImageScene::CreateDeviceResources(CHwndRenderTarget * target)
{
	__super::CreateDeviceResources(target);
	for (size_t i = 0; i < _allPersonIds.size(); ++i)
	{
		auto id = _allPersonIds[i];
		_personBitmaps[id] = new CD2DBitmap(target, GetAllPerson()[i].ResourceId, L"Person");
		HR(_personBitmaps[id]->Create(target));
	}
}