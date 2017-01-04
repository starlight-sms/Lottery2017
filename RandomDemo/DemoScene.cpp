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

RandomDemoScene::RandomDemoScene() :
	_ui{ 0, SampleCount - 1 }
{
}

void RandomDemoScene::CreateDeviceResources(CHwndRenderTarget *target)
{
	_black = new CD2DSolidColorBrush(target, ColorF(ColorF::Black));
	_white = new CD2DSolidColorBrush(target, ColorF(ColorF::White));
}

void RandomDemoScene::CreateDeviceSizeResources(CHwndRenderTarget *)
{
}

void RandomDemoScene::Render(CHwndRenderTarget * target)
{
	auto size = target->GetSize();
	auto unitH = size.height * 0.8f / (SampleTotal / SampleCount);
	auto baseH = size.height * 0.1f;
	auto baseW = size.width / SampleCount;

	double sum = std::accumulate(begin(_result), end(_result), 0.0);
	double mean = sum / _result.size();
	double sq_sum = inner_product(begin(_result), end(_result), begin(_result), 0.0);

	double stdev = sqrt(sq_sum / _result.size() - mean * mean);
	DrawStd(target, stdev, 10.0f, 0.0f);
	_minStd = min(stdev, _minStd);
	DrawStd(target, _minStd, 10.0f, 10.0f);
	_maxStd = max(stdev, _maxStd);
	DrawStd(target, _maxStd, 10.0f, 20.0f);
	auto stdavg = (_sumStd += stdev) / _iterateCount;
	DrawStd(target, stdavg, 10.0f, 30.0f);


	DrawStd(target, _iterateCount/100.0, 10.0f, 50.0f);

	for (int i = 0; i < SampleCount; ++i)
	{
		auto count = _result[i];
		CD2DRectF rect{
			i * baseW + 1, baseH,
			(i + 1)* baseW - 1, baseH + count * unitH
		};
		target->FillRectangle(rect, _black);
	}
}

void RandomDemoScene::Update()
{
	_result.clear();
	_result.assign(SampleCount, 0);
	for (auto i = 0; i < SampleTotal; ++i)
	{
		_result[GetNextRand()]++;
	}
	_iterateCount++;
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
	case ID_TEST_MT19937_64:
		return make_unique<Mt1993764DemoScene>();
	}
	throw make_exception_ptr("unknown type");
}

void RandomDemoScene::DrawStd(CHwndRenderTarget* target, double std, float height, float y)
{
	auto size = target->GetSize();
	CD2DRectF stdevRect{ 0.f, y, float(std / 150 * size.width), y + height };
	target->FillRectangle(stdevRect, _black);
}

int RandDemoScene::GetNextRand()
{
	return rand() % SampleCount;
}

int RdDemoScene::GetNextRand()
{
	return _ui(_rd);
}

int Mt19937DemoScene::GetNextRand()
{
	return _ui(_mt);
}

int RanluxDemoScene::GetNextRand()
{
	return _ui(_rd);
}

int MinstdRandDemoScene::GetNextRand()
{
	return _ui(_rd);
}

int CSDemoScene::GetNextRand()
{
	return Test::r->Next() % SampleCount;
}

int Mt1993764DemoScene::GetNextRand()
{
	return _ui(_mt);
}
