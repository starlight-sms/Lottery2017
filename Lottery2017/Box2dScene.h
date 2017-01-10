#pragma once
#include "FlashImageScene.h"

struct Person;

class Box2dScene final : public LotteryScene
{
public:
	Box2dScene(int count, int itemId, const std::vector<int>& personIds);
	virtual void Update() override;
	virtual void Render(CHwndRenderTarget * target, DxRes* dxRes) override;
	virtual void KeyUp(UINT key) override;

private:
	void ShowWinner(CHwndRenderTarget* target, DxRes* dxRes);
	const float RelativeSize = 10.0f;
	const float BorderWidth = RelativeSize / 100;

	enum class State
	{
		Pending,
		Started,
		Triggled,
		Completed,
	};

	State _state;

	// mist
	void PreRenderBody(CHwndRenderTarget* target, b2Body * body, float scale);

	// b2 related
	std::vector<b2Body*> _borders;
	std::vector<b2Body*> _personBodies;
	b2World _world;
	int _updateCount = 0;

	b2Body* CreateBorderBody(float x, float y, float angle, float length);
	b2Body* CreatePersonBody(int personId);
	void FindLuckyPersons();
	void CompleteAndSave();
	void EnterTriggerMode();

	// dxres
	D2D1::ColorF::Enum _borderColor = D2D1::ColorF::LightGray, _luckyColor = D2D1::ColorF::Red;
};