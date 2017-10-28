#include "DA2LoadSave.h"

cDA2LoadSave::cDA2LoadSave(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	player=NULL;
	party=NULL;
	maps=NULL;
	flags=NULL;
	animCounter=NULL;
	journal=NULL;
	options=NULL;
	//Frame=NULL;
}

cDA2LoadSave::~cDA2LoadSave(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	player=NULL;
	party=NULL;
	maps=NULL;
	flags=NULL;
	animCounter=NULL;
	journal=NULL;
	options=NULL;
	//if(Frame!=NULL){
	//	Frame->Release();
	//	Frame=NULL;
	//}
}

bool cDA2LoadSave::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, sPlayer *play, cDA2Party *par, cMap *map, short *flag, int *anim, cDA2Journal *jour, int *speed, cDA2Options *opt){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((player=play)==NULL) return false;
	if((party=par)==NULL) return false;
	if((maps=map)==NULL) return false;
	if((flags=flag)==NULL) return false;
	if((animCounter=anim)==NULL) return false;
	if((journal=jour)==NULL) return false;
	if(!text.Init(gfx)) return false;
	if(!box.Init(display,ddGfx)) return false;
	if((gameSpeed=speed)==NULL) return false;
	if((options=opt)==NULL) return false;

	/*
	DDSURFACEDESC2 ddsdGeneric;
	ZeroMemory(&ddsdGeneric, sizeof(ddsdGeneric));
	ddsdGeneric.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsdGeneric.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsdGeneric.dwHeight = 256;
	ddsdGeneric.dwWidth = 352;
	ddsdGeneric.dwSize = sizeof(ddsdGeneric);
	dd->dd->CreateSurface(&ddsdGeneric, &Frame, NULL);
	dd->LoadBitmapResource(Frame,0,0,"Gfx/loadsave.bmp");

	DDCOLORKEY	ddck;
	ddck.dwColorSpaceLowValue = 0;
  ddck.dwColorSpaceHighValue = 0;
	Frame->SetColorKey(DDCKEY_SRCBLT, &ddck);
	*/

	bQS = ReadHeader("data/save/quicksave.sav",QuickSave);
	bQL = ReadHeader("data/save/quickload.sav",QuickLoad);
	ReadDir();

	Selection = -1;
	Offset = 0;
	Cursor = -1;

	//Load/Save button
	clickList[0].x=225; clickList[0].w=61; clickList[0].y=220; clickList[0].h=24;
	//Delete button
  clickList[1].x=290; clickList[1].w=61; clickList[1].y=220; clickList[1].h=24;
	//Cancel button
  clickList[2].x=355; clickList[2].w=61; clickList[2].y=220; clickList[2].h=24;
	//Scroll Up
	clickList[3].x=468; clickList[3].w=16; clickList[3].y=276; clickList[3].h=16;
	//Scroll Down
  clickList[4].x=468; clickList[4].w=16; clickList[4].y=340; clickList[4].h=16;
	//Selection 1
  clickList[5].x=160; clickList[5].w=304; clickList[5].y=260; clickList[5].h=16;
	//Selection 2
  clickList[6].x=160; clickList[6].w=304; clickList[6].y=276; clickList[6].h=16;
	//Selection 3
  clickList[7].x=160; clickList[7].w=304; clickList[7].y=292; clickList[7].h=16;
	//Selection 4
  clickList[8].x=160; clickList[8].w=304; clickList[8].y=308; clickList[8].h=16;
	//Selection 5
  clickList[9].x=160; clickList[9].w=304; clickList[9].y=324; clickList[9].h=16;
	//Selection 6
  clickList[10].x=160; clickList[10].w=304; clickList[10].y=340; clickList[10].h=16;
	//Selection 6
  clickList[11].x=160; clickList[11].w=304; clickList[11].y=192; clickList[11].h=16;

	time(&loadTime);
	playTime=0;

	return true;
}

