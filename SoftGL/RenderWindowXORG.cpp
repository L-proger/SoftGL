#include "RenderWindowXORG.h"
#include <string.h>
#include <stdio.h>


RenderWindowXORG::RenderWindowXORG(uint16 width, uint16 height)
    :img(0)
{
    display_name = XOpenDisplay(NULL);
    screen = DefaultScreen(display_name);
    depth = DefaultDepth(display_name,screen);
    connection = ConnectionNumber(display_name);

    visual = DefaultVisual(display_name, 0);


    if(depth == 1)
            printf("You live in prehistoric times\n");
    else
            printf("You've got a coloured monitor with depth of %d\n",depth);

    rootwindow = RootWindow(display_name,screen);

    window = XCreateSimpleWindow(display_name, rootwindow,
                    0, 0, width, height, 1, 0, 0);

    gc  = XCreateGC(display_name, window, 0, NULL);

    XMapWindow(display_name, window);
    XFlush(display_name);
    XSynchronize(display_name, 1);
}

int RenderWindowXORG::GetTop()
{
    XWindowAttributes xwa;
    XGetWindowAttributes(display_name, window, &xwa);
    return xwa.y;
}

int RenderWindowXORG::GetLeft()
{
    XWindowAttributes xwa;
    XFlush(display_name);
    XGetWindowAttributes(display_name, window, &xwa);
    return xwa.x;
}

void RenderWindowXORG::SetPosition(int top, int left)
{
    XMoveWindow(display_name, window, top, left);
    XSync(display_name, 1);
    XFlush(display_name);
    XSynchronize(display_name, 1);
}


void RenderWindowXORG::Update()
{

}

void RenderWindowXORG::SetCaption(const String& caption)
{
    XStoreName(display_name, window, caption.c_str());
    XFlush(display_name);
}

int RenderWindowXORG::GetWindowHandle()
{
    return -1;
}

int RenderWindowXORG::GetWidth()
{
    XWindowAttributes xwa;
    XGetWindowAttributes(display_name, window, &xwa);
    return xwa.width;
}

int RenderWindowXORG::GetHeight()
{
    XWindowAttributes xwa;
    XGetWindowAttributes(display_name, window, &xwa);
    return xwa.height;
}

void RenderWindowXORG::SetSize(int w, int h)
{
    XResizeWindow(display_name, window, w, h);
    XFlush(display_name);
    XSync(display_name, 1);
}

const String& RenderWindowXORG::GetCaption()
{
    return String("Not yet implemented");
}

bool RenderWindowXORG::IsFullScreen()
{
    return false;
}

void RenderWindowXORG::SetFullScreen(bool state)
{

}

bool RenderWindowXORG::SwitchMode(bool fullscreen)
{

}

void RenderWindowXORG::SetFullscreenMode(DisplayMode mode)
{

}

void RenderWindowXORG::Present(Texture2D* tex)
{

   // char buf[256];




     if(img == 0)
     {
        // SetCaption("Create image!");
         img = XCreateImage(display_name, visual, 24, ZPixmap, 0, (char *)tex->getBuffer()->getDataPtr(), tex->width, tex->height, 32, 0);
     }


     XPutImage(display_name, window, gc, img, 0, 0, 0, 0,  tex->width, tex->height);
     XFlushGC(display_name, gc);
     XFlush(display_name);
     XSync(display_name, true);



    //  sprintf(buf, "Data ptr = 0x%x", img->data);

    //  SetCaption(String(buf));

    // img->data = NULL;
   //  XDestroyImage(img);


}

void RenderWindowXORG::OnActivate()
{

}

void RenderWindowXORG::OnDeactivate()
{

}

void RenderWindowXORG::OnResize()
{

}

bool RenderWindowXORG::IsActive()
{
    return true;
}

bool RenderWindowXORG::IsModeSwitching()
{
    return false;
}

void RenderWindowXORG::SetCursorVisible(bool state)
{

}

bool RenderWindowXORG::IsCursorVisible()
{
    return true;
}

void RenderWindowXORG::CenterWindow()
{
    int dw = DisplayWidth(display_name,screen);
    int dh = DisplayHeight(display_name,screen);

    int ww = GetWidth();
    int wh = GetHeight();
    int px = (dw - ww) / 2;
    int py = (dh - wh) / 2;

    SetPosition(px, py);
    XFlush(display_name);
}
