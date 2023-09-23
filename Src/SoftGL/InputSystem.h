#ifndef InputSystem_h__
#define InputSystem_h__

#include <OIS.h>
#include <sstream>
#include "RenderWindow.h"
#include "Delegate.h"

#ifdef _DEBUG
#pragma comment (lib, "OIS_static_d.lib")
#else
#pragma comment (lib, "OIS_static.lib")
#endif

using namespace OIS;

class InputSystem : public OIS::KeyListener, public OIS::MouseListener
{
public: 
	EVENT1(KBEvent, int);
	KBEvent Event_KeyDown;
	KBEvent Event_KeyUp;

	EVENT4(MSEvent, int, int, int, int);
	MSEvent Event_MouseMove;

	InputSystem(RenderWindow* window, int sw, int sh)
	{
		//init Input
		ParamList pl;

		HWND hw = (HWND)window->GetWindowHandle();
		std::ostringstream wnd;
		wnd << (size_t)hw; 
		pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

		bool hideCursor = false;

		if( !hideCursor)
		{
			pl.insert(std::make_pair(std::string("w32_mouse") , std::string("DISCL_BACKGROUND" )));

			pl.insert(std::make_pair(std::string("w32_mouse")  , std::string("DISCL_NONEXCLUSIVE")));
		}
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_BACKGROUND")));

		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string ("DISCL_NONEXCLUSIVE") ));




		input = InputManager::createInputSystem(pl);
		keyboard = (Keyboard*)input->createInputObject(OISKeyboard,1);
		mouse = (Mouse*)input->createInputObject(OISMouse,1);
		mouse->getMouseState().width = sw;
		mouse->getMouseState().height = sh;

		keyboard->setEventCallback(this);
		mouse->setEventCallback(this);
	}
	~InputSystem()
	{
		input->destroyInputObject(mouse);
		input->destroyInputObject(keyboard);
		InputManager::destroyInputSystem(input);
	}
	void Capture()
	{
		mouse->capture();
		keyboard->capture();
	}

	bool keyPressed(const KeyEvent &arg)
	{
		Event_KeyDown.Invoke((int)arg.key);
		return true;
	}
	bool keyReleased(const KeyEvent &arg)
	{
		Event_KeyUp.Invoke((int)arg.key);
		return true;
	}

	bool mouseMoved( const MouseEvent &arg )
	{
		Event_MouseMove.Invoke(arg.state.X.abs, arg.state.Y.abs, arg.state.X.rel, arg.state.Y.rel);
		return true;
	}
	bool mousePressed( const MouseEvent &arg, MouseButtonID id )
	{
		return true;
	}
	bool mouseReleased( const MouseEvent &arg, MouseButtonID id )
	{
		
		return true;
	}
	void SetKeyDownCallback()
	{

	}

private:
	InputManager* input;
	Keyboard* keyboard;
	Mouse* mouse;

};
#endif // InputSystem_h__
