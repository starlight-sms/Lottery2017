#pragma once

#include "DxRes.h"
#include "LotteryScene.h"

class LuckyScene : public Scene
{
public:
	LuckyScene(int itemId, std::vector<int> personIds);
	// overrides
	virtual void Update() override;
	virtual void Render(CHwndRenderTarget* target, DxRes* dxRes) override;
	virtual void KeyUp(UINT key) override;
private:
	void RenderNoLuckyPerson(CHwndRenderTarget * target, DxRes* dxRes);
	std::vector<int> _luckyPersonIds;
	int _itemId;
};