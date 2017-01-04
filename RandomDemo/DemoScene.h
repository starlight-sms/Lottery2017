#pragma once

#include "Scene.h"

const int SampleCount = 10;

class RandomDemoScene : public Scene
{
public:
	RandomDemoScene();
	// 通过 Scene 继承
	virtual void CreateDeviceResources(CHwndRenderTarget *) override;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget *) override;
	virtual void Render(CHwndRenderTarget *) override;
	virtual void Update() override;
	static std::unique_ptr<RandomDemoScene> Make(int type);

protected:
	std::uniform_int<int> _ui;
private:	
	const int SampleTotal = 50000;
	virtual int GetNextRand() = 0;
	std::vector<int> _result;

	double _minStd = 0x7fffffff, _maxStd = -0x7fffffff, _sumStd = 0;
	int _iterateCount = 0;

	void DrawStd(CHwndRenderTarget* target, double std, float height, float y);
	
	// dxres
	CD2DBrush * _black, *_white;
};

class RandDemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
};

class RdDemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
	std::random_device _rd;
};

class Mt19937DemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
	std::mt19937 _mt{ std::random_device()() };
};

class RanluxDemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
	std::ranlux24 _rd{ std::random_device()() };
};

class MinstdRandDemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
	std::minstd_rand _rd{ std::random_device()() };
};

class CSDemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
};

class Mt1993764DemoScene : public RandomDemoScene
{
	// 通过 DemoScene 继承
	virtual int GetNextRand() override;
	std::mt19937_64 _mt{ std::random_device()() };
};