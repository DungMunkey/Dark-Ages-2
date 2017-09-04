#ifndef _DA2BASEWINDOW_H
#define	_DA2BASEWINDOW_H

#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Input.h"
#include "DA2Text.h"

class cBaseWindow{
public:
	cBaseWindow();
	~cBaseWindow();

	bool Init(CDisplay* d, cDA2Gfx *gfx, cDA2Input *inp);
	void SetPos(int x, int y);
	//virtual bool Render();
	//virtual bool Logic();

protected:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cText text;

	int xPos;
	int yPos;

private:

};

#endif