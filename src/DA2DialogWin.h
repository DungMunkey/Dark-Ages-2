#ifndef _DA2DIALOGWIN_H
#define _DA2DIALOGWIN_H

#include "DA2BaseWindow.h"
#include "DA2Text.h"

class cDialogWin:public cBaseWindow{
public:
	bool GetButton();
	bool SetWindow(int x, int y, char *txt, bool but=true);
	virtual bool Logic();
	virtual bool Render(bool mouse=true);

protected:
	bool bButtons;
	bool bAccept;
	SDL_Rect buttons[2];
	//cText text;
	char str[512];

};

#endif