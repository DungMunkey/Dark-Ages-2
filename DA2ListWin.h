#ifndef _DA2LISTWIN_H
#define _DA2LISTWIN_H

#include "DA2BaseWindow.h"
#include "DA2Text.h"
#include "DA2Box.h"

class cListWin:public cBaseWindow{
public:
	bool AddItem(char *txt);
	int GetSelection();
	bool SetWindow(int x, int y, bool six=true);
	bool Logic();
	bool Render();

private:
	bool bSix;
	int selection;
	int size;
	SDL_Rect listThree[3];
	SDL_Rect listSix[6];
	//cText text;
	cBox box;
	char list[6][128];

};

#endif