bool cDA2LoadSave::Render(bool bLoad){
	SDL_Rect r;
	SDL_Rect r2;
	char str[41];
	int sz;
	int hours, minutes, seconds;
	int i,j;

  //clear the game window
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
  SDL_RenderClear(display->renderer);

	//blt the dialog
	r.x=144;	r.y=112; r.w=352; r.h=256; 
	r2.x=0; r2.y=0; r2.w=352; r2.h=256;
  SDL_RenderCopy(display->renderer, ddGfx->Frame->texture, &r2, &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Frame, &r2, DDBLT_WAIT,NULL);

	//Show quick save or quick load
	if(bLoad){
		if(bQL) {
			strcpy(str,"QuickSave   ");
			strcat(str,QuickLoad.dateStr);
			strcat(str,"    ");
			strncat(str,QuickLoad.descript,10);
			text.drawText(display->renderer,160,260,0,str);
		}
		
	} else {
		strcpy(str,"New Save...");
		text.drawText(display->renderer,160,260,0,str);
	}

	//Highlight selected line if hovering over it
	for(i=5;i<12;i++){
    if(diObj->CheckMousePos(clickList[i])){
      box.DrawBox(clickList[i], 1, true);
		}
	}

	//Show file lists
	j=0;
	for(i=Offset;i<vFiles.size() && j<5;i++){
		sprintf(str,"Save%.3d %s %s %.9s...",vFiles[i].ID,vFiles[i].dateStr,vFiles[i].timeStr,vFiles[i].descript);
		text.drawText(display->renderer,160,276+j*16,0,str);
		j++;
	}

	//Add scroll buttons if needed
  if(Offset>0) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &clickList[3]);
  //ddObj->ddsb->Blt(&clickList[3], ddGfx->Windows, &ddGfx->aWindows[11], DDBLT_WAIT | DDBLT_KEYSRC, NULL);
  if(i<vFiles.size()) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &clickList[4]);
  //ddObj->ddsb->Blt(&clickList[4], ddGfx->Windows, &ddGfx->aWindows[12], DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//blt any info, if needed
	if(Selection>-1){
		r.x=308; r.w=32; r.y=128; r.h=32;
    SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(*animCounter).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(*animCounter).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		r.x=372; r.w=32; r.y=128; r.h=32;
    SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(*animCounter + 16).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(*animCounter+16).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		r.x=436; r.w=32; r.y=128; r.h=32;
    SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(*animCounter + 32).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(*animCounter+32).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

		sprintf(str,"Save%.3d",CurrentInfo.ID);
		text.drawText(display->renderer,160,128,0,str);
		text.drawText(display->renderer,160,144,0,CurrentInfo.dateStr);
		text.drawText(display->renderer,236,144,0,CurrentInfo.timeStr);
		text.drawText(display->renderer,160,160,0,"Play Time:");
		hours=CurrentInfo.playTime/3600;
		minutes=(CurrentInfo.playTime-hours*3600)/60;
		seconds=CurrentInfo.playTime-hours*3600-minutes*60;
		sprintf(str,"%d:%.2d:%.2d",hours,minutes,seconds);
		text.drawText(display->renderer,168,176,0,str);
		text.drawText(display->renderer,160,192,0,CurrentInfo.descript);
		sz = sprintf(str,"%d/%d",CurrentInfo.HP[0][0],CurrentInfo.HP[0][1]);
		text.drawText(display->renderer,324-sz*4,160,0,str);
		sz = sprintf(str,"%d/%d",CurrentInfo.HP[1][0],CurrentInfo.HP[1][1]);
		text.drawText(display->renderer,388-sz*4,160,0,str);
		sz = sprintf(str,"%d/%d",CurrentInfo.HP[2][0],CurrentInfo.HP[2][1]);
		text.drawText(display->renderer,452-sz*4,160,0,str);
		sz = sprintf(str,"%d/%d",CurrentInfo.MP[0],CurrentInfo.MP[1]);
		text.drawText(display->renderer,324-sz*4,176,0,str);
		sz = sprintf(str,"-/-");
		text.drawText(display->renderer,388-sz*4,176,0,str);
		text.drawText(display->renderer,452-sz*4,176,0,str);

		if(Cursor>-1 && (*animCounter==1 || *animCounter==2))	text.drawText(display->renderer,156+8*Cursor,192,0,"|");
    if(Selection>0)	SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[20], &clickList[1]);
    //ddObj->ddsb->Blt(&clickList[1], ddGfx->Windows, &ddGfx->aWindows[20], DDBLT_WAIT | DDBLT_KEYSRC, NULL);

		if(bLoad){
      SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[17], &clickList[0]);
			//ddObj->ddsb->Blt(&clickList[0], ddGfx->Windows,&ddGfx->aWindows[17],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		} else {
      SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[18], &clickList[0]);
			//ddObj->ddsb->Blt(&clickList[0], ddGfx->Windows,&ddGfx->aWindows[18],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		}
	}

	//Always show the cancel button
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[19], &clickList[2]);
	//ddObj->ddsb->Blt(&clickList[2], ddGfx->Windows,&ddGfx->aWindows[19],DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	//display the mouse cursor
	r.y=diObj->MouseY()-6;
	r.x=diObj->MouseX()-2;
	r.h=24;
	r.w=16;
  SDL_RenderCopy(display->renderer, ddGfx->Cursor->texture, &ddGfx->aCursor[7], &r);
  //ddObj->ddsb->Blt(&r, ddGfx->Cursor,&ddGfx->aCursor[7],DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	//sprintf(str,"%d,%d,%d",player->Map,player->X,player->Y);
	//text.drawText(display->renderer,10,400,0,str);

	return true;
}

