#include "DA2Journal.h"

cDA2Journal::cDA2Journal(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	flags=NULL;
}

cDA2Journal::~cDA2Journal(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	flags=NULL;
}

bool cDA2Journal::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, short *flg){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if(!text.Init(ddGfx)) return false;
	if(!line.Init(display)) return false;
	if(!LoadEntries()) return false;
	if(!box.Init(display,ddGfx)) return false;

	if((flags=flg)==NULL) return false;

	TextOffset=0;
	MaxTextOffset=0;
	curEntry=0;
	bMain=true;
	bComplete=false;
	bPortals=false;
	bQuests=false;
	bTowns=false;
	bKeeps=false;
  
	//Done button
	clickList[0].x=562; clickList[0].w=70; clickList[0].y=440; clickList[0].h=32;
	//Map Up;
  clickList[1].x=502; clickList[1].w=52; clickList[1].y=360; clickList[1].h=32;
	//Map Down
  clickList[2].x=502; clickList[2].w=52; clickList[2].y=392; clickList[2].h=32;
	//Map Left
  clickList[3].x=450; clickList[3].w=52; clickList[3].y=392; clickList[3].h=32;
	//Map Right
  clickList[4].x=554; clickList[4].w=52; clickList[4].y=392; clickList[4].h=32;
	//Text Down
	clickList[5].x=616; clickList[5].w=16; clickList[5].y=130; clickList[5].h=16;
	//Text Up
  clickList[6].x=616; clickList[6].w=16; clickList[6].y=336; clickList[6].h=16;
	//Next Journal Entry
  clickList[7].x=528; clickList[7].w=52; clickList[7].y=40; clickList[7].h=32;
	//Previous Journal Entry
  clickList[8].x=476; clickList[8].w=52; clickList[8].y=40; clickList[8].h=32;
	//First Journal Entry
  clickList[9].x=424; clickList[9].w=52; clickList[9].y=40; clickList[9].h=32;
	//Last Journal Entry
  clickList[10].x=580; clickList[10].w=52; clickList[10].y=40; clickList[10].h=32;
	//Main Quests
  clickList[11].x=424; clickList[11].w=52; clickList[11].y=8; clickList[11].h=32;
	//Side Quests
  clickList[12].x=476; clickList[12].w=52; clickList[12].y=8; clickList[12].h=32;
	//Active Quests
  clickList[13].x=528; clickList[13].w=52; clickList[13].y=8; clickList[13].h=32;
	//Solved Quest
  clickList[14].x=580; clickList[14].w=52; clickList[14].y=8; clickList[14].h=32;
	//Portals Button
  clickList[15].x=73; clickList[15].w=69; clickList[15].y=440; clickList[15].h=32;
	//Center on Party
  clickList[16].x=450; clickList[16].w=52; clickList[16].y=360; clickList[16].h=32;
	//Center on Destination
  clickList[17].x=554; clickList[17].w=52; clickList[17].y=360; clickList[17].h=32;
	//Towns Button
  clickList[18].x=142; clickList[18].w=70; clickList[18].y=440; clickList[18].h=32;
	//Keeps Button
  clickList[19].x=212; clickList[19].w=69; clickList[19].y=440; clickList[19].h=32;
	//Quests Button
  clickList[20].x=281; clickList[20].w=70; clickList[20].y=440; clickList[20].h=32;
	return true;
}

bool cDA2Journal::LoadEntries(){
	FILE* f;
	int i,j;
	char str[512];
	string s;

	for(i=0;i<100;i++){
		JournalText[i].ID=0;
		JournalText[i].TextCount=0;
		JournalText[i].Active=0;
	}

	f=fopen("Data/jour.txt","rt");
	i=0;
	while(!feof(f)){
		fscanf(f,"%d,%d,%d,%d,%d,%d\n",&JournalText[i].ID,&JournalText[i].Difficulty,&JournalText[i].MainQuest,
			&JournalText[i].EndPoint.X,&JournalText[i].EndPoint.Y,&JournalText[i].Active);
		fgets(str,512,f);
		JournalText[i].Title=str;
		fscanf(f,"%d\n",&JournalText[i].TextCount);
		for(j=0;j<JournalText[i].TextCount;j++){
			fgets(str,512,f);
			s=str;
			JournalText[i].Text->push_back(s);
		}
		i++;
	}
	fclose(f);

	bMain=true;
	bComplete=false;
	SortJournal();

	return true;
}

