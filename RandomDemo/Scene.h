#pragma once

class Scene
{
public:
	virtual ~Scene() {};
	virtual void CreateDeviceResources(CHwndRenderTarget*) = 0;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget*) = 0;
	virtual void Render(CHwndRenderTarget*) = 0;
	virtual void Update() = 0;
};
