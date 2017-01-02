#include "pch.h"
#include "FlashImageScene.h"
#include <Box2D\Box2D.h>
#include "DxRes.h"
#include "Person.h"

using namespace std;

CD2DSizeF Divide(CD2DSizeF v, float x, float y);
CD2DSizeF Multiple(CD2DSizeF v, float x, float y);
CD2DSizeF GetDisplaySize(const CD2DSizeF& inputSize, const CD2DSizeF& clampSize);
CD2DRectF GetDrawCenterRect(const CD2DSizeF& topLeft, const CD2DSizeF& gridSize, const CD2DSizeF& realSize);

FlashImageScene::FlashImageScene(int count, std::vector<int> persons) :
	_count(count),
	_allPersonIds(move(persons)),
	_mt19937(random_device()())
{
	if (_allPersonIds.size() < _count)
	{
		throw make_exception_ptr("allPersionIds size must >= count.");
	}
}

void FlashImageScene::Step()
{
	if (_started)
	{
		shuffle(begin(_allPersonIds), end(_allPersonIds), _mt19937);
		_selectedPersonIds.assign(begin(_allPersonIds), begin(_allPersonIds) + _count);
	}	
}

const std::vector<int>& FlashImageScene::GetSelectedPersonIds() const
{
	return _selectedPersonIds;
}

bool FlashImageScene::Toggle()
{
	if (_started)
	{
		_started = false;
	}
	else if (!IsCompleted())
	{
		_started = true;
	}

	return IsCompleted();
}

void FlashImageScene::Render(CHwndRenderTarget * target, DxRes * dxRes)
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
		auto bmp = dxRes->PersonBitmaps[id];
		auto realSize = GetDisplaySize(bmp->GetSize(), gridSize);

		auto rect = GetDrawCenterRect(topLeft, gridSize, realSize);
		target->DrawBitmap(bmp, rect);
		CString str = GetAllPerson()[id].Name;
		if (!_started)
		{
			str.Append(L"\r\n");
			str.Append(GetAllPerson()[id].Notes);
		}
		target->DrawTextW(str, rect, dxRes->Blue, dxRes->TextFormat);

		++col;
		row += col / maxCol;
		col = col % maxCol;
	}
}

bool FlashImageScene::IsCompleted()
{
	return !_started && _selectedPersonIds.size() > 0;
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
