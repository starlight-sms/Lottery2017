#include "pch.h"
#include "LotteryScene.h"
#include "Person.h"

using namespace std;
using D2D1::ColorF;

LotteryScene::LotteryScene(int count, int itemId, std::vector<int> personIds) :
	_requiredCount(count),
	_itemId(itemId),
	_allPersonIds(move(personIds)),
	_rd(random_device{}())
{
}

void LotteryScene::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	if (_headerTextFormat)
		_headerTextFormat->Destroy();

	auto size = target->GetSize();
	auto minEdge = std::min(size.width, size.height);
	auto real = b2Clamp(minEdge / 40, 12.0f, 24.0f);
	_headerTextFormat = new CD2DTextFormat(target, L"Consolas", real);
	_textFormat = new CD2DTextFormat(target, L"Consolas", real * 0.8f);
}

CD2DSizeF LotteryScene::Divide(CD2DSizeF v, float x, float y)
{
	v.width /= x;
	v.height /= y;
	return v;
}

CD2DSizeF LotteryScene::Multiple(CD2DSizeF v, float x, float y)
{
	v.width *= x;
	v.height *= y;
	return v;
}

CD2DSizeF LotteryScene::GetDisplaySize(const CD2DSizeF & inputSize, const CD2DSizeF & scaleSize)
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

CD2DRectF LotteryScene::GetDrawCenterRect(const CD2DSizeF & topLeft, const CD2DSizeF & gridSize, const CD2DSizeF & realSize)
{
	return CD2DRectF(
		topLeft.width + (gridSize.width - realSize.width) / 2,
		topLeft.height + (gridSize.height - realSize.height) / 2,
		topLeft.width + (gridSize.width - realSize.width) / 2 + realSize.width,
		topLeft.height + (gridSize.height - realSize.height) / 2 + realSize.height
	);
}

const void LotteryScene::Save()
{
	SaveLuckyPersonIds(_itemId, _luckyPersonIds);
}

CD2DSolidColorBrush * LotteryScene::GetColorBrush(CHwndRenderTarget * target, D2D1::ColorF::Enum color)
{
	if (_brushCache.find(color) == _brushCache.end())
	{
		_brushCache[color] = new CD2DSolidColorBrush(target, ColorF(color));
		HR(_brushCache[color]->Create(target));
	}
	return _brushCache[color];
}

