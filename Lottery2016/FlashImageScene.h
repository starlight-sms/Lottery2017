#pragma once

class DxRes;

class FlashImageScene
{
public:
	FlashImageScene(int count, std::vector<int> personIds);
	void Step();
	const std::vector<int>& GetSelectedPersonIds() const;
	bool Toggle();
	void Render(CHwndRenderTarget* target, DxRes* dxRes);
private:
	bool IsCompleted();
	bool _started = false;
	std::mt19937 _mt19937;
	const size_t _count;
	std::vector<int> _allPersonIds;
	std::vector<int> _selectedPersonIds;
};