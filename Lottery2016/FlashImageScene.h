#pragma once

#include "DxRes.h"

class Scene : public DxRes
{
public:
	virtual void Update() = 0;
	virtual void Render(CHwndRenderTarget* target) = 0;
	virtual void KeyUp(UINT key) = 0;
};

class FlashImageScene : public Scene
{
public:
	FlashImageScene(int count, int itemId, std::vector<int> personIds);
	// overrides
	virtual void Update() override;
	virtual void CreateDeviceResources(CHwndRenderTarget * target) override;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget * target) override;
	virtual void Render(CHwndRenderTarget* target) override;
	virtual void KeyUp(UINT key) override;
private:
	const size_t _count;
	const int _itemId;

	bool _started = false;
	std::mt19937 _mt19937;
	
	std::vector<int> _allPersonIds;
	std::vector<int> _selectedPersonIds;	

	// dxres
	std::unordered_map<int, CD2DBitmap*> _personBitmaps;
	CD2DTextFormat* _textFormat = nullptr;
	CD2DSolidColorBrush* _blue;
};