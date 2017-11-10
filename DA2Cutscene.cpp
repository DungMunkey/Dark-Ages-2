#include "DA2Cutscene.h"

DA2Cutscene::DA2Cutscene(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	Maps=NULL;
	animCounter=NULL;
  music=NULL;
}

DA2Cutscene::~DA2Cutscene(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	Maps=NULL;
	animCounter=NULL;
  music=NULL;
}

bool DA2Cutscene::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cMap *maps, cDA2Music *mus, int *anim){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((Maps=maps)==NULL) return false;
  if((music=mus)==NULL) return false;
	if((animCounter=anim)==NULL) return false;
	text.Init(display,ddGfx,diObj);
  lastTicks=0;
	return true;
}

bool DA2Cutscene::Render(){
	int x,y,z;
	int Tile;
	SDL_Rect r;
	int TrueMap,TrueX,TrueY;

	//animate
	//if(*bAnimNow){
	//	*bAnimNow=false;
	//	animCounter++;
	//	if(animCounter>3) animCounter=0;
	//}

	//Clear the screen
  //SDL_SetRenderTarget(display->renderer, canvas);
  SDL_RenderClear(display->renderer);

	//Draw the tiles
  r.w=32;
  r.h=32;
	for(z=0;z<4;z++){
		for(y=SceneCam.TilePosY-8;y<SceneCam.TilePosY+8;y++){
			for(x=SceneCam.TilePosX-10;x<SceneCam.TilePosX+11;x++){
				if(y<0){
					if(x<0){
						TrueMap=MapWin[0][0];	TrueX=x+96;	TrueY=y+96;
					} else if(x>95){
						TrueMap=MapWin[2][0];	TrueX=x-96;	TrueY=y+96;
					} else {
						TrueMap=MapWin[1][0];	TrueX=x; TrueY=y+96;
					}
				} else if(y>95){
					if(x<0){
						TrueMap=MapWin[0][2];	TrueX=x+96;	TrueY=y-96;
					} else if(x>95){
						TrueMap=MapWin[2][2];	TrueX=x-96;	TrueY=y-96;
					} else {
						TrueMap=MapWin[1][2];	TrueX=x; TrueY=y-96;
					}
				} else {
					if(x<0){
						TrueMap=MapWin[0][1];	TrueX=x+96;	TrueY=y;
					} else if(x>95){
						TrueMap=MapWin[2][1];	TrueX=x-96;	TrueY=y;
					} else {
						TrueMap=MapWin[1][1];	TrueX=x;	TrueY=y;
					}
				}
				Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[TrueX][TrueY][z]];
				if(Tile==0) continue;
				if(z==3 && !bRoof) continue;
				if(Tile>2000) Tile+=*animCounter;
				
				r.x=(x-(SceneCam.TilePosX-10))*32-SceneCam.OffsetX;
				r.y=(y-(SceneCam.TilePosY-8))*32-SceneCam.OffsetY;
				if(r.x>639 || r.y>479) continue;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[ddGfx->vTile->at(Tile).index], &ddGfx->vTile->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[ddGfx->vTile->at(Tile).index]->texture, &ddGfx->vTile->at(Tile).r, &r);
			}
		
		}

		//display objects
		if(z==1) ObjRender();
		if(z==1) NPCRender();

	}

	//display border
	r.x=0;
	r.y=0;
	r.h=480;
	r.w=640;
	//ddObj->ddsb->Blt(&r,ddGfx->Border,&r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
  SDL_RenderCopy(display->renderer, ddGfx->Border->texture, &r, &r);

	//display any text
	if(bText) text.Render();

  //SDL_SetRenderTarget(display->renderer, NULL);
  //SDL_RenderCopy(display->renderer, canvas, NULL, NULL);

	return true;
}

void DA2Cutscene::ObjRender(){
	int i,x,y,mx,my;
	int TrueMap;
	int Tile;
	SDL_Rect r;

  r.w=32;
  r.h=32;
	for(my=0;my<3;my++){
		for(mx=0;mx<3;mx++){
			TrueMap=MapWin[mx][my];
			for(i=0;i<Maps[TrueMap].ObjArray->size();i++){
				if(Maps[TrueMap].ObjArray->at(i).Status==0) continue;
				x=Maps[TrueMap].ObjArray->at(i).Xco+96*(mx-1);
				y=Maps[TrueMap].ObjArray->at(i).Yco+96*(my-1);
				Tile=Maps[TrueMap].ObjArray->at(i).ObjectPic;

				if(x-(SceneCam.TilePosX-10) >=0 && x-(SceneCam.TilePosX-10) <=20 &&
					 y-(SceneCam.TilePosY-8) >=0 && y-(SceneCam.TilePosY-8) <=16) {
					r.x=(x-(SceneCam.TilePosX-10))*32-SceneCam.OffsetX;
					r.y=(y-(SceneCam.TilePosY-8))*32-SceneCam.OffsetY;
					//if(Tile>0) ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(Tile).index], &ddGfx->vObj->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
          if(Tile>0) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(Tile).index]->texture, &ddGfx->vObj->at(Tile).r, &r);
				}
			}
		}
	}

}

