#ifndef _DA2TITLE_H
#define _DA2TITLE_H

#include "DA2DX.h"
#include "DA2Graphic.h"
#include "DA2Input.h"

class cDA2Title{
public:
	CGraphic* Title;
  CGraphic* Credits;

	SDL_Rect aTitle[4];
	SDL_Rect bTitle[4];
	SDL_Rect cursor;

	bool Init(CDisplay *d, cDA2Input *di);
	bool Render();
	int Logic();
  void SetCredits(bool b);

	cDA2Title();
	~cDA2Title();

protected:

private:
	CDisplay *display;
	cDA2Input *input;
  bool bCredits;

	//int LoadBitmapResource(LPDIRECTDRAWSURFACE7 lpdds, int xDest, int yDest, char *fn);

};

#endif