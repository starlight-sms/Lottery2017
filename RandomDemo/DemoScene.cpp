#include "pch.h"
#include "DemoScene.h"
#include "resource.h"

using namespace std;
using D2D1::ColorF;
using D2D1::Matrix3x2F;

ref class Test
{
public:
	static System::Random ^r = gcnew System::Random(System::Guid::NewGuid().GetHashCode());
};

void RandomDemoScene::CreateDeviceResources(CHwndRenderTarget *target)
{
	_brush = new CD2DSolidColorBrush(target, ColorF(ColorF::Black));
}

void RandomDemoScene::CreateDeviceSizeResources(CHwndRenderTarget *)
{
}

void RandomDemoScene::Render(CHwndRenderTarget * target)
{
	auto size = target->GetSize();
	const UINT MaxS = 0x10000;
	auto scaleX = size.width / MaxS;
	auto scaleY = size.height / MaxS;

	target->SetTransform(Matrix3x2F::Scale(scaleX, scaleY));
	for (auto p : _points)
	{
		auto x = UINT(p % MaxS);
		auto y = UINT(p / MaxS);
		CD2DRectF rect{ (float)x, (float)y, (float)x + 2 / scaleX, (float)y + 2 / scaleY };
		target->FillRectangle(rect, _brush);
	}
}
;
void RandomDemoScene::Update()
{
	_points.clear();
	for (auto i = 0; i < 50000; ++i)
	{
		_points.push_back(GetNextRand());
	}
}

std::unique_ptr<RandomDemoScene> RandomDemoScene::Make(int type)
{
	switch (type)
	{
	case ID_TEST_RAND:
		return make_unique<RandDemoScene>();
	case ID_TEST_RD:
		return make_unique<RdDemoScene>();
	case ID_TEST_MT19937:
		return make_unique<Mt19937DemoScene>();
	case ID_TEST_MINSTD:
		return make_unique<MinstdRandDemoScene>();
	case ID_TEST_RANLUX:
		return make_unique<RanluxDemoScene>();
	case ID_TEST_CSRANDOM:
		return make_unique<CSDemoScene>();
	}
	throw make_exception_ptr("unknown type");
}

UINT RandDemoScene::GetNextRand()
{
	return (rand() << 17) + (rand() * 2);
}

UINT RdDemoScene::GetNextRand()
{
	return _rd();
}

UINT Mt19937DemoScene::GetNextRand()
{
	return _mt();
}

UINT RanluxDemoScene::GetNextRand()
{
	return (UINT)_rd();
}

UINT MinstdRandDemoScene::GetNextRand()
{
	return _rd() * 2;
}

UINT CSDemoScene::GetNextRand()
{
	return (UINT)Test::r->Next() * 2;
}
