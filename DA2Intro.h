#ifndef _DA2INTRO_H
#define _DA2INTRO_H

#include "DA2Global.h"
#include "DA2Text.h"
#include "DA2BaseWindow.h"
#include "DA2Party.h"

class cDA2Intro:public cBaseWindow {
public:
	cDA2Intro();
	~cDA2Intro();

	bool GetButton();
	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cDA2Party *p, int *anim);
	bool Logic();
	bool Render();
	void SetDefault();
	void Update();

protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cDA2Party *party;
	int *animCounter;

	SDL_Rect targets[3];
	SDL_Rect button;

	int mode;
	int Cursor;
	int currentPlayer;

  char DIKtoASCII(DA2KEYS dik);


};

#endif