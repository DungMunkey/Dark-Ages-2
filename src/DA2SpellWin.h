#ifndef _DA2SPELLWIN_H
#define _DA2SPELLWIN_H

#include "DA2BaseWindow.h"
#include "DA2Box.h"
#include "DA2Text.h"
#include "DA2Party.h"

class cSpellWin:public cBaseWindow{
public:
	cSpellWin();
	~cSpellWin();

	bool GetButton();
	bool SetWindow(int x, int y, cDA2Party *par);
	bool Logic();
	bool Render(bool mouse=true);
	void Update();

protected:
	SDL_Rect popUpList[6];
	SDL_Rect targets[3];
	SDL_Rect buttons[2];
	//cText text;
	cBox box;
	cDA2Party *party;
	bool spellsKnown[6];
	int activeSpell;
	bool isSelecting;


};

#endif