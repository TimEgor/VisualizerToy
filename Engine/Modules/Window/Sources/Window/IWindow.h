#pragma once

class IWindow
{
public:
	IWindow() = default;
	virtual ~IWindow() {}

	virtual bool init() = 0;
	virtual void release() = 0;

	virtual void show() = 0;
	virtual void hide() = 0;
	virtual bool isShown() const = 0;
};