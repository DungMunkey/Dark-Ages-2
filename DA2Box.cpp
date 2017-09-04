#include "DA2Box.h"

cBox::cBox(){}

cBox::~cBox(){
	display=NULL;
	ddGfx=NULL;
}

bool cBox::Init(CDisplay *d, cDA2Gfx *gfx){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	return true;
}

void cBox::DrawBox(int x, int y, int x2, int y2, int color, bool filled, int rotation){
  SDL_Rect r;
  r.x=x;
  r.y=y;
  r.w=x2 - x;
  r.h=y2 - y;
  DrawBox(r, color, filled, rotation);
}

void cBox::DrawBox(SDL_Rect& r, int color, bool filled, int rotation){
  Uint8 R, G, B, A;

  SDL_GetRenderDrawColor(display->renderer, &R, &G, &B, &A);
  switch(color){
  case 80:
  case 88:
  case 128:
  case 136:
    SDL_SetRenderDrawColor(display->renderer, display->palette[color + rotation].r, display->palette[color + rotation].g, display->palette[color + rotation].b, 255);
    break;
  default: 
    SDL_SetRenderDrawColor(display->renderer, display->palette[color].r, display->palette[color].g, display->palette[color].b, 255);
    break;
  }
  if(filled) SDL_RenderFillRect(display->renderer, &r);
  else SDL_RenderDrawRect(display->renderer, &r);
  SDL_SetRenderDrawColor(display->renderer, R, G, B, A);
}

