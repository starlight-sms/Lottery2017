#pragma once

#include "DxRes.h"
#include "LotteryScene.h"

class FlashImageScene : public LotteryScene
{
public:
	FlashImageScene(int count, int itemId, std::vector<int> personIds);
	// overrides
	virtual void Update() override;
	virtual void Render(CHwndRenderTarget* target, DxRes* dxRes) override;
	virtual void KeyUp(UINT key) override;
private:
	bool _started = false;
};