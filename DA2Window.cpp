#include "DA2Window.h"

cDA2Window::cDA2Window(){
	display=NULL;
	ddGfx=NULL;
}

cDA2Window::~cDA2Window(){
	display=NULL;
	ddGfx=NULL;
}

bool cDA2Window::Init(CDisplay *d, cDA2Gfx *gfx){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	box.Init(display,ddGfx);
	return true;
}

bool cDA2Window::SetSize(int x, int y, int w, int h){
	xPos=x;
	yPos=y;
	width=w;
	height=h;

	frame[0]=ddGfx->aWindows[24];
	frame[0].w=width-28;
	frame[1]=ddGfx->aWindows[25];
	frame[1].h=height-28;
	frame[2]=ddGfx->aWindows[26];
	frame[2].w=width-28;
	frame[3]=ddGfx->aWindows[27];
	frame[3].h=height-28;

	return true;
}

bool cDA2Window::DrawWindow(){
	SDL_Rect r;

	//Clear the inside
	box.DrawBox(xPos,yPos,xPos+width,yPos+height,0,true);

	//The frame
	r.x=xPos+14; r.y=yPos; r.w=width-28; r.h=12;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &frame[0], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &frame[0], DDBLT_WAIT,NULL);
	r.x=xPos; r.y=yPos+14; r.w=12; r.h=height-28;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &frame[1], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &frame[1], DDBLT_WAIT,NULL);
	r.x=xPos+14; r.y=yPos+height-12; r.w=width-28; r.h=12;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &frame[2], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &frame[2], DDBLT_WAIT,NULL);
	r.x=xPos+width-12; r.y=yPos+14; r.w=12; r.h=height-28;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &frame[3], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &frame[3], DDBLT_WAIT,NULL);

	//The corners
	r.x=xPos; r.y=yPos; r.w=25; r.h=25;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[28], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[28], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	r.x=xPos+width-25; r.y=yPos; r.w=25; r.h=25;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[29], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[29], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	r.x=xPos; r.y=yPos+height-25; r.w=25; r.h=25;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[30], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[30], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	r.x=xPos+width-25; r.y=yPos+height-25; r.w=25; r.h=25;
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[31], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[31], DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	return true;

}
