#ifndef _DA2BOX_H
#define _DA2BOX_H

#include "DA2DX.h"
#include "DA2Gfx.h"

class cBox{
public:

	cBox();
	~cBox();

	bool Init(CDisplay *d, cDA2Gfx *gfx);
	void DrawBox(int x, int y, int x2, int y2, int color, bool filled=false, int rotation=0);
  void DrawBox(SDL_Rect& r, int color, bool filled=false, int rotation=0);

protected:
private:
  CDisplay *display;
	cDA2Gfx *ddGfx;
};

#endif