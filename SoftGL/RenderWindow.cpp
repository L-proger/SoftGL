#include "RenderWindow.h"
#include <winuser.h>

#ifdef WIN32
#define L_WIN32
#include <winuser.h>


RenderWindow::RenderWindow()
	:UpdateCamera(false) {
	bFullscreen = false;
	bActive = true;
	bModeSwitching = false;
	bCursorVisible = true;
	bAltEnterLocked = false;

	width = 256;
	height = 256;

	g_hInst = GetModuleHandle(NULL);
	g_hwnd = NULL;

	//memset(&g_wndClass,0,sizeof(WNDCLASSEX));

	WNDCLASSEX g_wndClass = { NULL };

	g_wndClass.cbSize = sizeof(WNDCLASSEX);
	g_wndClass.cbClsExtra = NULL;
	g_wndClass.cbWndExtra = NULL;
	g_wndClass.hbrBackground = (HBRUSH)GetStockObject(1);
	g_wndClass.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	g_wndClass.hIcon = LoadIcon(g_hInst, IDI_APPLICATION);
	g_wndClass.hIconSm = LoadIcon(g_hInst, IDI_APPLICATION);
	g_wndClass.hInstance = g_hInst;
	g_wndClass.lpfnWndProc = &RenderWindow::WndProc;
	g_wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	g_wndClass.lpszMenuName = NULL;
	g_wndClass.lpszClassName = _text("Onotole");

	if (!RegisterClassEx(&g_wndClass)) {
		MessageBox(g_hwnd, _text("Can not create window class! We all gonna die!!"), _text("OH SHIT!"), MB_OK);
	}


	//calculate window size
	RECT NewWindowSize;
	NewWindowSize.top = 30;
	NewWindowSize.left = 300;
	NewWindowSize.bottom = NewWindowSize.top + height;
	NewWindowSize.right = NewWindowSize.left + width;

	AdjustWindowRect(&NewWindowSize, WS_OVERLAPPEDWINDOW, false);

	/*WS_MINIMIZEBOX
	WS_MAXIMIZEBOX*/


	if (!(g_hwnd = CreateWindowEx(NULL, _text("Onotole"), _text("LifeEngine render window"), WS_OVERLAPPEDWINDOW,
		NewWindowSize.left, NewWindowSize.top, NewWindowSize.right - NewWindowSize.left, NewWindowSize.bottom - NewWindowSize.top, NULL, NULL, g_hInst, NULL))) {
		MessageBox(g_hwnd, _text("Can not create window! We all gonna die!!"), _text("HOLY SHIT!"), MB_OK);
		return;
	}

	ShowWindow(g_hwnd, SW_SHOWNORMAL);
	SetForegroundWindow(g_hwnd);
	SetFocus(g_hwnd);


	ZeroMemory(&mode, sizeof(DEVMODE));
	mode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);

	RenderWindow* ptr = this;
	long val = (long)ptr;
	SetWindowLongPtr(g_hwnd, GWL_USERDATA, val);

	fsMode.width = 640;
	fsMode.height = 480;
	fsMode.refreshRate = 60;
	fsMode.bpp = 4;

	::ZeroMemory(&bitmapInfo, sizeof(BITMAPINFOHEADER));
	bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.biWidth = width;
	bitmapInfo.biHeight = -height;
	bitmapInfo.biPlanes = 1;
	bitmapInfo.biBitCount = 32;
	bitmapInfo.biCompression = 0;
	bitmapInfo.biSizeImage = width * height * 4;

	info.bmiHeader = bitmapInfo;

	context = ::GetDC(g_hwnd);

	CenterWindow();
}
void RenderWindow::Update() {
#ifdef L_WIN32
	MSG msg = { 0 };
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		switch (msg.message) {
		case WM_QUIT:
			event_activate();
			exit(0);

			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
}
void RenderWindow::SetCaption(const String& val) {
	caption = val;
	SetWindowText(g_hwnd, caption.c_str());
}
void RenderWindow::SetFullScreen(bool state) {
	bFullscreen = state;
}
bool RenderWindow::IsFullScreen() {
	return bFullscreen;
}
RenderWindow::~RenderWindow(void) {
	ChangeDisplaySettings(&mode, 0);
}


void RenderWindow::OnActivate() {
	bActive = true;
	event_activate();
}
void RenderWindow::OnDeactivate() {
	bActive = false;
	event_deactivate();
}
void RenderWindow::OnResize() {
	if (!IsFullScreen() || !bModeSwitching) {
		RECT r;
		::GetClientRect(g_hwnd, &r);
		width = r.right - r.left;
		height = r.bottom - r.top;

		::ZeroMemory(&bitmapInfo, sizeof(BITMAPINFOHEADER));
		bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.biWidth = width;
		bitmapInfo.biHeight = -height;
		bitmapInfo.biPlanes = 1;
		bitmapInfo.biBitCount = 32;
		bitmapInfo.biCompression = 0;
		bitmapInfo.biSizeImage = width * height * 4;

		info.bmiHeader = bitmapInfo;
	}
	event_resize();
}
bool RenderWindow::IsActive() {
	return bActive;
}
bool RenderWindow::IsModeSwitching() {
	return bModeSwitching;
}
void RenderWindow::SetCursorVisible(bool state) {
	bCursorVisible = state;
}
bool RenderWindow::IsCursorVisible() {
	return bCursorVisible;
}


bool RenderWindow::SwitchMode(bool fullscreen) {
	bFullscreen = fullscreen;

	if (bFullscreen) {
		bModeSwitching = true;
		DEVMODE fmode;
		ZeroMemory(&fmode, sizeof(DEVMODE));
		fmode.dmSize = sizeof(DEVMODE);
		fmode.dmPelsWidth = fsMode.width;
		fmode.dmPelsHeight = fsMode.height;
		fmode.dmBitsPerPel = mode.dmBitsPerPel;
		fmode.dmDisplayFrequency = fsMode.refreshRate;
		fmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

		//if(ChangeDisplaySettings(&fmode,0)!= DISP_CHANGE_SUCCESSFUL)
		//	return false;

		SetWindowLong(g_hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		SetWindowLong(g_hwnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, fsMode.width, fsMode.height, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		bModeSwitching = false;
	} else {
		//if(ChangeDisplaySettings(&mode_window,0)!= DISP_CHANGE_SUCCESSFUL)
		//	return false;
		SetWindowLong(g_hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
		SetWindowLong(g_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetSize(width, height);
		CenterWindow();
	}
	event_switch_fullscreen();
	return true;
}
void RenderWindow::CenterWindow() {
	int mx = GetSystemMetrics(SM_CXSCREEN);
	int my = GetSystemMetrics(SM_CYSCREEN);

	int nx = (mx - width) / 2;
	int ny = (my - height) / 2;

	SetWindowPos(g_hwnd, NULL, nx, ny, -1, -1,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
void RenderWindow::SetFullscreenMode(DisplayMode _mode) {
	fsMode = _mode;
}
void RenderWindow::SetSize(int w, int h) {
	if (!IsFullScreen()) {
		width = w;
		height = h;
		RECT NewWindowSize;
		NewWindowSize.top = 50;
		NewWindowSize.left = 50;
		NewWindowSize.bottom = NewWindowSize.top + h;
		NewWindowSize.right = NewWindowSize.left + w;

		AdjustWindowRect(&NewWindowSize, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(g_hwnd, HWND_TOP, NewWindowSize.left, NewWindowSize.top, NewWindowSize.right - NewWindowSize.left, NewWindowSize.bottom - NewWindowSize.top, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		//::MoveWindow(g_hwnd,NewWindowSize.left, NewWindowSize.top, NewWindowSize.right - NewWindowSize.left, NewWindowSize.bottom - NewWindowSize.top, true);
		CenterWindow();
	}
}
LRESULT __stdcall RenderWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
	case WM_SIZE:
	{
		//не ресайзим девайс если окно свёрнуто
		if (wparam != SIZE_MINIMIZED) {
			long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
			if (val) {
				RenderWindow* wnd = (RenderWindow*)val;
				wnd->OnResize();
			}
		}
	}
	break;
	case WM_SETCURSOR:
	{
		static bool hiddencursor = false;
		WORD ht = LOWORD(lparam);

		long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
		if (val) {
			RenderWindow* wnd = (RenderWindow*)val;
			if (wnd->IsCursorVisible()) {
				ShowCursor(true);
			} else {
				if (HTCLIENT == ht && !hiddencursor) {
					hiddencursor = true;
					ShowCursor(false);
				} else if (HTCLIENT != ht && hiddencursor) {
					hiddencursor = false;
					ShowCursor(true);
				}
			}
		}



	}
	break;
	case WM_ACTIVATE: //Catch Alt + Tab
	{
		switch (wparam) {
		case WA_ACTIVE:
		{
			long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
			if (val) {
				RenderWindow* wnd = (RenderWindow*)val;
				wnd->OnActivate();
			}
		}
		break;
		case WA_INACTIVE:
		{
			long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
			if (val) {
				RenderWindow* wnd = (RenderWindow*)val;
				wnd->OnDeactivate();
			}
		}
		break;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		break;
	case WM_SYSKEYDOWN:
		switch (wparam) {
		case VK_RETURN:
		{
			if (GetKeyState(VK_MENU)) //Alt + Enter
			{
				long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
				if (val) {

					RenderWindow* wnd = (RenderWindow*)val;
					if (!wnd->IsAltEnterLocked()) {
						wnd->SetAltEnterLocked(true);
						wnd->SwitchMode(!wnd->IsFullScreen());
					}
				}
			}
		}
		break;
		}
		break;
	case WM_SYSKEYUP:
	{
		switch (wparam) {
		case VK_RETURN:
		{
			long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
			if (val) {

				RenderWindow* wnd = (RenderWindow*)val;
				wnd->SetAltEnterLocked(false);
			}
		}
		break;
		}
	}
	break;
	case WM_KEYDOWN:
		switch (wparam) {
		case VK_SPACE:
		{
			long val = GetWindowLongPtr(hwnd, GWL_USERDATA);
			if (val) {
				RenderWindow* wnd = (RenderWindow*)val;
				wnd->UpdateCamera = !wnd->UpdateCamera;
			}
		}
		break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			exit(0);
			break;
		}
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}


#endif
