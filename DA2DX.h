#ifndef _DA2DX_H
#define _DA2DX_H

#include "DA2Structs.h"
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

typedef struct sDAVidMode{
  int w;
  int h;
  string name;
}sDAVidMode;

typedef struct sDAPal{
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} sDAPal;

class CDisplay {
public:
  CDisplay();
  ~CDisplay();

  SDL_Surface*  screenSurface;
  SDL_Renderer* renderer;
  SDL_Window*   window;

  bool init(sConf& conf);
  void resize(size_t screenMode);

  vector<sDAVidMode> screenModes;
  int           screenHeight;
  int           screenWidth;
  size_t        currentScreenMode;
  SDL_Color     txtColors[20];
  sDAPal        palette[256];

private:

};

/*
#include <ddraw.h>

//using namespace std;

class cDX{
public:
	LPDIRECTDRAW7 dd;
	LPDIRECTDRAWSURFACE7 ddsp;	//primary
	LPDIRECTDRAWSURFACE7 ddsb;	//backbuffer
	LPDIRECTDRAWSURFACE7 ddsc;	//collision
	LPDIRECTDRAWPALETTE ddPal;
	LPDIRECTDRAWCLIPPER ddClip;
	LPDIRECTDRAWCLIPPER ddClipCollide;

	PALETTEENTRY pal[256];
	int rotation;

	LPDIRECTDRAWPALETTE ddPalRot[7];

	cDX();
	~cDX();

	bool ReInit(HWND wnd, int colorDepth=8);
	bool Init(HWND wnd, int colorDepth=8);
	LPDIRECTDRAWPALETTE DDLoadPalette(LPDIRECTDRAW7 pdd, LPCSTR szBitmap);
	LPDIRECTDRAWPALETTE DA2LoadPalette(LPDIRECTDRAW7 pdd, char* file);
	int LoadBitmapResource(LPDIRECTDRAWSURFACE7 lpdds, int xDest, int yDest, char *fn);
	void RotatePalette();
	bool ChangeDisplayMode(int colorDepth=8);
	bool SetSurfaces(int colorDepth=8);

	DWORD ElapsedTime;

};
*/

#endif