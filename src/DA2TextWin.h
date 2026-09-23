#ifndef _DA2TEXTWIN_H
#define _DA2TEXTWIN_H

#include "DA2BaseWindow.h"
#include "DA2Text.h"

class cTextWin:public cBaseWindow{
public:
	bool SetWindow(int x, int y, char *txt);
	bool Logic();
	bool Render();

private:
	//cText text;
	char str[512];

};

#endif