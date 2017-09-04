#ifndef _DA2OPTIONS_H
#define _DA2OPTIONS_H

#include "DA2DX.h"
#include "DA2Text.h"
#include "DA2BaseWindow.h"
#include <iostream>

using namespace std;

typedef struct OptionsStruct {
	bool Border;
	bool Diagnostics;
  bool Fullscreen;
	int GameSpeed;
	int MusicVolume;
  size_t ScreenRes;
} OptionsStruct;


class cDA2Options:public cBaseWindow {
public:
	cDA2Options();
	~cDA2Options();

	bool GetButton();
	bool Logic();
	bool Render();
	void Update();

	OptionsStruct Options;

protected:
	SDL_Rect targets[3];
	SDL_Rect buttons[14];

  size_t tmpScreen;
  bool tmpFullscreen;

};

#endif