int cDA2LoadSave::Logic(bool bLoad){
	int i,j;
	int ID;
	bool bClick;
	bool bFindID;
	string str,str2;
	char c;

	diObj->pollEvents();

	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
		diObj->LockKey(KEY_ESCAPE);
		Selection=-1;
		return 0;
	}

  //This needs to be fixed!!!
	if(Cursor>-1){
		str=CurrentInfo.descript;
		for(i=0;i<=100;i++){
      if(diObj->KeyPress((DA2KEYS)i) && !diObj->CheckLock((DA2KEYS)i)){
        diObj->LockKey((DA2KEYS)i);
        c=DIKtoASCII((DA2KEYS)i);
				if(c>=97 && c<=122)	{
					if(diObj->KeyPress(KEY_LSHIFT) || diObj->KeyPress(KEY_RSHIFT)) c-=32;
					if(strlen(CurrentInfo.descript)==40) continue;
					if(Cursor==strlen(CurrentInfo.descript)){
						str+=c;
						strcpy(CurrentInfo.descript,&str[0]);
					} else {
						str2=str.substr(Cursor,40);
						str=str.substr(0,Cursor);
						str+=c;
						str+=str2;
						strcpy(CurrentInfo.descript,&str[0]);
					}
					Cursor++;
				} else if(c>=32 && c<=57) { //numbers
					if(strlen(CurrentInfo.descript)==40) continue;
					if(Cursor==strlen(CurrentInfo.descript)){
						str+=c;
						strcpy(CurrentInfo.descript,&str[0]);
					} else {
						str2=str.substr(Cursor,40);
						str=str.substr(0,Cursor);
						str+=c;
						str+=str2;
						strcpy(CurrentInfo.descript,&str[0]);
					}
					Cursor++;
				} else if (c==8){ //backspace
					if(Cursor==strlen(CurrentInfo.descript)){
						CurrentInfo.descript[strlen(CurrentInfo.descript)-1]='\0';
					  str=CurrentInfo.descript;
					} else {
						str2=str.substr(Cursor,40);
						str=str.substr(0,Cursor-1);
						str+=str2;
						strcpy(CurrentInfo.descript,&str[0]);
					}
					Cursor--;
					if(Cursor<0) Cursor=0;
				}
			}
		}
		if(diObj->KeyPress(KEY_DELETE) && !diObj->CheckLock(KEY_DELETE) && Cursor<strlen(CurrentInfo.descript)){
			diObj->LockKey(KEY_DELETE);
			str2=str.substr(Cursor+1,40);
			str=str.substr(0,Cursor);
			str+=str2;
			strcpy(CurrentInfo.descript,&str[0]);
		}
		if(diObj->KeyPress(KEY_LEFT) && !diObj->CheckLock(KEY_LEFT) && Cursor>0) {
			Cursor--;
			diObj->LockKey(KEY_LEFT);
		}
		if(diObj->KeyPress(KEY_RIGHT) && !diObj->CheckLock(KEY_RIGHT) && Cursor<strlen(CurrentInfo.descript))	{
			Cursor++;
			diObj->LockKey(KEY_RIGHT);
		}
	}

	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		for(i=0;i<12;i++){
      if(diObj->MouseX() >= clickList[i].x && diObj->MouseX() <= clickList[i].x+clickList[i].w &&
         diObj->MouseY() >= clickList[i].y && diObj->MouseY() <= clickList[i].y+clickList[i].h){
				bClick=true;
				diObj->LockMouse(0);
				break;
			}
		}

		if(bClick){
			switch(i){
			case 0:
				if(bLoad){
					LoadGame(CurrentInfo.ID);
					playTime=CurrentInfo.playTime;
					time(&loadTime);
					Selection=-1;
					return 2;
				} else {
					SaveGame(CurrentInfo.ID);
					Selection=-1;
					Cursor=0;
					ReadDir();
					return 0;
				}
				break;
			case 2:
				Selection=-1;
				return 0;
				break;
			case 3:
				if(Offset>0) Offset--;
				break;
			case 4:
				if(Offset+5<vFiles.size()) Offset++;
				break;
			case 5:
				Selection=0;
				if(bLoad){
				} else {
					ID=1;
					while(true){
						bFindID=false;
						for(j=0;j<vFiles.size();j++){
							if(vFiles[j].ID==ID) {
								ID++;
								bFindID=true;
								break;
							}
						}
						if(!bFindID) break;
					}
					time(&saveTime);
					CurrentInfo.ID=ID;
					_strdate(CurrentInfo.dateStr);
					_strtime(CurrentInfo.timeStr);
					strcpy(CurrentInfo.descript,"");
					CurrentInfo.playTime=playTime+(int)difftime(saveTime,loadTime);
					for(j=0;j<3;j++){
						CurrentInfo.HP[j][0]=party->Players[j].HP;
						CurrentInfo.HP[j][1]=party->Players[j].MaxHP;
					}
					CurrentInfo.MP[0]=party->Players[0].MP;
					CurrentInfo.MP[1]=party->Players[0].MaxMP;
					Cursor=0;
				}
				break;
			case 6:
				if(Offset<vFiles.size()) {
					Selection=1;
					CurrentInfo=vFiles[Offset];
					if(!bLoad) Cursor=strlen(CurrentInfo.descript);
				}
				break;
			case 7:
				if(Offset+1<vFiles.size()) {
					Selection=2;
					CurrentInfo=vFiles[Offset+1];
					if(!bLoad) Cursor=strlen(CurrentInfo.descript);
				}
				break;
			case 8:
				if(Offset+2<vFiles.size()) {
					Selection=3;
					CurrentInfo=vFiles[Offset+2];
					if(!bLoad) Cursor=strlen(CurrentInfo.descript);
				}
				break;
			case 9:
				if(Offset+3<vFiles.size()) {
					Selection=4;
					CurrentInfo=vFiles[Offset+3];
					if(!bLoad) Cursor=strlen(CurrentInfo.descript);
				}
				break;
			case 10:
				if(Offset+4<vFiles.size()) {
					Selection=5;
					CurrentInfo=vFiles[Offset+4];
					if(!bLoad) Cursor=strlen(CurrentInfo.descript);
				}
				break;
			case 11:
				Cursor=(diObj->MouseX()-160)/8;
				if(Cursor>strlen(CurrentInfo.descript)) Cursor=strlen(CurrentInfo.descript);
				break;
			default:
				Cursor=-1;
				break;
			}
		}

	}


	return 1;
}

