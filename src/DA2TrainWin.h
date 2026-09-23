#ifndef _DA2TRAINWIN_H
#define _DA2TRAINWIN_H

#include "DA2DialogWin.h"
#include "DA2Party.h"
#include "DA2Text.h"

class cTrainWin:public cDialogWin{
public:
	cTrainWin();
	~cTrainWin();

	bool Logic();
	bool Render(bool mouse=true);
	bool SetWindow(int x, int y,int cst, int stt, cDA2Party *par);

private:
	cDA2Party *party;
	SDL_Rect sliderButtons[6];
	int train[3];
	int cost;
	int stat;
	int price;

};

#endif