#include "DA2Options.h"


cDA2Options::cDA2Options(){
	Options.Border=true;
	Options.Diagnostics=false;
  Options.Fullscreen=false;
	Options.GameSpeed=2;
	Options.MusicVolume=5;
  Options.ScreenRes=0;

	buttons[0].x=300; buttons[0].w=16; buttons[0].y=80; buttons[0].h=16;
  buttons[1].x=390; buttons[1].w=16; buttons[1].y=80; buttons[1].h=16;
  buttons[2].x=300; buttons[2].w=16; buttons[2].y=110; buttons[2].h=16;
  buttons[3].x=390; buttons[3].w=16; buttons[3].y=110; buttons[3].h=16;
  buttons[4].x=300; buttons[4].w=16; buttons[4].y=140; buttons[4].h=16;
  buttons[5].x=390; buttons[5].w=16; buttons[5].y=140; buttons[5].h=16;
  buttons[6].x=300; buttons[6].w=16; buttons[6].y=170; buttons[6].h=16;
  buttons[7].x=390; buttons[7].w=16; buttons[7].y=170; buttons[7].h=16;
  buttons[8].x=300; buttons[8].w=16; buttons[8].y=200; buttons[8].h=16;
  buttons[9].x=390; buttons[9].w=16; buttons[9].y=200; buttons[9].h=16;
  buttons[10].x=420; buttons[10].w=16; buttons[10].y=200; buttons[10].h=16;
  buttons[11].x=300; buttons[11].w=16; buttons[11].y=230; buttons[11].h=16;
  buttons[12].x=390; buttons[12].w=16; buttons[12].y=230; buttons[12].h=16;
  buttons[13].x=420; buttons[13].w=16; buttons[13].y=230; buttons[13].h=16;
}

cDA2Options::~cDA2Options(){
  conf=NULL;
}

bool cDA2Options::Logic(){
	int i;
	bool bClick;

	diObj->pollEvents();

	//Make sure keys and buttons aren't locked
	if( diObj->CheckLockMouse(0) ) return true;

	if( diObj->MousePress(0) ){
		diObj->LockMouse(0);

		//check the buttons
		for(i=0;i<14;i++){
      if(diObj->CheckMousePos(buttons[i])){

				switch(i){
				case 0:
				case 1:
					if(Options.Border) Options.Border=false;
					else Options.Border=true;
					break;
				case 2:
					if(Options.MusicVolume>0) Options.MusicVolume--;
					break;
				case 3:
					if(Options.MusicVolume<10) Options.MusicVolume++;
					break;
				case 4:
          if(Options.GameSpeed<4) Options.GameSpeed++;
					break;
				case 5:
					if(Options.GameSpeed>1) Options.GameSpeed--;
					break;
				case 6:
				case 7:
					if(Options.Diagnostics) Options.Diagnostics=false;
					else Options.Diagnostics=true;
					break;
        case 8:
          if(tmpScreen>0) tmpScreen--;
          break;
        case 9:
          if(tmpScreen < display->screenModes.size() - 1) tmpScreen++;
          break;
        case 10:
          if(tmpScreen!=Options.ScreenRes) {
            Options.ScreenRes=tmpScreen;
            display->resize(Options.ScreenRes);
            float sx, sy;
            SDL_Texture* tempText =SDL_GetRenderTarget(display->renderer);
            SDL_SetRenderTarget(display->renderer, NULL);
            SDL_RenderGetScale(display->renderer, &sx, &sy);
            SDL_SetRenderTarget(display->renderer, tempText);
            tempText=NULL;
            diObj->setMouseOffsets(sx, (display->screenWidth / sx - 640) / 2, sy, (display->screenHeight / sy - 480) / 2);
            conf->h=display->screenHeight;
            conf->w=display->screenWidth;
          }
          break;
        case 11:
        case 12:
          if(tmpFullscreen) tmpFullscreen=false;
          else tmpFullscreen=true;
          break;
        case 13:
          if(tmpFullscreen!=Options.Fullscreen) {
            Options.Fullscreen=tmpFullscreen;
            if(tmpFullscreen) {
              SDL_SetWindowFullscreen(display->window, SDL_WINDOW_FULLSCREEN);
              conf->fullScreen=true;
            } else {
              SDL_SetWindowFullscreen(display->window, 0);
              conf->fullScreen=false;
            }
          }
          break;
				default:
					break;
				}
			}
		}

	}

	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
		diObj->LockKey(KEY_ESCAPE);
    tmpScreen=Options.ScreenRes;
    tmpFullscreen=Options.Fullscreen;
		return false;
	}

	return true;
}

bool cDA2Options::Render(){
	int i;
	SDL_Rect r;
	char str[8];

	//Draw a blank screen
  SDL_RenderClear(display->renderer);

	//List options
	text.drawText(display->renderer,100,80,0,"Game Border");
	text.drawText(display->renderer,100,110,0,"Music Volume");
	text.drawText(display->renderer,100,140,0,"Game Speed");
	text.drawText(display->renderer,100,170,0,"Show Diagnostics");
  text.drawText(display->renderer, 100, 200, 0, "Screen Resolution");
  text.drawText(display->renderer, 100, 230, 0, "Screen Mode");

	//List option values
	if(Options.Border) text.drawText(display->renderer,324,80,0,"On");
	else text.drawText(display->renderer,324,80,0,"Off");
	sprintf(str,"%d",Options.MusicVolume);
	text.drawText(display->renderer,324,110,0,str);
	sprintf(str,"%d",5-Options.GameSpeed);
	text.drawText(display->renderer,324,140,0,str);
	if(Options.Diagnostics) text.drawText(display->renderer,324,170,0,"On");
	else text.drawText(display->renderer,324,170,0,"Off");
  text.drawText(display->renderer, 324, 200, 0, &display->screenModes[tmpScreen].name[0]);
  if(tmpFullscreen) text.drawText(display->renderer, 324, 230, 0, "Full");
  else text.drawText(display->renderer, 324, 230, 0, "Window");


	//Display checkboxes/buttons
	for(i=0;i<5;i++){
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[21], &buttons[i * 2]);
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[22], &buttons[i * 2 + 1]);
		//ddObj->ddsb->Blt(&buttons[i*2], ddGfx->Windows, &ddGfx->aWindows[21], DDBLT_WAIT,NULL);
		//ddObj->ddsb->Blt(&buttons[i*2+1], ddGfx->Windows, &ddGfx->aWindows[22], DDBLT_WAIT,NULL);
	}
  if(tmpScreen!=Options.ScreenRes){
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[22], &buttons[10]);
  }
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[21], &buttons[11]);
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[22], &buttons[12]);
  if(tmpFullscreen != Options.Fullscreen){
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[22], &buttons[13]);
  }

	//Done button

	//Mouse
	r.y=diObj->MouseY()-16;
	r.x=diObj->MouseX()-16;
	r.h=32;
	r.w=32;
  SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	return true;

}

void cDA2Options::SetConf(sConf* con){
  conf=con;
}

void cDA2Options::Update(){
  Options.ScreenRes=display->currentScreenMode;
  tmpScreen=Options.ScreenRes;
  tmpFullscreen=false;
}