void cDA2LoadSave::ReadDir(){

	//struct _finddata_t savFile;
  DIR* dir_p;
  dirent* de;
  //long hFile;
	string path;
	//string newpath;
	SaveInfo si;
  
	vFiles.clear();

	//Change directory to the saves directory

  dir_p = opendir("Saves");
  if(dir_p==NULL) {
    mkdir("Saves");
    dir_p = opendir("Saves");
  }

  while(dir_p){
    de=readdir(dir_p);
    if(de!=NULL){
      if(strstr(de->d_name,".sav")!=NULL){
        path="Saves/";
        path+=de->d_name;
        if(ReadHeader(&path[0], si)) vFiles.push_back(si);
      } else {
      }
    } else {
      break;
    }
  }
  closedir(dir_p);

  /*
	//Get first file
  if( (hFile = _findfirst( "*.sav", &savFile )) == -1L ) {
		//Go back to game directory
		_chdir("..");
		return;
	} else {
		//Get header info from file
		if(ReadHeader(savFile.name,si)) vFiles.push_back(si);

    //Get the rest of the files
    while( _findnext( hFile, &savFile ) == 0 ) {
			if(ReadHeader(savFile.name,si)) vFiles.push_back(si);
    }

    _findclose( hFile );
	}

	//Go back to game directory
	_chdir("..");
  */

}

