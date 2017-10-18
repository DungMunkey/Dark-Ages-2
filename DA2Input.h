#ifndef _DA2INPUT_H
#define _DA2INPUT_H

#include <SDL.h>
#include <stdio.h>

enum DA2KEYS{
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_SPACE,
  KEY_BACKSPACE,
  KEY_DELETE,
  KEY_ESCAPE,
  KEY_TAB,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_MINUS,
  KEY_ADD,
  KEY_SUBTRACT,
  KEY_LSHIFT,
  KEY_RSHIFT,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9
};


class cDA2Input{
public:
  cDA2Input();
	~cDA2Input();

  bool getButtonState(int k);
  bool getKeyState(int k);
  int  mouseAction();
  bool isPressed(int k);
  bool isReleased(int k);
  void pollEvents();
  void setMouseOffsets(float mx, int ox, float my, int oy);

	bool keyLocked[256];
	bool mouseLocked[2];
	int mousePosX;
	int mousePosY;
  int mouseX;
  int mouseY;
  float multPosX;
  float multPosY;
  int offsetPosX;
  int offsetPosY;

  bool CheckMousePos(SDL_Rect& r); //return true if in this box
  bool CheckLock(DA2KEYS key);
  void LockKey(DA2KEYS key);
  bool KeyPress(DA2KEYS key);
	bool CheckLockMouse(int button);
	void LockMouse(int button);
	bool MousePress(int button);
	int MouseX();
	int MouseY();
	//bool Read();

private:
  SDL_Event e;
  bool keyState[128];
  bool buttonState[3];
  int  lastButton;

  void setButton(Uint8 k, bool b);
  void setKey(SDL_Keycode k, bool b);

};


#endif 