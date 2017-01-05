#include "pch.h"
#include "DxRes.h"
#include "Items.h"
#include "Person.h"
using D2D1::ColorF;
using D2D1::Matrix3x2F;

#define new DEBUG_NEW

void DxRes::CreateDeviceResources(CHwndRenderTarget * target)
{
	LotteryBitmaps.clear();
	for (size_t i = 0; i < GetItems().size(); ++i)
	{
		LotteryBitmaps.push_back(new CD2DBitmap(target, GetItems()[i].ResourceId, L"Item"));
		HR((*LotteryBitmaps.rbegin())->Create(target));
	}

	for (size_t i = 0; i < GetAllPerson().size(); ++i)
	{
		PersonBrushes[i] = new CD2DBitmapBrush(target, GetAllPerson()[i].ResourceId, L"Person");
		HR(PersonBrushes[i]->Create(target));
		PersonBitmaps[i] = new CD2DBitmap(target, GetAllPerson()[i].ResourceId, L"Person");
		HR(PersonBitmaps[i]->Create(target));
	}
}

void DxRes::CreateDeviceSizeResources(CHwndRenderTarget * target)
{
	if (HeaderTextFormat)
	{
		HeaderTextFormat->Destroy();
		TextFormat->Destroy();
		delete HeaderTextFormat;
		delete TextFormat;
	}		

	auto size = target->GetSize();
	auto minEdge = std::min(size.width, size.height);
	auto real = b2Clamp(minEdge / 40, 12.0f, 24.0f);
	HeaderTextFormat = new CD2DTextFormat(target, L"Consolas", real);
	TextFormat = new CD2DTextFormat(target, L"Consolas", real * 0.8f);

	for (auto& kv : PersonBrushes)
	{
		auto bitmapSize = kv.second->GetBitmap()->GetPixelSize();
		auto minEdge = (float)std::min(bitmapSize.width, bitmapSize.height);
		auto transform =
			Matrix3x2F::Translation(
				-float(bitmapSize.width - minEdge) / 2,
				-float(bitmapSize.height - minEdge) / 2) *
			Matrix3x2F::Translation(-minEdge / 2, -minEdge / 2) *
			Matrix3x2F::Scale(PersonSize / minEdge, PersonSize / minEdge);
		kv.second->SetTransform(&transform);
	}
}

void DxRes::ClearDeviceResources()
{
	_brushCache.clear();
}

CD2DSolidColorBrush * DxRes::GetColorBrush(CHwndRenderTarget *target, D2D1::ColorF::Enum color)
{
	if (_brushCache.find(color) == _brushCache.end())
	{
		_brushCache[color] = new CD2DSolidColorBrush(target, ColorF(color));
		HR(_brushCache[color]->Create(target));
	}
	return _brushCache[color];
}

RectangleGeometry * DxRes::GetOrCreateBorderGeometry(CHwndRenderTarget * target, float w, float h)
{
	if (BorderGeometries.find(w) == BorderGeometries.end())
	{
		BorderGeometries[w] = new RectangleGeometry(target,
		{
			-w / 2.0f,
			-h / 2.0f,
			w / 2.0f,
			h / 2.0f
		});
		HR(BorderGeometries[w]->Create(target));
	}
	return BorderGeometries[w];
}
