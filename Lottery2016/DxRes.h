#pragma once

class DxRes
{
	virtual void CreateDeviceResources(CHwndRenderTarget *target) = 0;
	virtual void CreateDeviceSizeResources(CHwndRenderTarget *target) = 0;
};