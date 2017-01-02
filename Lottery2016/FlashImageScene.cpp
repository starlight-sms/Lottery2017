#include "pch.h"
#include "FlashImageScene.h"
#include <Box2D\Box2D.h>
#include "DxRes.h"
#include "Person.h"

using namespace std;
using D2D1::ColorF;

CD2DSizeF Divide(CD2DSizeF v, float x, float y);
CD2DSizeF Multiple(CD2DSizeF v, float x, float y);
CD2DSizeF GetDisplaySize(const CD2DSizeF& inputSize, const CD2DSizeF& clampSize);
CD2DRectF GetDrawCenterRect(const CD2DSizeF& topLeft, const CD2DSizeF& gridSize, const CD2DSizeF& realSize);

FlashImageScene::FlashImageScene(int count, int itemId, std::vector<int> persons) :
	_count(count),
	_itemId(itemId), 
	_allPersonIds(move(persons)),
	_mt19937(random_device()())
{
	if (_allPersonIds.size() < _count)
	{
		throw make_exception_ptr("allPersionIds size must >= count.");
	}
}

void FlashImageScene::Update()
{
	if (_started)
	{
		shuffle(begin(_allPersonIds), end(_allPersonIds), _mt19937);
		_selectedPersonIds.assign(begin(_allPersonIds), begin(_allPersonIds) + _count);
	}	
}

void FlashImageScene::Render(CHwndRenderTarget * target)
{
	auto windowSize = target->GetSize();
	auto whRate = windowSize.width / windowSize.height;
	auto maxCol = (int)ceil(sqrt(_count * whRate));
	auto maxRow = (int)ceil(1.0 * _count / maxCol);
		
	auto gridSize = Divide(windowSize, (float)maxCol, (float)maxRow);

	for (size_t i = 0, col = 0, row = 0; i < _selectedPersonIds.size(); ++i)
	{
		auto id = _selectedPersonIds[i];
		auto topLeft = Multiple(gridSize, (float)col, (float)row);
		auto bmp = _personBitmaps[id];
		auto realSize = GetDisplaySize(bmp->GetSize(), gridSize);

		auto rect = GetDrawCenterRect(topLeft, gridSize, realSize);
		target->DrawBitmap(bmp, rect);
		CString str = GetAllPerson()[id].Name;
		if (!_started)
		{
			str.Append(L"\r\n");
			str.Append(GetAllPerson()[id].Notes);
		}
		target->DrawTextW(str, rect, _blue, _textFormat);

		++col;
		row += col / maxCol;
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
			// save...
			SaveLuckyPersonIds(_itemId, _selectedPersonIds);
		}
		else if (_selectedPersonIds.size() == 0)
		{
			_started = true;
		}
	}
}

void FlashImageScene::CreateDeviceResources(CHwndRenderTarget * target)
{
	for (size_t i = 0; i < GetAllPerson().size(); ++i)
	{
		_personBitmaps.push_back(new CD2DBitmap(target, GetAllPerson()[i].ResourceId, L"Person"));
		HR((*_personBitmaps.rbegin())->Create(target));
	}
	_blue = new CD2DSolidColorBrush(target, ColorF(ColorF::Blue));
}

void FlashImageScene::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	if (_textFormat)
		_textFormat->Destroy();

	auto size = target->GetSize();
	auto minEdge = std::min(size.width, size.height);
	auto real = b2Clamp(minEdge / 40, 12.0f, 24.0f);
	_textFormat = new CD2DTextFormat(target, L"Consolas", real);
}

CD2DSizeF Divide(CD2DSizeF v, float x, float y)
{
	v.width /= x;
	v.height /= y;
	return v;
}

CD2DSizeF Multiple(CD2DSizeF v, float x, float y)
{
	v.width *= x;
	v.height *= y;
	return v;
}

CD2DSizeF GetDisplaySize(const CD2DSizeF & inputSize, const CD2DSizeF & scaleSize)
{
	auto inputRate = inputSize.width / inputSize.height;
	auto clampRate = scaleSize.width / scaleSize.height;

	float scale;
	if (inputRate > clampRate)
	{
		scale = inputSize.width / scaleSize.width;
	}
	else
	{
		scale = inputSize.height / scaleSize.height;
	}
	return Divide(inputSize, scale, scale);
}

CD2DRectF GetDrawCenterRect(const CD2DSizeF & topLeft, const CD2DSizeF & gridSize, const CD2DSizeF & realSize)
{
	return CD2DRectF(
		topLeft.width  + (gridSize.width  - realSize.width)  / 2, 
		topLeft.height + (gridSize.height - realSize.height) / 2, 
		topLeft.width  + (gridSize.width  - realSize.width)  / 2 + realSize.width,
		topLeft.height + (gridSize.height - realSize.height) / 2 + realSize.height
	);
}
