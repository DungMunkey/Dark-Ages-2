#include "DA2TextWin.h"

bool cTextWin::SetWindow(int x, int y, char *txt){
	xPos=x;
	yPos=y;

	//text.Init(ddGfx);
	strcpy(str,txt);
	return true;
	
}

bool cTextWin::Render(){
	SDL_Rect r;
  Uint8 R, G, B, A;
	
  r.x=xPos;
	r.y=yPos;
	r.w=545;
	r.h=175;
  
  //translucent background
  SDL_GetRenderDrawColor(display->renderer, &R, &G, &B, &A);
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 200);
  SDL_RenderFillRect(display->renderer, &r);
  SDL_SetRenderDrawColor(display->renderer, R, G, B, A);

  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[0], &r);
	text.drawText(display->renderer,xPos+21,yPos+16,63,str);

	return true;
}

//This function checks to see if space or mouse button was pressed. If so, close the window.
bool cTextWin::Logic(){

	diObj->pollEvents();

	//Make sure keys and buttons aren't locked
	if( diObj->CheckLockMouse(0) || diObj->CheckLockMouse(1) || diObj->CheckLock(KEY_SPACE)) return true;

	if( diObj->MousePress(0) ){
		diObj->LockMouse(0);
		return false;
	}

	if( diObj->KeyPress(KEY_SPACE) ) {
		diObj->LockKey(KEY_SPACE);
		return false;
	}

	return true;
}


