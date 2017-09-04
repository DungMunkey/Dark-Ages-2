#ifndef _DA2TEXT_H
#define _DA2TEXT_H

#include "DA2DX.h"
#include "DA2Gfx.h"

class cText{
public:

	cText();
	~cText();

	bool Init(cDA2Gfx *gfx);
	//void DrawText(int x, int y, int wrap, char *txt);
	int drawText(SDL_Renderer *surf, int x, int y, int wrap, char *txt, int color=0);

private:
	cDA2Gfx *ddGfx;

};


#endif