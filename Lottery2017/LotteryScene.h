#pragma once

#include "DxRes.h"

class Scene
{
public:
	virtual void Update() = 0;
	virtual void Render(CHwndRenderTarget* target, DxRes* res) = 0;
	virtual void KeyUp(UINT key) = 0;
	virtual ~Scene() {};

protected:
	CD2DSizeF Divide(CD2DSizeF v, float x, float y);
	CD2DSizeF Multiple(CD2DSizeF v, float x, float y);
	CD2DSizeF GetDisplaySize(const CD2DSizeF& inputSize, const CD2DSizeF& scaleSize);
	CD2DRectF GetDrawCenterRect(const CD2DSizeF& topLeft, const CD2DSizeF& gridSize, const CD2DSizeF& realSize);
};

class LotteryScene: public Scene
{
public:
	LotteryScene(int count, int itemId, std::vector<int> personIds);
protected:
	const void Save();
	
	const int _itemId;
	const int _requiredCount;
	std::vector<int> _allPersonIds;
	std::unordered_set<int> _luckyPersonIds;
	std::mt19937 _rd;
};