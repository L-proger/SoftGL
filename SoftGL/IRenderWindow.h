#ifndef IRenderWindow_h__
#define IRenderWindow_h__


#include "LString.h"
#include "DisplayMode.h"
#include "Types.h"
#include "Texture2D.h"


class IRenderWindow{
public:
	virtual ~IRenderWindow(void) {}
	virtual void Update() = 0;
	virtual void SetCaption(const String& caption) = 0;
	virtual int GetWindowHandle() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
    virtual int GetTop() = 0;
    virtual int GetLeft() = 0;
    virtual void SetPosition(int top, int left) = 0;
	virtual void SetSize(int w, int h) = 0;
	virtual const String& GetCaption() = 0;

	virtual bool IsFullScreen() = 0;
	virtual void SetFullScreen(bool state) = 0;
	virtual bool SwitchMode(bool fullscreen) = 0;
	virtual void SetFullscreenMode(DisplayMode mode) = 0;

	virtual const DisplayMode& GetFullscreenMode()
	{
		return fsMode;
	}
    virtual void Present(Texture2D* tex) = 0;

	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;
	virtual void OnResize() = 0;
	virtual bool IsActive() = 0;
	virtual bool IsModeSwitching() = 0;
	virtual void SetCursorVisible(bool state) = 0;
	virtual bool IsCursorVisible() = 0;
	virtual void CenterWindow() = 0;
protected:
	bool bCursorVisible;
	bool bModeSwitching;
	bool bFullscreen;
	bool bActive;
	int width;
	int height;
	String caption;
	DisplayMode fsMode;
};
#endif // IRenderWindow_h__
