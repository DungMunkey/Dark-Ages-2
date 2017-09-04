#include "DA2ListWin.h"

bool cListWin::SetWindow(int x, int y, bool six){
	xPos=x;
	yPos=y;

	listThree[0].x=x+10; listThree[0].y=y+10; listThree[0].w=225; listThree[0].h=50;
	listThree[1].x=x+10; listThree[1].y=y+50; listThree[1].w=225; listThree[1].h=90;
	listThree[2].x=x+10; listThree[2].y=y+90; listThree[2].w=225; listThree[2].h=130;

	listSix[0].x=x+10; listSix[0].y=y+10; listSix[0].w=225; listSix[0].h=19;
	listSix[1].x=x+10; listSix[1].y=y+30; listSix[1].w=225; listSix[1].h=19;
	listSix[2].x=x+10; listSix[2].y=y+50; listSix[2].w=225; listSix[2].h=19;
	listSix[3].x=x+10; listSix[3].y=y+70; listSix[3].w=225; listSix[3].h=19;
	listSix[4].x=x+10; listSix[4].y=y+90; listSix[4].w=225; listSix[4].h=19;
	listSix[5].x=x+10; listSix[5].y=y+110; listSix[5].w=225; listSix[5].h=19;


	selection=-1;
	bSix=six;

	size=0;

	text.Init(ddGfx);
	box.Init(display,ddGfx);
	return true;
	
}

bool cListWin::AddItem(char *txt){
	if(bSix && size==6) return false;
	if(!bSix && size==3) return false;
	strcpy(list[size++],txt);
	return true;
}

int cListWin::GetSelection(){
	return selection;
}

bool cListWin::Render(){
	int i;
	SDL_Rect r;
  Uint8 R, G, B, A;
	//char str[10];

	//Draw window
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

	//Draw List Items
	for(i=0;i<size;i++)	{
		if(bSix){
      text.drawText(display->renderer, xPos + 12, yPos + 11 + (i * 20), 25, list[i]);
		} else {
      text.drawText(display->renderer, xPos + 12, yPos + 11 + (i * 40), 25, list[i]);
		}
	}

	//Draw Selection box
	for(i=0;i<size;i++){
		if(bSix){
      if(diObj->CheckMousePos(listSix[i])){
				box.DrawBox(listSix[i],15);
				//sprintf(str,"%d",i);
				//text.DrawText(ddObj->ddsb,xPos+200,yPos+11,10,str);
			}
		} else {
      if(diObj->CheckMousePos(listThree[i])){
				box.DrawBox(listThree[i],15);
			}
		}
	}

	//Draw mouse cursor
	r.x=diObj->mouseX-16;
	r.y=diObj->mouseY-16;
	r.w=32;
	r.h=32;
	//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
  SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);

	return true;
}

//This function checks to see if a button was pushed
bool cListWin::Logic(){
	int i;
	diObj->pollEvents();

	//Make sure keys and buttons aren't locked
  if(diObj->CheckLockMouse(0) || diObj->CheckLockMouse(1) || diObj->CheckLock(KEY_ESCAPE)) return true;

	//Check if click happened on a list item.
	if( diObj->MousePress(0) ){

		diObj->LockMouse(0);
		for(i=0;i<size;i++){
			if(bSix){
				if(diObj->CheckMousePos(listSix[i])){
					selection=i;
					return false;
				}
			} else {
				if(diObj->CheckMousePos(listThree[i])){
					selection=i;
					return false;
				}
			}
		}

	}

	if( diObj->MousePress(1) ) {
		diObj->LockMouse(1);
		selection=-1;
		return false;
	}

  if(diObj->KeyPress(KEY_ESCAPE)) {
    diObj->LockKey(KEY_ESCAPE);
    selection=-1;
    return false;
  }

	return true;
}


