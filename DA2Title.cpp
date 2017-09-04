#include "DA2Title.h"

cDA2Title::cDA2Title(){
  display=NULL;
  input=NULL;
  Title=NULL;
  Credits=NULL;
}

cDA2Title::~cDA2Title(){
  display=NULL;
  input=NULL;
	if(Title){
		delete Title;
		Title=NULL;
	}
  if(Credits){
    delete Credits;
    Credits=NULL;
  }
}

bool cDA2Title::Init(CDisplay *d,cDA2Input *di){
  display=d;
  input=di;
  Title = new CGraphic("Gfx/TitleScreen.bmp",display,true);
  Credits = new CGraphic("Gfx/Credits.bmp",display);

  /*
	if((ddObj=dd)==NULL) return false;
	if((diObj=di)==NULL) return false;
	
	//Set up generic surface description
	DDSURFACEDESC2 ddsdGeneric;
	ZeroMemory(&ddsdGeneric, sizeof(ddsdGeneric));
	ddsdGeneric.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsdGeneric.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsdGeneric.dwHeight = 530;
	ddsdGeneric.dwWidth = 640;
	ddsdGeneric.dwSize = sizeof(ddsdGeneric);

	//Load graphics onto surfaces
	ddObj->dd->CreateSurface(&ddsdGeneric, &Title, NULL);
	LoadBitmapResource(Title,0,0,"Gfx/TitleScreen.bmp");
  ddObj->dd->CreateSurface(&ddsdGeneric, &Credits, NULL);
	LoadBitmapResource(Credits,0,0,"Gfx/Credits.bmp");

	DDCOLORKEY	ddck;
	ddck.dwColorSpaceLowValue = 0;
  ddck.dwColorSpaceHighValue = 0;
	Title->SetColorKey(DDCKEY_SRCBLT, &ddck);
  */

	//Set up locations for mouse-overs
	aTitle[0].x=282; aTitle[0].y=266; aTitle[0].w=77; aTitle[0].h=44; 
	aTitle[1].x=273; aTitle[1].y=310; aTitle[1].w=94; aTitle[1].h=44;
	aTitle[2].x=252; aTitle[2].y=354; aTitle[2].w=136; aTitle[2].h=46;
	aTitle[3].x=280; aTitle[3].y=401; aTitle[3].w=79; aTitle[3].h=45;

	//Set up coordinates of mouse-over graphics
	bTitle[0].x=0; bTitle[0].y=480; bTitle[0].w=77; bTitle[0].h=44;
	bTitle[1].x=77; bTitle[1].y=480; bTitle[1].w=94; bTitle[1].h=44;
	bTitle[2].x=171; bTitle[2].y=480; bTitle[2].w=136; bTitle[2].h=46;
	bTitle[3].x=307; bTitle[3].y=480; bTitle[3].w=79; bTitle[3].h=45;

	cursor.x=386; cursor.y=480; cursor.w=16; cursor.h=16;
	return true;
}

bool cDA2Title::Render(){
	SDL_Rect r;
	int i;

  r.x=0; r.y=0; r.w=640; r.h=480;
  if(bCredits){
	  SDL_RenderCopy(display->renderer,Credits->texture,&r,&r);
  } else {
    SDL_RenderCopy(display->renderer, Title->texture, &r, &r);
  }
	//ddObj->ddsb->Blt(&r, Title, &r, DDBLT_WAIT,NULL);

  if(!bCredits){
    int mx = input->MouseX();
    int my = input->MouseY();
	  //diObj->Read();
	  for(i=0;i<4;i++){
		  if(mx>=aTitle[i].x && mx<=(aTitle[i].x+aTitle[i].w) && 
			   my>=aTitle[i].y && my<(aTitle[i].y+aTitle[i].h)){ 
        SDL_RenderCopy(display->renderer, Title->texture, &bTitle[i], &aTitle[i]);
			  //ddObj->ddsb->BltFast(aTitle[i].left,aTitle[i].top, Title, &bTitle[i], DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
			  //ddObj->ddsb->Blt(&aTitle[i], Title, &bTitle[i], DDBLT_WAIT,NULL);
		  }
	  }
  }

	//display the mouse cursor && selected item
	r.y=input->mouseY;
	r.x=input->mouseX;
	r.w=16;
	r.h=16;
  SDL_RenderCopy(display->renderer, Title->texture, &cursor, &r);
	//ddObj->ddsb->BltFast(r.left,r.top, Title, &cursor, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//ddObj->ddsb->Blt(&r, Title,&cursor,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

  SDL_RenderPresent(display->renderer);
	return true;
}

int cDA2Title::Logic(){
	int i;
	//diObj->Read();
  //Render();

  if(!input->CheckLockMouse(0) && input->getButtonState(0)) {
		input->LockMouse(0);

    if(bCredits){
      while(input->CheckLockMouse(0)) input->pollEvents();
      return 5;
    }

    int mx=input->MouseX();
    int my=input->MouseY();
    printf("%d,%d\n", mx, my);

		for(i=0;i<4;i++){
      if(mx >= aTitle[i].x && mx <= (aTitle[i].x + aTitle[i].w) &&
         my >= aTitle[i].y && my<(aTitle[i].y + aTitle[i].h)){
				while(input->CheckLockMouse(0)) input->pollEvents();
				return i+1;
			}
		}
	}
	return 0;

  /*
  while(true){

    i=title.run();
    if(i == 1){
      if(newGame()) {
        fadeIn=255;
        script.clear();
        script.addText("Your adventure begins!");
        showText=true;
        run();
      }
    } else if(i == 2){
      if(titleLoad()>0) run();
    } else if(i == 3){
      COptions opt(display, &music, &font, &gfx, conf);
      opt.run();
    } else if(i == 4){
      return;
    }
  }
  */

}

void cDA2Title::SetCredits(bool b){
  bCredits=b;
}

