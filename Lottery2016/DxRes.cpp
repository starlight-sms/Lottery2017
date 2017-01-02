#include "pch.h"
#include "DxRes.h"
#include "Items.h"
#include "Person.h"
#include <Box2D\Box2D.h>

using D2D1::ColorF;

void DxRes::CreateDeviceResources(CHwndRenderTarget *target)
{
	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		LotteryBitmaps.push_back(new CD2DBitmap(target, GetItems()[i].ResourceId, L"Item"));
		HR((*LotteryBitmaps.rbegin())->Create(target));
	}
	for (size_t i = 0; i < GetAllPerson().size(); ++i)
	{
		PersonBitmaps.push_back(new CD2DBitmap(target, GetAllPerson()[i].ResourceId, L"Person"));
		HR((*PersonBitmaps.rbegin())->Create(target));
	}

	Blue = new CD2DSolidColorBrush(target, ColorF(ColorF::Blue));
}

void DxRes::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	if (TextFormat)
	{
		TextFormat->Destroy();
	}

	auto size = target->GetSize();
	auto minEdge = std::min(size.width, size.height);
	auto real = b2Clamp(minEdge / 40, 12.0f, 24.0f);
	TextFormat = new CD2DTextFormat(target, L"Consolas", real);
}