void cDA2Journal::Reset(){
	bMain=true;
	bComplete=false;
	TextOffset=0;
	SortJournal();
}

void cDA2Journal::SortJournal(){
	int i;
	journalIndex.clear();

	for(i=0;i<100;i++){
		if(JournalText[i].Active==0) continue;
		if(bMain && bComplete && JournalText[i].MainQuest>0 && JournalText[i].Difficulty==0) journalIndex.push_back(i);
		if(bMain && !bComplete && JournalText[i].MainQuest>0 && JournalText[i].Difficulty>0) journalIndex.push_back(i);
		if(!bMain && bComplete && JournalText[i].MainQuest==0 && JournalText[i].Difficulty==0) journalIndex.push_back(i);
		if(!bMain && !bComplete && JournalText[i].MainQuest==0 && JournalText[i].Difficulty>0) journalIndex.push_back(i);
	}
	if(journalIndex.size()<1) curEntry=-1;
	else curEntry=0;
}

int cDA2Journal::Logic(){
	int i;
	bool bClick;

	diObj->pollEvents();

	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
		diObj->LockKey(KEY_ESCAPE);
		return 0;
	}

	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		for(i=0;i<21;i++){
      if(diObj->MouseX() >= clickList[i].x && diObj->MouseX() <= clickList[i].x+clickList[i].w &&
         diObj->MouseY() >= clickList[i].y && diObj->MouseY() <= clickList[i].y+clickList[i].h){
				bClick=true;
				break;
			}
		}

		if(bClick){
			switch(i){
			case 0:
				diObj->LockMouse(0);
				return 0;
				break;
			case 1:
				JournalY-=3;
				if(JournalY<0) JournalY=0;
				break;
			case 2:
				JournalY+=3;
				if(JournalY>600) JournalY=600;
				break;
			case 3:
				JournalX-=3;
				if(JournalX<0) JournalX=0;
				break;
			case 4:
				JournalX+=3;
				if(JournalX>616) JournalX=616;
				break;
			case 5:
				if(TextOffset>0) {
					TextOffset-=2;
					if(TextOffset<0) TextOffset=0;
				}
				break;
			case 6:
				if(TextOffset<MaxTextOffset) {
					TextOffset+=2;
					if(TextOffset>MaxTextOffset) TextOffset=MaxTextOffset;
				}
				break;
			case 7: //Next Journal Entry
				diObj->LockMouse(0);
				if(curEntry>-1 && curEntry<journalIndex.size()-1) {
					curEntry++;
					TextOffset=0;
				}
				break;
			case 8: //Previous Journal Entry
				diObj->LockMouse(0);
				if(curEntry>0) {
					curEntry--;
					TextOffset=0;
				}
				break;
			case 9: //First entry
				diObj->LockMouse(0);
				if(curEntry>0) {
					curEntry=0;
					TextOffset=0;
				}
				break;
			case 10: //Last entry
				diObj->LockMouse(0);
				if(curEntry>-1) {
					curEntry=journalIndex.size()-1;
					TextOffset=0;
				}
				break;
			case 11: //main quests
				diObj->LockMouse(0);
				bMain=true;
				SortJournal();
				TextOffset=0;
				break;
			case 12: //side quests
				diObj->LockMouse(0);
				bMain=false;
				SortJournal();
				TextOffset=0;
				break;
			case 13: //active quests
				diObj->LockMouse(0);
				bComplete=false;
				SortJournal();
				TextOffset=0;
				break;
			case 14: //finished quests
				diObj->LockMouse(0);
				bComplete=true;
				SortJournal();
				TextOffset=0;
				break;
			case 15:
				diObj->LockMouse(0);
				if(bPortals) bPortals=false;
				else bPortals=true;
				break;
			case 16:
				diObj->LockMouse(0);
				if(PlayerX>0 && PlayerY>0){
					JournalX=PlayerX-204;
					JournalY=PlayerY-212;
					if(JournalX<0) JournalX=0;
					if(JournalY<0) JournalY=0;
					if(JournalX>616) JournalX=616;
					if(JournalY>600) JournalY=600;
				}
				break;
			case 17:
				diObj->LockMouse(0);
				if(!bComplete && curEntry>-1){
					if(JournalText[journalIndex[curEntry]].EndPoint.X>0 && JournalText[journalIndex[curEntry]].EndPoint.Y>0){
						JournalX=JournalText[journalIndex[curEntry]].EndPoint.X-182;
						JournalY=JournalText[journalIndex[curEntry]].EndPoint.Y-188;
						if(JournalX<0) JournalX=0;
						if(JournalY<0) JournalY=0;
						if(JournalX>616) JournalX=616;
						if(JournalY>600) JournalY=600;
					}
				}
				break;
			case 18:
				diObj->LockMouse(0);
				if(!bTowns) bTowns=true;
				else bTowns=false;
				break;
			case 19:
				diObj->LockMouse(0);
				if(!bKeeps) bKeeps=true;
				else bKeeps=false;
				break;
			case 20:
				diObj->LockMouse(0);
				if(!bQuests) bQuests=true;
				else bQuests=false;
				break;
			default:
				break;
			}
		}
	}

	return 1;
}

