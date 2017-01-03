#pragma once

class Scene;

class DemoWindow : public CFrameWnd
{
public:
	DemoWindow();

private:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnSize(UINT, int, int);
	afx_msg LRESULT OnDraw2D(WPARAM, LPARAM);
	afx_msg void OnIdSelected(UINT id);

	// custom methodss
	void CreateScene(int id);

	std::vector<std::unique_ptr<Scene>> _scenes;
	CMenu _menu;
};