bool cDA2LoadSave::ReadHeader(char *file, SaveInfo &sav){
	FILE *f;
	int version;

	f=fopen(file,"rb");
	if(f==NULL) return false;

	fread(&version,4,1,f);
	fread(&sav,sizeof(SaveInfo),1,f);
	fclose(f);

	return true;

}

bool cDA2LoadSave::SaveGame(int index){
	FILE *f;
	int i,j,k,m,n;
	char str[256];

	time(&saveTime);
	_strdate(CurrentInfo.dateStr);
	_strtime(CurrentInfo.timeStr);
	CurrentInfo.playTime=playTime+(int)difftime(saveTime,loadTime);
	for(j=0;j<3;j++){
		CurrentInfo.HP[j][0]=party->Players[j].HP;
		CurrentInfo.HP[j][1]=party->Players[j].MaxHP;
	}
	CurrentInfo.MP[0]=party->Players[0].MP;
	CurrentInfo.MP[1]=party->Players[0].MaxMP;

	sprintf(str,"Saves/save%.3d.sav",index);

	f=fopen(str,"wb");
	i=1;
	fwrite(&i,4,1,f); //version number
	fwrite(&CurrentInfo,sizeof(SaveInfo),1,f);
	fwrite(&party->Players,sizeof(cPlayerCharacter),3,f);
	fwrite(&party->PartyItems,4,40,f);
	fwrite(&party->Gold,4,1,f);
	fwrite(&flags[0],2,300,f);
	//for(i=0;i<300;i++) fwrite(&flags[i],2,1,f);
	fwrite(&player->Map,4,1,f);
	fwrite(&player->X,4,1,f);
	fwrite(&player->Y,4,1,f);
	fwrite(gameSpeed,4,1,f);
	fwrite(&party->Spells[0],sizeof(Spell),30,f);
	fwrite(&party->SpellCount,4,1,f);
	fwrite(&party->SpellList,4,30,f);
	for(i=0;i<500;i++){
		fwrite(&maps[i].NPCCount,4,1,f);
		for(j=0;j<maps[i].NPCCount;j++) fwrite(&maps[i].NPCArray[j],sizeof(NPCType),1,f);
	}
	for(i=0;i<500;i++){
		k=maps[i].ObjArray->size();
		fwrite(&k,4,1,f);
		for(j=0;j<k;j++) fwrite(&maps[i].ObjArray->at(j),sizeof(ObjectType),1,f);
	}
	for(i=0;i<100;i++){
		fwrite(&journal->JournalText[i].ID,2,1,f);
		fwrite(&journal->JournalText[i].Difficulty,2,1,f);
		fwrite(&journal->JournalText[i].MainQuest,2,1,f);
		fwrite(&journal->JournalText[i].EndPoint,sizeof(PointType),1,f);
		fwrite(&journal->JournalText[i].Active,2,1,f);
		fwrite(&journal->JournalText[i].TextCount,2,1,f);
		k=journal->JournalText[i].Title.size();
		fwrite(&k,4,1,f);
		for(j=0;j<k;j++)fwrite(&journal->JournalText[i].Title[j],1,1,f);
		k=journal->JournalText[i].Text->size();
		fwrite(&k,4,1,f);
		for(j=0;j<k;j++){
			n=journal->JournalText[i].Text->at(j).size();
			fwrite(&n,4,1,f);
			for(m=0;m<n;m++) fwrite(&journal->JournalText[i].Text->at(j)[m],1,1,f);
		}
	}

	for(i=0;i<3;i++){
		fwrite(&party->BackRow[i],1,1,f);
	}
	fwrite(&options->Options,sizeof(OptionsStruct),1,f);

	fclose(f);
	return true;
}

