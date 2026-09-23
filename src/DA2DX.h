#ifndef _DA2DX_H
#define _DA2DX_H

#include "DA2Structs.h"
#include <SDL3/SDL.h>
#include "SDLCompat.h"
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

  //SDL_Surface*  screenSurface;
  SDL_Renderer* renderer;
  SDL_Window*   window;
  SDL_PixelFormat pixelFormat;

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

#endif
