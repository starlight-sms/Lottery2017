#pragma once

class DxRes
{
public:
	virtual void CreateDeviceResources(CHwndRenderTarget *target) = 0;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget *target) = 0;

	virtual ~DxRes() {};
};