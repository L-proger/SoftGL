#pragma once
#ifdef WIN32
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "IRenderWindow.h"

class RenderWindow : public IRenderWindow {
public:
	/*virtual ~RenderWindow(void) {}
	virtual void Update() = 0;
	virtual void SetCaption(const String& caption) = 0;
	virtual int GetWindowHandle() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual const String& GetCaption() = 0;*/

	bool UpdateCamera;

	RenderWindow();
	~RenderWindow(void);
	void Update();
	void SetCaption(const String& caption);
	int GetWindowHandle() { return (int)g_hwnd; }
	void SetSize(int w, int h);
	bool IsFullScreen();
	void SetFullScreen(bool state);
	bool SwitchMode(bool fullscreen);
	void SetFullscreenMode(DisplayMode mode);


	bool IsActive();
	bool IsModeSwitching();
	void SetCursorVisible(bool state);
	bool IsCursorVisible();
	void CenterWindow();

	int GetWidth() {
		return width;
	}
	virtual int GetHeight() {
		return height;
	}
	const String& GetCaption() {
		return caption;
	}
	void SetAltEnterLocked(bool state) {
		bAltEnterLocked = state;
	}
	bool IsAltEnterLocked() {
		return bAltEnterLocked;
	}
	virtual void event_activate() {

	}
	virtual void event_deactivate() {

	}
	virtual void event_resize() {

	}
	virtual void event_switch_fullscreen() {

	}
	virtual void Present(Texture2D* tex) override {
		if (tex->bpp == 4) {

			SetDIBitsToDevice(
				context,
				0,
				0,
				tex->width,
				tex->height,
				0,
				0,
				0,
				tex->height,
				tex->getBuffer()->getDataPtr(),
				&info,
				0
			);
		}
	}
protected:
	bool bAltEnterLocked;
	HINSTANCE g_hInst;
	HWND g_hwnd;
	DEVMODE mode;
private:
	void OnActivate();
	void OnDeactivate();
	void OnResize();
	static LRESULT __stdcall WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	virtual int GetTop() override {
		RECT rect;
		GetWindowRect(g_hwnd, &rect);
		return rect.top;
	}

	virtual int GetLeft() override {
		RECT rect;
		GetWindowRect(g_hwnd, &rect);
		return rect.left;
	}

	virtual void SetPosition(int top, int left) override {
		RECT rect;
		GetWindowRect(g_hwnd, &rect);
		SetWindowPos(g_hwnd, NULL, left, top, GetWidth(), GetHeight(), 0);
	}

	BITMAPINFOHEADER bitmapInfo;
	BITMAPINFO info;
	HDC context;

	
};

#endif