void DA2Cutscene::NPCRender(){
	int i,x,y;
	int Tile;
	SDL_Rect r;

  r.w=32;
  r.h=32;
	for(i=0;i<20;i++){
		if(npc[i].character<0) continue;

		x=npc[i].posX/32;
		y=npc[i].posY/32;
		Tile=npc[i].character+npc[i].direction+npc[i].frame-1;
		if(npc[i].bMoving) Tile+=*animCounter;

		if(x-(SceneCam.TilePosX-10) >=0 && x-(SceneCam.TilePosX-10) <=20 &&
			 y-(SceneCam.TilePosY-8) >=0 && y-(SceneCam.TilePosY-8) <=16) {
			r.x=(x-(SceneCam.TilePosX-10))*32-SceneCam.OffsetX+npc[i].posX%32;
			r.y=(y-(SceneCam.TilePosY-8))*32-SceneCam.OffsetY+npc[i].posY%32;
			//ddObj->ddsb->Blt(&r, ddGfx->Chars[ddGfx->vNPC->at(Tile).index], &ddGfx->vNPC->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Chars[ddGfx->vNPC->at(Tile).index]->texture, &ddGfx->vNPC->at(Tile).r, &r);
		}
	}

}

bool DA2Cutscene::Logic(){
	int i;
	int j;
	char str[512];

  music->CheckState();

  unsigned int curTicks=SDL_GetTicks();

	//If scene is running, keep going until all movement stops
	if(bAction){

    if(curTicks-lastTicks>10) lastTicks=curTicks;
    else return true;

		//Try to stop the action
		bAction=false;

		//Check the NPCs
		for(i=0;i<20;i++){

			if(npc[i].character<0) continue;
			npc[i].bMoving=false;

			//Y-movement
			j=npc[i].destY-npc[i].posY;
			if(j!=0){

				//this npc still moving, so keep the action rolling
				bAction=true;
				npc[i].bMoving=true;

				if(j<0) {
					npc[i].posY-=npc[i].speed;
					if(npc[i].destY-npc[i].posY > 0) npc[i].posY=npc[i].destY;
				} else {
					npc[i].posY+=npc[i].speed;
					if(npc[i].destY-npc[i].posY < 0) npc[i].posY=npc[i].destY;
				}

			}

			//X-movement
			j=npc[i].destX-npc[i].posX;
			if(j!=0){

				//this npc still moving, so keep the action rolling
				bAction=true;
				npc[i].bMoving=true;

				if(j<0) {
					npc[i].posX-=npc[i].speed;
					if(npc[i].destX-npc[i].posX > 0) npc[i].posX=npc[i].destX;
				} else {
					npc[i].posX+=npc[i].speed;
					if(npc[i].destX-npc[i].posX < 0) npc[i].posX=npc[i].destX;
				}

			}

		}

		//Check the camera
		i = CamX - SceneCam.CameraX;
		j = CamY - SceneCam.CameraY;
		if(i!=0 || j!=0) {
			bAction=true;
			SceneCam.PanCamera(CamX,CamY,CamSpeed);
		}

	//if there is a text window, handle that
	} else if(bText) {
		bText=text.Logic();

	//see if waiting for user click
	} else if(bPause){
		diObj->pollEvents();

		if( diObj->CheckLockMouse(0) || diObj->CheckLock(KEY_SPACE)) return true;
		if( diObj->MousePress(0) || diObj->KeyPress(KEY_SPACE)){
			diObj->LockMouse(0);
			diObj->LockKey(KEY_SPACE);
			while(diObj->CheckLock(KEY_SPACE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			bPause=false;
		}

	//otherwise, set up more actions
	} else {

		while(sceneIndex<scene.size()){
			switch(scene[sceneIndex].type){
			case 0:
				MapWin[1][1]=scene[sceneIndex].target;
				break;
			case 1:
				parser.parseText(scene[sceneIndex].text,str);
				text.SetWindow(48,258,str);
				bText=true;
				sceneIndex++;
				return true;
			case 2:
				CamX=scene[sceneIndex].posX+32;
				CamY=scene[sceneIndex].posY+32;
				if(CamY<224) CamY=224;
				CamSpeed=scene[sceneIndex].speed;
				bAction=true;
				sceneIndex++;
				return true;
			case 3:
				npc[scene[sceneIndex].target].character=scene[sceneIndex].pic;
				npc[scene[sceneIndex].target].posX=scene[sceneIndex].posX+32;
				npc[scene[sceneIndex].target].posY=scene[sceneIndex].posY+32;
				npc[scene[sceneIndex].target].destX=scene[sceneIndex].posX+32;
				npc[scene[sceneIndex].target].destY=scene[sceneIndex].posY+32;
				npc[scene[sceneIndex].target].frame=0;
				npc[scene[sceneIndex].target].bMoving=false;
				break;
			case 4:
				npc[scene[sceneIndex].target].character=-1;
				break;
			case 5:
				switch(scene[sceneIndex].pic){
				case 2:npc[scene[sceneIndex].target].direction=0; break;
				case 4:npc[scene[sceneIndex].target].direction=12; break;
				case 6:npc[scene[sceneIndex].target].direction=4; break;
				case 8:npc[scene[sceneIndex].target].direction=8; break;
				default: break;
				}
				npc[scene[sceneIndex].target].destX=scene[sceneIndex].posX+32;
				npc[scene[sceneIndex].target].destY=scene[sceneIndex].posY+32;
				npc[scene[sceneIndex].target].speed=scene[sceneIndex].speed;
				break;
			case 6:
				switch(scene[sceneIndex].pic){
				case 2:npc[scene[sceneIndex].target].direction=0; break;
				case 4:npc[scene[sceneIndex].target].direction=12; break;
				case 6:npc[scene[sceneIndex].target].direction=4; break;
				case 8:npc[scene[sceneIndex].target].direction=8; break;
				default: break;
				}
				break;
			case 7:
				break;
			case 8:
				bPause=true;
				sceneIndex++;
				return true;
			case 9:
				CamX=scene[sceneIndex].posX+32;
				CamY=scene[sceneIndex].posY+32;
				if(CamY<224) CamY=224;
				SceneCam.SetCamera(CamX,CamY);
				break;
			case 10:
				bAction=true;
				sceneIndex++;
				return true;
			case 11:
				if(bRoof) bRoof=false;
				else bRoof=true;
				break;
			case 12:
			case 13:
        break;
			case 14:
        music->ChangeSong(99999);
        break;

			default:
				break;
			}
			sceneIndex++;
		}
		return false;

	}

	return true;
}

bool DA2Cutscene::LoadScene(int index, char *p1, char *p2, char *p3){
	FILE *f;
	int i,j;
	Act a;
	char str[512];
	scene.clear();
	sceneIndex=0;
	bool bDone=false;

	f=fopen("Data/cutscr.dat","rt");

	while(!feof(f) && !bDone){

		fgets(str,512,f);
		if(str[0]==0) continue;
		if(feof(f)) {
			fclose(f);
			return false;
		}

		//see if this is the right script
		if(str[0]=='*') {
			fscanf(f,"%d\n",&i);
			if(i==index){

				//if so, read it into memory
				while(!bDone){

					//get each command
					fgets(str,512,f);

					//end of script
					if(str[0]=='!'){
						bDone=true;
						break;
					} else {
						a.type=atoi(str);
					}
					switch(a.type){
					case 0: //set map
						fscanf(f,"%d\n",&a.target);
						break;
					case 1: //set text
						fgets(a.text,512,f);
						break;
					case 2: //pan camera
						fscanf(f,"%d,%d,%d\n",&a.posX,&a.posY,&a.speed);
						break;
					case 3: //show npc
						fscanf(f,"%d,%d,%d,%d\n",&a.target,&a.pic,&a.posX,&a.posY);
						break;
					case 4: //hide npc
						fscanf(f,"%d\n",&a.target);
						break;
					case 5: //move npc
						fscanf(f,"%d,%d,%d,%d,%d\n",&a.target,&a.pic,&a.posX,&a.posY,&a.speed);
						break;
					case 6: //turn npc
						fscanf(f,"%d,%d\n",&a.target,&a.pic);
						break;
					case 7: //skip
						break;
					case 8: //wait for input
						break;
					case 9: //set camera
						fscanf(f,"%d,%d\n",&a.posX,&a.posY);
						break;
					case 10: //action!
						break;
					case 11: //hide/show roof
						break;
					case 12: //show credits - end of game
						break;
					case 13: //exit game - end of game
						break;
					case 14: //set song
						fscanf(f,"%d\n",&a.target);
						break;
					default:
						break;
					}

					scene.push_back(a);

				}//while

			}//if index
		}//if *

	}//while not eof

	fclose(f);

	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			MapWin[i][j]=0;
		}
	}
	CamX=500;
	CamY=500;
	CamSpeed=1;
	for(i=0;i<20;i++)	{
		npc[i].character=-1;
		npc[i].direction=0;
		npc[i].frame=0;
	}
	bAction=false;
	bPause=false;
	bBorder=false;
	bPause=false;
	bRoof=true;
	parser.setNames(p1,p2,p3);

	return true;
}