bool cDA2LoadSave::LoadGame(int index){
	FILE *f;
	bool b;
	int i,j,k,m,n;
	int version;
	char str[256];
	char c;
	string s;
	ObjectType o;
	//FILE *f2;

	sprintf(str,"Saves/save%.3d.sav",index);
	

	f=fopen(str,"rb");
	//f2=fopen("load.txt","wt");
	//fclose(f2);

	fread(&version,4,1,f);
	fread(&CurrentInfo,sizeof(SaveInfo),1,f);
	fread(&party->Players,sizeof(cPlayerCharacter),3,f);
	//party->Players[0].TP=100;
	//party->Players[1].TP=100;
	//party->Players[2].TP=100;

	//f2=fopen("load.txt","at");
	//for(i=0;i<3;i++) fprintf(f2,"%d %s\n",party->Players[i].MaxHP,party->Players[i].PlayerName);
	//fclose(f2);

	fread(&party->PartyItems,4,40,f);

	//f2=fopen("load.txt","at");
	//for(i=0;i<40;i++) fprintf(f2,"%d\n",party->PartyItems[i]);
	//fclose(f2);

	fread(&party->Gold,4,1,f);
	//party->Gold=50000;

	//f2=fopen("load.txt","at");
	//fprintf(f2,"%d\n",party->Gold);
	//fclose(f2);

	//for(i=0;i<300;i++) fread(&flags[i],2,1,f);
	fread(&flags[0],2,300,f);

	FILE* f2=fopen("flaggers.txt","wt");
	for(i=0;i<300;i++) {
	//	if(flags[i]>10) flags[i]=0;
		fprintf(f2,"%d: %d\n",i,flags[i]);
	}
	fclose(f2);

	//f2=fopen("load.txt","at");
	//for(i=0;i<300;i++) fprintf(f2,"%d\n",flags[i]);
	//fclose(f2);

	fread(&player->Map,4,1,f);

	//f2=fopen("load.txt","at");
	//fprintf(f2,"%d\n",player->Map);
	//fclose(f2);

	fread(&player->X,4,1,f);

	//f2=fopen("load.txt","at");
	//fprintf(f2,"%d\n",player->X);
	//fclose(f2);

	fread(&player->Y,4,1,f);

	fread(gameSpeed,4,1,f);
	fread(&party->Spells[0],sizeof(Spell),30,f);
	fread(&party->SpellCount,4,1,f);
	fread(&party->SpellList,4,30,f);

	//f2=fopen("load.txt","at");
	//fprintf(f2,"%d\n",player->Y);
	//fclose(f2);

	for(i=0;i<500;i++){
		fread(&maps[i].NPCCount,4,1,f);
		for(j=0;j<maps[i].NPCCount;j++) fread(&maps[i].NPCArray[j],sizeof(NPCType),1,f);
	}
	for(i=0;i<500;i++){
		maps[i].ObjArray->clear();
		fread(&k,4,1,f);
		for(j=0;j<k;j++) {
			fread(&o,sizeof(ObjectType),1,f);
			maps[i].ObjArray->push_back(o);
		}
	}
	for(i=0;i<100;i++){
		fread(&journal->JournalText[i].ID,2,1,f);
		fread(&journal->JournalText[i].Difficulty,2,1,f);
		fread(&journal->JournalText[i].MainQuest,2,1,f);
		fread(&journal->JournalText[i].EndPoint,sizeof(PointType),1,f);
		fread(&journal->JournalText[i].Active,2,1,f);
		fread(&journal->JournalText[i].TextCount,2,1,f);
		fread(&k,4,1,f); //read title length
		journal->JournalText[i].Title="";
		for(j=0;j<k;j++){
			fread(&c,1,1,f);
			if(c!='\n') journal->JournalText[i].Title+=c;
		}
		journal->JournalText[i].Text->clear();
		fread(&k,4,1,f); //read number of text entries
		for(j=0;j<k;j++){
			s="";
			fread(&n,4,1,f); //read text entry length
			for(m=0;m<n;m++) {
				fread(&c,1,1,f);
				s+=c;
			}
			journal->JournalText[i].Text->push_back(s);
		}
	}

	f2=fopen("journal.txt","wt");
	for(i=0;i<100;i++) {
		fprintf(f2,"%d: %hd %hd\n",i,journal->JournalText[i].ID, journal->JournalText[i].Difficulty);
		fprintf(f2,"%s\n",&journal->JournalText[i].Title[0]);
		if(journal->JournalText[i].MainQuest==1) fprintf(f2,"Main\n");
		else fprintf(f2,"Side\n");
		fprintf(f2,"%hd, %hd\n",journal->JournalText[i].EndPoint.X,journal->JournalText[i].EndPoint.Y);
		if(journal->JournalText[i].Active==1) fprintf(f2,"In Use\n");
		else fprintf(f2,"Unused\n");
		fprintf(f2,"Text Count: %hd %d\n",i,journal->JournalText[i].TextCount,journal->JournalText[i].Text->size());
		for(j=0;j<journal->JournalText[i].Text->size();j++){
			fprintf(f2,"%s\n",&journal->JournalText[i].Text->at(j)[0]);
		}
		fprintf(f2,"\n");
	}
	fclose(f2);

	if(version>0){
		for(i=0;i<3;i++){
			fread(&b,1,1,f);
			party->BackRow[i]=b;
		}
		fread(&options->Options,sizeof(OptionsStruct),1,f);
		*gameSpeed=options->Options.GameSpeed;
	}

	fclose(f);
	journal->Reset();
	return true;
}

