#ifndef _DA2WINDOW_H
#define _DA2WINDOW_H

#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Box.h"

class cDA2Window{
public:

	cDA2Window();
	~cDA2Window();

	bool Init(CDisplay *display, cDA2Gfx *gfx);
	bool SetSize(int x, int y, int x2, int y2);
	bool DrawWindow();

protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cBox box;
	SDL_Rect frame[4];
	int xPos;
	int yPos;
	int width;
	int height;
};

#endif