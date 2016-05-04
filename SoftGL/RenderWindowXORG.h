#ifndef RENDERWINDOWXORG_H
#define RENDERWINDOWXORG_H

#include "IRenderWindow.h"
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<stdio.h>

class RenderWindowXORG : public IRenderWindow
{
public:
    RenderWindowXORG(uint16 width, uint16 height);
    void Update();
    void SetCaption(const String& caption);
    int GetWindowHandle();
    int GetWidth();
    int GetHeight();
    void SetSize(int w, int h);
    const String& GetCaption();

    int GetTop();
    int GetLeft();
    void SetPosition(int top, int left);

    bool IsFullScreen();
    void SetFullScreen(bool state);
    bool SwitchMode(bool fullscreen);
    void SetFullscreenMode(DisplayMode mode);

    void Present(Texture2D* tex);

    void OnActivate();
    void OnDeactivate();
    void OnResize();
    bool IsActive();
    bool IsModeSwitching();
    void SetCursorVisible(bool state);
    bool IsCursorVisible();
    void CenterWindow();
private:
    uint8_t* image32;
    Visual *visual;
    GC gc;
    Display *display_name;
    XImage* img;
    int depth,screen,connection;
    Window window, rootwindow;
};

#endif // RENDERWINDOWXORG_H