char cDA2LoadSave::DIKtoASCII(DA2KEYS dik){
	switch(dik){
		case KEY_BACKSPACE: return 8;
		case KEY_SPACE: return ' ';
		case KEY_PERIOD: return '.';
		case KEY_COMMA: return ',';
		case KEY_MINUS: return '-';
		case KEY_0: return '0';
		case KEY_1: return '1';
		case KEY_2: return '2';
		case KEY_3: return '3';
		case KEY_4: return '4';
		case KEY_5: return '5';
		case KEY_6: return '6';
		case KEY_7: return '7';
		case KEY_8: return '8';
		case KEY_9: return '9';
		case KEY_A: return 'a';
		case KEY_B: return 'b';
		case KEY_C: return 'c';
		case KEY_D: return 'd';
		case KEY_E: return 'e';
		case KEY_F: return 'f';
		case KEY_G: return 'g';
		case KEY_H: return 'h';
		case KEY_I: return 'i';
		case KEY_J: return 'j';
		case KEY_K: return 'k';
		case KEY_L: return 'l';
		case KEY_M: return 'm';
		case KEY_N: return 'n';
		case KEY_O: return 'o';
		case KEY_P: return 'p';
		case KEY_Q: return 'q';
		case KEY_R: return 'r';
		case KEY_S: return 's';
		case KEY_T: return 't';
		case KEY_U: return 'u';
		case KEY_V: return 'v';
		case KEY_W: return 'w';
		case KEY_X: return 'x';
		case KEY_Y: return 'y';
		case KEY_Z: return 'z';
		default:
			break;
	}
	return 0;
}