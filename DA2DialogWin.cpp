#include "DA2DialogWin.h"

bool cDialogWin::SetWindow(int x, int y, char *txt, bool but){
	xPos=x;
	yPos=y;

	buttons[0].x=x+107; buttons[0].y=y+153; buttons[0].w=61; buttons[0].h=24;
	buttons[1].x=x+171; buttons[1].y=y+153; buttons[1].w=61; buttons[1].h=24;

	bButtons=but;
	bAccept=false;
	text.Init(ddGfx);
	strcpy(str,txt);
	return true;
	
}

bool cDialogWin::GetButton(){
	return bAccept;
}

bool cDialogWin::Render(bool mouse){
	SDL_Rect r;
  Uint8 R, G, B, A;

	//Draw the dialog window
	r.x=xPos;
	r.y=yPos;
	r.w=245;
	r.h=140;

  //translucent background
  SDL_GetRenderDrawColor(display->renderer, &R, &G, &B, &A);
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 200);
  SDL_RenderFillRect(display->renderer, &r);
  SDL_SetRenderDrawColor(display->renderer, R, G, B, A);

	//ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[1], DDBLT_WAIT,NULL);
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[1], &r);

	//Draw the buttons
	if(bButtons){
		r.y=yPos+140;
		r.h=50;
		//ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[3], DDBLT_WAIT,NULL);
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[3], &r);
	}
	
	//Draw the text message
	text.drawText(display->renderer,xPos+15,yPos+15,25,str);

	//Draw mouse cursor
	if(mouse){
		r.y=diObj->MouseY()-16;
		r.x=diObj->MouseX()-16;
		r.h=32;
		r.w=32;
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
	}
	return true;
}

//This function checks to see if a button was pushed
bool cDialogWin::Logic(){
	diObj->pollEvents();

	//Make sure keys and buttons aren't locked
	if( diObj->CheckLockMouse(0) ) return true;

	if( diObj->MousePress(0) ){
		diObj->LockMouse(0);
	
		if(bButtons){
			if(diObj->CheckMousePos(buttons[0])){
				bAccept=true;
				return false;
			}

      if(diObj->CheckMousePos(buttons[1])){
				bAccept=false;
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}