bool cDA2Journal::Render(){
	SDL_Rect r,r2;
	int i;
	int LineCount;
	char str[4096];

	//clear the collision surface
  SDL_RenderClear(display->renderer);

	//display map
	r.x=JournalX;	r.y=JournalY;	r.w=408;	r.h=424;
	r2.x=8,r2.w=408; r2.y=8; r2.h=424;
  SDL_RenderCopy(display->renderer, ddGfx->WorldMap->texture, &r, &r2);
	//ddObj->ddsb->Blt(&r2, ddGfx->WorldMap, &r, DDBLT_WAIT,NULL);

	if(bTowns){
		text.drawText(display->renderer,85-JournalX,114-JournalY,0,"Wisp",1);
    text.drawText(display->renderer, 84 - JournalX, 113 - JournalY, 0, "Wisp");

    text.drawText(display->renderer, 235 - JournalX, 602 - JournalY, 0, "Tristen", 1);
    text.drawText(display->renderer, 234 - JournalX, 601 - JournalY, 0, "Tristen");

    text.drawText(display->renderer, 200 - JournalX, 246 - JournalY, 0, "Willowbrook", 1);
    text.drawText(display->renderer, 199 - JournalX, 245 - JournalY, 0, "Willowbrook");

    text.drawText(display->renderer, 517 - JournalX, 408 - JournalY, 0, "Hafen", 1);
    text.drawText(display->renderer, 516 - JournalX, 407 - JournalY, 0, "Hafen");

    text.drawText(display->renderer, 245 - JournalX, 875 - JournalY, 0, "Pagan", 1);
    text.drawText(display->renderer, 244 - JournalX, 874 - JournalY, 0, "Pagan");

    text.drawText(display->renderer, 861 - JournalX, 880 - JournalY, 0, "Trost", 1);
    text.drawText(display->renderer, 860 - JournalX, 879 - JournalY, 0, "Trost");

    text.drawText(display->renderer, 662 - JournalX, 595 - JournalY, 0, "Zephyr", 1);
    text.drawText(display->renderer, 661 - JournalX, 594 - JournalY, 0, "Zephyr");

    text.drawText(display->renderer, 365 - JournalX, 448 - JournalY, 0, "Erond", 1);
    text.drawText(display->renderer, 364 - JournalX, 447 - JournalY, 0, "Erond");

    text.drawText(display->renderer, 760 - JournalX, 370 - JournalY, 0, "Ilmari", 1);
		text.drawText(display->renderer,759-JournalX,369-JournalY,0,"Ilmari");

		text.drawText(display->renderer,750-JournalX,695-JournalY,0,"Unschuld",1);
		text.drawText(display->renderer,749-JournalX,694-JournalY,0,"Unschuld");

		text.drawText(display->renderer,870-JournalX,435-JournalY,0,"Nanten",1);
		text.drawText(display->renderer,869-JournalX,434-JournalY,0,"Nanten");
	}

	if(bKeeps){
		text.drawText(display->renderer,170-JournalX,158-JournalY,0,"Galgarant",1);
		text.drawText(display->renderer,169-JournalX,157-JournalY,0,"Galgarant",6);

		text.drawText(display->renderer,133-JournalX,217-JournalY,0,"Regskada",1);
		text.drawText(display->renderer,132-JournalX,216-JournalY,0,"Regskada",6);

		text.drawText(display->renderer,332-JournalX,256-JournalY,0,"Illana",1);
		text.drawText(display->renderer,331-JournalX,255-JournalY,0,"Illana",6);

		text.drawText(display->renderer,233-JournalX,725-JournalY,0,"Hans-Dirk",1);
		text.drawText(display->renderer,232-JournalX,724-JournalY,0,"Hans-Dirk",6);

		text.drawText(display->renderer,695-JournalX,925-JournalY,0,"Sebastian",1);
		text.drawText(display->renderer,694-JournalX,924-JournalY,0,"Sebastian",6);

		text.drawText(display->renderer,552-JournalX,500-JournalY,0,"Matthias",1);
		text.drawText(display->renderer,551-JournalX,499-JournalY,0,"Matthias",6);

		text.drawText(display->renderer,450-JournalX,980-JournalY,0,"Brightblade",1);
		text.drawText(display->renderer,449-JournalX,979-JournalY,0,"Brightblade",6);

		text.drawText(display->renderer,665-JournalX,265-JournalY,0,"Maenig",1);
		text.drawText(display->renderer,664-JournalX,264-JournalY,0,"Maenig",6);
	}


	if(bPortals) DrawPortals();

	//Draw the quests
  r.w=32;
  r.h=32;
	if(bQuests) {
		for(i=0;i<100;i++){
			if(JournalText[i].Active==0) continue;
			if(JournalText[i].EndPoint.X>0 && JournalText[i].EndPoint.Y>0){
				r.y=JournalText[i].EndPoint.Y-JournalY+24;
				r.x=JournalText[i].EndPoint.X-JournalX+22;
        if(JournalText[i].MainQuest) SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(50).r, &r); 
        //ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->vTile->at(50).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
        else SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(48).r, &r); 
        //ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->vTile->at(48).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
			}
		}
	}

	//display the Player location
	if(PlayerX>0 && PlayerY>0){
		r.y=PlayerY-JournalY;
		r.x=PlayerX-JournalX;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}

	//display the quest destination
	if(!bComplete && curEntry>-1){
		if(JournalText[journalIndex[curEntry]].EndPoint.X>0 && JournalText[journalIndex[curEntry]].EndPoint.Y>0){
			r.y=JournalText[journalIndex[curEntry]].EndPoint.Y-JournalY+24;
			r.x=JournalText[journalIndex[curEntry]].EndPoint.X-JournalX+22;
      SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(26).r, &r);
			//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(26).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		}
	}

	//display JournalFace
	//r.left=0;	r.top=0;	r.right=r.left+640;	r.bottom=r.top+480;
	//ddObj->ddsb->Blt(&r, ddGfx->JournalFace, &r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
  SDL_RenderCopy(display->renderer, ddGfx->JournalFace->texture, NULL, NULL);

	if(bMain){
		r.y=0;r.x=452;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	} else {
		r.y=0;r.x=504;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	if(bComplete){
		r.y=0;r.x=608;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	} else {
		r.y=0;r.x=556;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	if(bPortals){
		r.y=432;r.x=118;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	if(bTowns){
		r.y=432;r.x=188;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	if(bKeeps){
		r.y=432;r.x=257;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	if(bQuests){
		r.y=432;r.x=327;
    SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(49).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(49).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	
	//display journal text
	MaxTextOffset=0;
	if(curEntry>-1){
		strcpy(str,&JournalText[journalIndex[curEntry]].Title[0]);
		text.drawText(display->renderer,426,82,0,str);
		sprintf(str,"Diff:%d Size:%d",JournalText[journalIndex[curEntry]].Difficulty,journalIndex.size());
		text.drawText(display->renderer,426,98,0,str);
		strcpy(str,"");
		for(i=0;i<JournalText[journalIndex[curEntry]].TextCount;i++){
			strcat(str,&JournalText[journalIndex[curEntry]].Text->at(i)[0]);
			strcat(str,"\n\n");
		}
    r2.x=426,r2.w=188; r2.y=130; r2.h=220;
    SDL_RenderSetClipRect(display->renderer, &r2);
		LineCount=text.drawText(display->renderer,426,130-TextOffset,23,str);
    SDL_RenderSetClipRect(display->renderer, NULL);
		MaxTextOffset=LineCount*16-220;
    //SDL_RenderCopy(display->renderer, ddGfx->TextSurface->texture, &r, &r2);
		//ddObj->ddsb->Blt(&r2, ddGfx->TextSurface, &r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}

	//draw buttons
  if(TextOffset<MaxTextOffset) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &clickList[6]);
  //ddObj->ddsb->Blt(&clickList[6], ddGfx->Windows, &ddGfx->aWindows[12], DDBLT_WAIT, NULL);
  if(TextOffset>0) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &clickList[5]); 
  //ddObj->ddsb->Blt(&clickList[5], ddGfx->Windows, &ddGfx->aWindows[11], DDBLT_WAIT, NULL);

	//display the mouse cursor
	r.y=diObj->MouseY()-16;
	r.x=diObj->MouseX()-16;
	r.h=32;
	r.w=32;
  SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	return true;

}

void cDA2Journal::SetPlayerPos(int map, int x, int y){
	JournalX=0;
	JournalY=0;
	if(map<=400){
		PlayerY = map/20 * 48 + y/64 +24;
		PlayerX = (map-1)%20 * 48 + x/64 +24;
	} else {
		PlayerY=0;
		PlayerX=0;
	}
	JournalX=PlayerX-204;
	JournalY=PlayerY-212;
	if(JournalX<0) JournalX=0;
	if(JournalY<0) JournalY=0;
	if(JournalX>616) JournalX=616;
	if(JournalY>600) JournalY=600;

}

void cDA2Journal::DrawPortals(){

	//Draw portals
	if(flags[250]==1) box.DrawBox(283-JournalX,701-JournalY,286-JournalX,704-JournalY,60);
	if(flags[251]==1) box.DrawBox(831-JournalX,828-JournalY,834-JournalX,831-JournalY,60);
	if(flags[252]==1) box.DrawBox(97-JournalX,872-JournalY,100-JournalX,875-JournalY,60);
	if(flags[253]==1) box.DrawBox(198-JournalX,787-JournalY,201-JournalX,790-JournalY,60);
	if(flags[254]==1) box.DrawBox(380-JournalX,429-JournalY,383-JournalX,432-JournalY,60);
	if(flags[255]==1) box.DrawBox(93-JournalX,171-JournalY,96-JournalX,174-JournalY,60);
	if(flags[256]==1) box.DrawBox(251-JournalX,205-JournalY,254-JournalX,208-JournalY,60);
	if(flags[257]==1) box.DrawBox(669-JournalX,557-JournalY,672-JournalX,560-JournalY,60);
	if(flags[258]==1) box.DrawBox(317-JournalX,501-JournalY,320-JournalX,504-JournalY,60);
	if(flags[259]==1) box.DrawBox(594-JournalX,481-JournalY,597-JournalX,484-JournalY,60);
	if(flags[57]==2) box.DrawBox(885-JournalX,539-JournalY,888-JournalX,542-JournalY,60);
	if(flags[262]==1) box.DrawBox(795-JournalX,387-JournalY,798-JournalX,390-JournalY,60);
	if(flags[263]==1) box.DrawBox(735-JournalX,865-JournalY,738-JournalX,868-JournalY,60);
	if(flags[264]==1) box.DrawBox(651-JournalX,603-JournalY,654-JournalX,606-JournalY,60);
	if(flags[265]==1) box.DrawBox(495-JournalX,941-JournalY,498-JournalX,944-JournalY,60);
	if(flags[266]==1) box.DrawBox(647-JournalX,603-JournalY,650-JournalX,606-JournalY,60);
	if(flags[267]==1) box.DrawBox(908-JournalX,439-JournalY,911-JournalX,442-JournalY,60);
	if(flags[268]==1) box.DrawBox(643-JournalX,599-JournalY,646-JournalX,602-JournalY,60);
	if(flags[269]==1) box.DrawBox(251-JournalX,627-JournalY,254-JournalX,630-JournalY,60);
	if(flags[270]==1) box.DrawBox(649-JournalX,595-JournalY,652-JournalX,598-JournalY,60);
	if(flags[271]==1) box.DrawBox(777-JournalX,912-JournalY,780-JournalX,915-JournalY,60);
	if(flags[272]==1) box.DrawBox(654-JournalX,599-JournalY,657-JournalX,602-JournalY,60);
	if(flags[273]==1) box.DrawBox(650-JournalX,261-JournalY,563-JournalX,264-JournalY,60);
	
	//Draw lines
	if(flags[250]==1 && flags[251]==1) line.DrawLine(284-JournalX,702-JournalY,832-JournalX,829-JournalY,15);
	if(flags[252]==1 && flags[253]==1) line.DrawLine(98-JournalX,873-JournalY,199-JournalX,788-JournalY,15);
	if(flags[254]==1 && flags[255]==1) line.DrawLine(381-JournalX,430-JournalY,94-JournalX,172-JournalY,15);
	if(flags[256]==1 && flags[257]==1) line.DrawLine(252-JournalX,206-JournalY,670-JournalX,558-JournalY,15);
	if(flags[258]==1 && flags[259]==1) line.DrawLine(318-JournalX,502-JournalY,595-JournalX,482-JournalY,15);
	if(flags[57]==2){
		box.DrawBox(367-JournalX,826-JournalY,370-JournalX,829-JournalY,62);
		line.DrawLine(886-JournalX,540-JournalY,368-JournalX,827-JournalY,15);
	}
	if(flags[262]==1 && flags[263]==1) line.DrawLine(796-JournalX,388-JournalY,736-JournalX,866-JournalY,15);
	if(flags[264]==1 && flags[265]==1) line.DrawLine(652-JournalX,604-JournalY,496-JournalX,942-JournalY,15);
	if(flags[266]==1 && flags[267]==1) line.DrawLine(648-JournalX,604-JournalY,909-JournalX,440-JournalY,15);
	if(flags[268]==1 && flags[269]==1) line.DrawLine(644-JournalX,600-JournalY,252-JournalX,628-JournalY,15);
	if(flags[270]==1 && flags[271]==1) line.DrawLine(651-JournalX,596-JournalY,778-JournalX,913-JournalY,15);
	if(flags[272]==1 && flags[273]==1) line.DrawLine(655-JournalX,600-JournalY,651-JournalX,262-JournalY,15);

	/*
		If flags(250) = 1 Then MSurfBack.DrawCircle 246 - JPX + 239 - JXOffset, 664 - JPY + 240 - JYOffset, 2
    If flags(251) = 1 Then MSurfBack.DrawCircle 794 - JPX + 239 - JXOffset, 791 - JPY + 240 - JYOffset, 2
    If flags(252) = 1 Then MSurfBack.DrawCircle 60 - JPX + 239 - JXOffset, 835 - JPY + 240 - JYOffset, 2
    If flags(253) = 1 Then MSurfBack.DrawCircle 161 - JPX + 239 - JXOffset, 750 - JPY + 240 - JYOffset, 2
    If flags(254) = 1 Then MSurfBack.DrawCircle 343 - JPX + 239 - JXOffset, 392 - JPY + 240 - JYOffset, 2
    If flags(255) = 1 Then MSurfBack.DrawCircle 56 - JPX + 239 - JXOffset, 134 - JPY + 240 - JYOffset, 2
    If flags(256) = 1 Then MSurfBack.DrawCircle 214 - JPX + 239 - JXOffset, 168 - JPY + 240 - JYOffset, 2
    If flags(257) = 1 Then MSurfBack.DrawCircle 632 - JPX + 239 - JXOffset, 520 - JPY + 240 - JYOffset, 2
    If flags(258) = 1 Then MSurfBack.DrawCircle 280 - JPX + 239 - JXOffset, 464 - JPY + 240 - JYOffset, 2
    If flags(259) = 1 Then MSurfBack.DrawCircle 557 - JPX + 239 - JXOffset, 444 - JPY + 240 - JYOffset, 2
    If flags(57) = 2 Then MSurfBack.DrawCircle 848 - JPX + 239 - JXOffset, 502 - JPY + 240 - JYOffset, 2
    If flags(262) = 1 Then MSurfBack.DrawCircle 758 - JPX + 239 - JXOffset, 350 - JPY + 240 - JYOffset, 2
    If flags(263) = 1 Then MSurfBack.DrawCircle 698 - JPX + 239 - JXOffset, 828 - JPY + 240 - JYOffset, 2
    If flags(264) = 1 Then MSurfBack.DrawCircle 614 - JPX + 239 - JXOffset, 566 - JPY + 240 - JYOffset, 2
    If flags(265) = 1 Then MSurfBack.DrawCircle 458 - JPX + 239 - JXOffset, 904 - JPY + 240 - JYOffset, 2
    If flags(266) = 1 Then MSurfBack.DrawCircle 610 - JPX + 239 - JXOffset, 566 - JPY + 240 - JYOffset, 2
    If flags(267) = 1 Then MSurfBack.DrawCircle 871 - JPX + 239 - JXOffset, 402 - JPY + 240 - JYOffset, 2
    If flags(268) = 1 Then MSurfBack.DrawCircle 606 - JPX + 239 - JXOffset, 562 - JPY + 240 - JYOffset, 2
    If flags(269) = 1 Then MSurfBack.DrawCircle 214 - JPX + 239 - JXOffset, 590 - JPY + 240 - JYOffset, 2
    If flags(270) = 1 Then MSurfBack.DrawCircle 612 - JPX + 239 - JXOffset, 558 - JPY + 240 - JYOffset, 2
    If flags(271) = 1 Then MSurfBack.DrawCircle 740 - JPX + 239 - JXOffset, 875 - JPY + 240 - JYOffset, 2
    If flags(272) = 1 Then MSurfBack.DrawCircle 617 - JPX + 239 - JXOffset, 562 - JPY + 240 - JYOffset, 2
    If flags(273) = 1 Then MSurfBack.DrawCircle 613 - JPX + 239 - JXOffset, 224 - JPY + 240 - JYOffset, 2
    
    If flags(250) = 1 And flags(251) = 1 Then MSurfBack.DrawLine 246 - JPX + 239 - JXOffset, 664 - JPY + 240 - JYOffset, 794 - JPX + 239 - JXOffset, 791 - JPY + 240 - JYOffset
    If flags(252) = 1 And flags(253) = 1 Then MSurfBack.DrawLine 60 - JPX + 239 - JXOffset, 835 - JPY + 240 - JYOffset, 161 - JPX + 239 - JXOffset, 750 - JPY + 240 - JYOffset
    If flags(254) = 1 And flags(255) = 1 Then MSurfBack.DrawLine 56 - JPX + 239 - JXOffset, 134 - JPY + 240 - JYOffset, 343 - JPX + 239 - JXOffset, 392 - JPY + 240 - JYOffset
    If flags(256) = 1 And flags(257) = 1 Then MSurfBack.DrawLine 214 - JPX + 239 - JXOffset, 168 - JPY + 240 - JYOffset, 632 - JPX + 239 - JXOffset, 520 - JPY + 240 - JYOffset
    If flags(258) = 1 And flags(259) = 1 Then MSurfBack.DrawLine 280 - JPX + 239 - JXOffset, 464 - JPY + 240 - JYOffset, 557 - JPX + 239 - JXOffset, 444 - JPY + 240 - JYOffset
    If flags(57) = 2 Then
        MSurfBack.DrawCircle 330 - JPX + 239 - JXOffset, 789 - JPY + 240 - JYOffset, 2
        MSurfBack.DrawLine 848 - JPX + 239 - JXOffset, 502 - JPY + 240 - JYOffset, 330 - JPX + 239 - JXOffset, 789 - JPY + 240 - JYOffset
    End If
    If flags(262) = 1 And flags(263) = 1 Then MSurfBack.DrawLine 758 - JPX + 239 - JXOffset, 350 - JPY + 240 - JYOffset, 698 - JPX + 239 - JXOffset, 828 - JPY + 240 - JYOffset
    If flags(264) = 1 And flags(265) = 1 Then MSurfBack.DrawLine 614 - JPX + 239 - JXOffset, 566 - JPY + 240 - JYOffset, 458 - JPX + 239 - JXOffset, 904 - JPY + 240 - JYOffset
    If flags(266) = 1 And flags(267) = 1 Then MSurfBack.DrawLine 610 - JPX + 239 - JXOffset, 566 - JPY + 240 - JYOffset, 871 - JPX + 239 - JXOffset, 402 - JPY + 240 - JYOffset
    If flags(268) = 1 And flags(269) = 1 Then MSurfBack.DrawLine 606 - JPX + 239 - JXOffset, 562 - JPY + 240 - JYOffset, 214 - JPX + 239 - JXOffset, 590 - JPY + 240 - JYOffset
    If flags(270) = 1 And flags(271) = 1 Then MSurfBack.DrawLine 612 - JPX + 239 - JXOffset, 558 - JPY + 240 - JYOffset, 740 - JPX + 239 - JXOffset, 875 - JPY + 240 - JYOffset
    If flags(272) = 1 And flags(273) = 1 Then MSurfBack.DrawLine 617 - JPX + 239 - JXOffset, 562 - JPY + 240 - JYOffset, 613 - JPX + 239 - JXOffset, 224 - JPY + 240 - JYOffset
		*/
}


//******************
// Script Finding
//******************

int cDA2Journal::GetID(int ID){
	int i;
	bool bFound=false;
	for(i=0;i<100;i++){
		if(JournalText[i].ID==ID){
			bFound=true;
			break;
		}
	}
	if(bFound) return i;
	return -1;
}



//******************
// Script Modifiers
//******************

void cDA2Journal::SetID(int index, short value){
	JournalText[index].ID=value;
}

void cDA2Journal::SetDifficulty(int index, short value){
	JournalText[index].Difficulty=value;
}

void cDA2Journal::SetMainQuest(int index, short value){
	JournalText[index].MainQuest=value;
}

void cDA2Journal::SetEndPoint(int index, short x, short y){
	JournalText[index].EndPoint.X=x;
	JournalText[index].EndPoint.Y=y;
}

void cDA2Journal::SetActive(int index, short value){
	JournalText[index].Active=value;
}

void cDA2Journal::SetTitle(int index, char *str){
	JournalText[index].Title=str;
}

void cDA2Journal::AddText(int index, char *str){
	string s=str;
	JournalText[index].Text->push_back(s);
	JournalText[index].TextCount++;
}