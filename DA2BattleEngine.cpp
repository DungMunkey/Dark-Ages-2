#include "DA2BattleEngine.h"

cDA2BattleEngine::cDA2BattleEngine(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	party=NULL;
	ItemList=NULL;
	action = new vector<BattleAnimStruct>;
}

cDA2BattleEngine::~cDA2BattleEngine(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	party=NULL;
	ItemList=NULL;
	delete action;
}

bool cDA2BattleEngine::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cDA2Party *par, cItemController *itm){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((party=par)==NULL) return false;
	if((ItemList=itm)==NULL) return false;
	if(text.Init(gfx)==NULL) return false;
	if(textWin.Init(d,gfx,inp)==NULL) return false;
	if(box.Init(d,gfx)==NULL) return false;
	if(dialog.Init(d,gfx,inp)==NULL) return false;
	if(!MonsterLoad()) return false;
	ListSelection=-1;
	ListOffset=0;

	strcpy(skills[0][0],"MP to HP");
	strcpy(skills[0][1],"Relax Mind");
	strcpy(skills[0][2],"HP to MP");
	strcpy(skills[0][3],"Revitilize");
	strcpy(skills[0][4],"Half Life");
	strcpy(skills[0][5],"Two Spells");

	strcpy(skills[1][0],"Escape");
	strcpy(skills[1][1],"Quick Strike");
	strcpy(skills[1][2],"Cripple Leg");
	strcpy(skills[1][3],"Gouge Eyes");
	strcpy(skills[1][4],"Critical Blow");
	strcpy(skills[1][5],"Flurry");

	strcpy(skills[2][0],"Berserk");
	strcpy(skills[2][1],"Pummel");
	strcpy(skills[2][2],"Catch Breath");
	strcpy(skills[2][3],"Great Swing");
	strcpy(skills[2][4],"Maraud");
	strcpy(skills[2][5],"Annihilate");

	skillTargets[0][0]=1;
	skillTargets[0][1]=1;
	skillTargets[0][2]=1;
	skillTargets[0][3]=1;
	skillTargets[0][4]=3;
	skillTargets[0][5]=5;

	skillTargets[1][0]=6;
	skillTargets[1][1]=3;
	skillTargets[1][2]=3;
	skillTargets[1][3]=3;
	skillTargets[1][4]=3;
	skillTargets[1][5]=1;

	skillTargets[2][0]=1;
	skillTargets[2][1]=3;
	skillTargets[2][2]=1;
	skillTargets[2][3]=4;
	skillTargets[2][4]=5;
	skillTargets[2][5]=5;

	return true;
}

void cDA2BattleEngine::BumpAwareness(int pawnID){
	float a=(float)pawns[pawnID].Intelligence*0.4f;
	if(a>20) a=20.0f;
	pawns[pawnID].Awareness+=a;
	if(pawns[pawnID].Awareness>100) pawns[pawnID].Awareness=100.0f;
}

void cDA2BattleEngine::ClearStack(){
	while(battleStack.Pop()){
		//keep iterating
	}
}

bool cDA2BattleEngine::MonsterLoad(){
	FILE *f;
	int i=0;
	char str[128];
	char *tok;

	f=fopen("Data/mstats.txt","rt");
	while(!feof(f)){
		fgets(str,128,f);
		tok=strtok(str,",\"\n");
		tok=strtok(NULL,",\"\n");
		Monsters[i].Agl=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].Spd=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].Str=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].HP=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].Armor=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].Experience=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].Gold=atoi(tok);
		tok=strtok(NULL,",\"\n");
		strcpy(Monsters[i].Name,tok);
		tok=strtok(NULL,",\"\n");
		Monsters[i].Level=atoi(tok);
		i++;
	}
	fclose(f);

	i=1;
	f=fopen("Data/monscrip.dat","rt");
	while(!feof(f)){
		fscanf(f,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",&Scripts[i].ArenaPrice,&Scripts[i].IsRandom,&Scripts[i].Variable,
			&Scripts[i].Fixed,&Scripts[i].MonsterID[0],&Scripts[i].MonsterID[1],&Scripts[i].MonsterID[2],
			&Scripts[i].MonsterID[3],&Scripts[i].MonsterID[4],&Scripts[i].MonsterID[5],&Scripts[i].MonsterID[6],
			&Scripts[i].BGID,&Scripts[i].Flee,&Scripts[i].SongID);
		i++;
	}
	fclose(f);

	SingleMonster[0].x=80; SingleMonster[0].w=96; SingleMonster[0].y=170; SingleMonster[0].h=96;
  SingleMonster[1].x=176; SingleMonster[1].w=96; SingleMonster[1].y=170; SingleMonster[1].h=96;
  SingleMonster[2].x=272; SingleMonster[2].w=96; SingleMonster[2].y=170; SingleMonster[2].h=96;
  SingleMonster[3].x=80; SingleMonster[3].w=96; SingleMonster[3].y=266; SingleMonster[3].h=96;
  SingleMonster[4].x=176; SingleMonster[4].w=96; SingleMonster[4].y=266; SingleMonster[4].h=96;
  SingleMonster[5].x=272; SingleMonster[5].w=96; SingleMonster[5].y=266; SingleMonster[5].h=96;

	ActionButton[0].x=463; ActionButton[0].w=56; ActionButton[0].y=152; ActionButton[0].h=24;
  ActionButton[1].x=519; ActionButton[1].w=56; ActionButton[1].y=152; ActionButton[1].h=24;
  ActionButton[2].x=575; ActionButton[2].w=56; ActionButton[2].y=152; ActionButton[2].h=24;
  ActionButton[3].x=463; ActionButton[3].w=56; ActionButton[3].y=176; ActionButton[3].h=24;
  ActionButton[4].x=519; ActionButton[4].w=56; ActionButton[4].y=176; ActionButton[4].h=24;
  ActionButton[5].x=575; ActionButton[5].w=56; ActionButton[5].y=176; ActionButton[5].h=24;

  SinglePlayer[0].x=80; SinglePlayer[0].w=96; SinglePlayer[0].y=384; SinglePlayer[0].h=96;
  SinglePlayer[1].x=176; SinglePlayer[1].w=96; SinglePlayer[1].y=384; SinglePlayer[1].h=96;
  SinglePlayer[2].x=272; SinglePlayer[2].w=96; SinglePlayer[2].y=384; SinglePlayer[2].h=96;

  RowMonster[0].x=80; RowMonster[0].w=272; RowMonster[0].y=170; RowMonster[0].h=96;
  RowMonster[1].x=80; RowMonster[1].w=272; RowMonster[1].y=266; RowMonster[1].h=96;

  RowPlayer.x=80; RowPlayer.w=272; RowPlayer.y=384; RowPlayer.h=96;

	AllMonster.x=80; AllMonster.w=272; AllMonster.y=170; AllMonster.h=192;

	return true;
}

bool cDA2BattleEngine::Load(int BattleNum){

	int TotalFoes;
	int MonsterID;
	int i,j;

	//Clear any leftover stack from last battle
	ClearStack();

	//Reset level indicator
	LevelUp[0]=LevelUp[1]=LevelUp[2]=false;

	if(party->Gold<Scripts[BattleNum].ArenaPrice){
		//Display some message about lack of funds for the arena
	}
	
	//Subtract any fees
	party->Gold-=Scripts[BattleNum].ArenaPrice;

	//Set flee flag
	if(Scripts[BattleNum].Flee) CanRun=true;
	else CanRun=false;

	if(Scripts[BattleNum].IsRandom){
		TotalFoes=(rand()%Scripts[BattleNum].Variable+1)+Scripts[BattleNum].Fixed;
	} else {
		TotalFoes=Scripts[BattleNum].Fixed;
	}

	for(i=0;i<TotalFoes;i++){
		if(Scripts[BattleNum].IsRandom){
			j=rand()%100;
			if(j==0) MonsterID=Scripts[BattleNum].MonsterID[0];
			else if(j<=16) MonsterID=Scripts[BattleNum].MonsterID[1];
			else if(j<=33) MonsterID=Scripts[BattleNum].MonsterID[2];
			else if(j<=50) MonsterID=Scripts[BattleNum].MonsterID[3];
			else if(j<=66) MonsterID=Scripts[BattleNum].MonsterID[4];
			else if(j<=83) MonsterID=Scripts[BattleNum].MonsterID[5];
			else MonsterID=Scripts[BattleNum].MonsterID[6];
		} else {
			MonsterID=Scripts[BattleNum].MonsterID[i];
		}

		pawns[i+3].Picture=MonsterID+6;
		pawns[i+3].Agility=Monsters[MonsterID].Agl;
		pawns[i+3].Speed=Monsters[MonsterID].Spd;
		pawns[i+3].Strength=Monsters[MonsterID].Str;
		pawns[i+3].HP=Monsters[MonsterID].HP;
		pawns[i+3].MaxHP=pawns[i+3].HP;
		pawns[i+3].Armor=Monsters[MonsterID].Armor;
		pawns[i+3].Experience=Monsters[MonsterID].Experience;
		pawns[i+3].Gold=Monsters[MonsterID].Gold;
		pawns[i+3].Level=Monsters[MonsterID].Level;
		strcpy(pawns[i+3].Name,Monsters[MonsterID].Name);
		pawns[i+3].Alive=true;
		pawns[i+3].ActionCounter=0;
		pawns[i+3].ModAgl=0;
		pawns[i+3].ModInt=0;
		pawns[i+3].ModSpd=0;
		pawns[i+3].ModStr=0;
		pawns[i+3].ModArmor=0;
		if(i<3){
			pawns[i+3].YPos=1;
			pawns[i+3].XPos=i;
		} else {
			pawns[i+3].YPos=0;
			pawns[i+3].XPos=i-3;
		}
	}

	pawnCount=3+TotalFoes;

	//Add player stats to pawns here
	for(i=0;i<3;i++){
		pawns[i].Picture=i*2;
		pawns[i].ActionCounter=0;
		pawns[i].Agility=party->Players[i].ModAgl;
		pawns[i].Awareness=party->Players[i].CS;
		if(pawns[i].Awareness>100) pawns[i].Awareness=100.0f;
		if(party->Players[i].HP>0) pawns[i].Alive=true;
		else pawns[i].Alive=false;
		pawns[i].Armor=party->Players[i].Armor;
		pawns[i].WeaponBal=ItemList->Items[party->Players[i].Gear.Weapon].WeaponBal;
		pawns[i].WeaponStr=ItemList->Items[party->Players[i].Gear.Weapon].WeaponStr;
		if(pawns[i].WeaponStr<1) pawns[i].WeaponStr=1;
		if(ItemList->Items[party->Players[i].Gear.Weapon].Ranged==0) pawns[i].Ranged=false;
		else pawns[i].Ranged=true;
		pawns[i].BackRow=party->BackRow[i];
		pawns[i].Encumbrance=party->Players[i].Encumbrance;
		pawns[i].HP=party->Players[i].HP;
		pawns[i].MaxHP=party->Players[i].MaxHP;
		pawns[i].Intelligence=party->Players[i].ModInt;
		pawns[i].Level=party->Players[i].Level;
		pawns[i].ModAgl=0;
		pawns[i].ModArmor=0;
		pawns[i].ModInt=0;
		pawns[i].ModSpd=0;
		pawns[i].ModStr=0;
		strcpy(pawns[i].Name,party->Players[i].PlayerName);
		pawns[i].Speed=party->Players[i].ModSpeed;
		pawns[i].Strength=party->Players[i].ModStr;
		pawns[i].MP=party->Players[i].MP;
		pawns[i].MaxMP=party->Players[i].MaxMP;
		pawns[i].XPos=i;
		pawns[i].YPos=2;
		pawns[i].SAP=party->Players[i].SAP;
	}

	//count potions
	HPotion=0;
	MPotion=0;
	for(i=0;i<40;i++){
		if(party->PartyItems[i]==62) HPotion++;
		if(party->PartyItems[i]==60) MPotion++;
	}

	BGNum=Scripts[BattleNum].BGID;
	battleStack.Push(BattleMain);

	return true;
}

bool cDA2BattleEngine::GetNextCharacter(int &pawnID){

	int i;

	//Check for highest value over 300
	float best=0;
	int bestIndex=-1;
	for(i=0;i<pawnCount;i++){
		if(!pawns[i].Alive) continue;
		if(pawns[i].ActionCounter>300 && pawns[i].ActionCounter>best){
			best=pawns[i].ActionCounter;
			bestIndex=i;
		}
	}

	if(best>300){
		pawnID=bestIndex;
		return true;
	}

	//If nobody exceeded 300, increase turn counters
	for(i=0;i<pawnCount;i++){
		if(!pawns[i].Alive) continue;
		pawns[i].ActionCounter+=sqrt((double)(pawns[i].Speed+pawns[i].ModSpd));
	}

	return false;

	/*
    
        For i = 0 To (TotalObjects - 1)
            If BObj(i).Active = 1 And BObj(i).AtCount < 3 And BObj(i).EffectBonus(4) = 0 Then BObj(i).MyTurn = BObj(i).MyTurn + Sqr(BObj(i).info.ModSpeed + BObj(i).EffectBonus(1))
            If BObj(i).MyTurn > 300 Then BObj(i).MyTurn = 300
        Next i
				*/
}

int cDA2BattleEngine::GoBattle(){

  Uint32 curTicks=SDL_GetTicks();
  battleTicks+=curTicks - lastTicks;
  lastTicks=curTicks;
  if(battleTicks > 125){
    battleTicks-=125;
    colorCounter++;
    if(colorCounter > 7) colorCounter=0;
  }

	battleStack.SetStateFirst();
	while(battleStack.Process()!=NoGameStack){
		switch(battleStack.Process()){
		case BattleMain:
			if(battleStack.PeekNext()!=NoGameStack) Render(false);
			else Render(true);
			break;
		case BattleAnim:
			if(!BattleAnimation()) {
				action->erase(action->begin());
				BattleAnimTimer=SDL_GetTicks();
				battleStack.Pop();
			}
			break;
		case BattleSpell:
			SpellRender();
			break;
		case BattleSkill:
			SkillRender();
			break;
		case BattleWon:
			BattleOver(0);
			break;
		case BattleLost:
			BattleOver(LastAlive);
			break;
		case BattleDialog:
			dialog.Render();
			break;
		default:
			break;
		}
		if(!battleStack.SetStateNext()) break;
	}

	//ddObj->ddsp->WaitForVerticalSync();
	//ddObj->dd->WaitForVerticalBlank();
	//ddObj->ddsp->Flip(NULL,DDFLIP_WAIT);
  SDL_SetRenderTarget(display->renderer, NULL);
  SDL_RenderCopy(display->renderer, canvas, NULL, NULL);
  SDL_RenderPresent(display->renderer);
  SDL_SetRenderTarget(display->renderer, canvas);

	battleStack.SetStateLast();
	if(battleStack.Process()!=NoGameStack){
		switch(battleStack.Process()){
		case BattleMain:
			switch(Logic()){
			case 0:
			case 4:
				battleStack.Pop();
				return 0;
			case 2:
				battleStack.Push(BattleLost);
				break;
			case 3:
				battleStack.Push(BattleWon);
				break;
			default:
				break;
			}
			break;
		case BattleAnim:
			break;
		case BattleSpell:
			switch(SpellLogic()){
			case 0:
				battleStack.Pop();
				break;
			case 2:
				battleStack.Pop();
				BattleAnimTimer=SDL_GetTicks();
				battleStack.Push(BattleAnim);
				break;
			default:
				break;
			}
			break;
		case BattleSkill:
			switch(SkillLogic()){
			case 0:
				battleStack.Pop();
				break;
			case 2:
				//battleStack.Pop();
				BattleAnimTimer=SDL_GetTicks();
				//battleStack.Push(BattleAnim);
				break;
			case 4:
				battleStack.Pop();
				return 0;
			default:
				break;
			}
			break;
		case BattleLost:
			if(!textWin.Logic()) {
				while(diObj->CheckLock(KEY_SPACE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
				battleStack.Pop();
				battleStack.Pop();
				return 2;
			}
			break;
		case BattleWon:
			if(!textWin.Logic()) {
        while(diObj->CheckLock(KEY_SPACE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
				battleStack.Pop();
				battleStack.Pop();
				return 3;
			}
			break;
		case BattleDialog:
			if(!dialog.Logic()){
        while(diObj->CheckLock(KEY_SPACE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
				battleStack.Pop();
			}
			break;
		default:
			break;
		}
	}

	return 1;
}

void cDA2BattleEngine::GShot(){
  /*1
  int width=640;
  int height=480;

  char fBuf[80];
  time_t rawtime;
  struct tm* timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (fBuf,80,"img%Y%m%d_%H%M%S.bmp",timeinfo);

  PALETTEENTRY palx[256];

  char str[64];
	char* tok;
	FILE* f2=fopen("da2pal.txt","rt");
	for(int i=0;i<256;i++){
		fgets(str,64,f2);
		tok=strtok(str," \t\n");
		palx[i].peBlue=atoi(tok);
		tok=strtok(NULL," \t\n");
		palx[i].peGreen=atoi(tok);
		tok=strtok(NULL," \t\n");
		palx[i].peRed=atoi(tok);
    palx[i].peFlags=0;
	}
	fclose(f2);

  BITMAPFILEHEADER bh;
	BITMAPINFOHEADER info;
	memset(&bh,0,sizeof(BITMAPFILEHEADER));
	memset(&info,0,sizeof(BITMAPINFOHEADER));

  bh.bfType = 19778;
	bh.bfReserved1 = 0;
	bh.bfReserved2 = 0;
	bh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 307200;
	bh.bfOffBits = 1078;

  info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;	
	info.biBitCount = 8;
	info.biCompression = BI_RGB;	
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;  
	info.biYPelsPerMeter = 0;     
	info.biClrUsed = 0;	
	info.biClrImportant = 0; 

  FILE* f=fopen(fBuf,"wb");
  fwrite(&bh,sizeof(BITMAPFILEHEADER),1,f);
  fwrite(&info,sizeof(BITMAPINFOHEADER),1,f);
  fwrite(&palx,sizeof(PALETTEENTRY),256,f);
  DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
  ddObj->ddsb->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_READONLY, NULL);

  long newsize = height*width;
	BYTE* newbuf = new BYTE[newsize];

  memset(newbuf,0,newsize);
	long bufpos = 0;   
	long newpos = 0;
  for(int y=height-1;y>=0;y--){
    for(int x=0;x<width;x++){
      newbuf[newpos++]=((BYTE*)ddsd.lpSurface)[x+y*width];
    }
  }
  ddObj->ddsb->Unlock(NULL);
  fwrite(newbuf,newsize,1,f);
  fclose(f);

  delete[] newbuf;
  */
}

bool cDA2BattleEngine::Render(bool mouse){
	int i,j;
	SDL_Rect r;
	char str[32];

	//display background
  SDL_RenderClear(display->renderer);
  SDL_RenderCopy(display->renderer, ddGfx->Battle[BGNum]->texture, NULL,NULL);

	//display characters
  
	for(i=0;i<3;i++){
		r.x = 80 + pawns[i].XPos * 96;
		r.y = 384+32*pawns[i].BackRow;
    r.w=96;
    r.h=96;
		if(pawns[i].Alive) {
			//ddObj->ddsb->Blt(&r, ddGfx->Monsters[ddGfx->vMonster->at(pawns[i].Picture).index], &ddGfx->vMonster->at(pawns[i].Picture).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Monsters[ddGfx->vMonster->at(pawns[i].Picture).index]->texture, &ddGfx->vMonster->at(pawns[i].Picture).r, &r);
			
			//Draw any status effects
      r.w=16;
      r.h=16;
			if(pawns[i].ModAgl>0) {
				r.x = 80 + pawns[i].XPos * 96;
				r.y = 464;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[0], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[0], &r);
			}
			if(pawns[i].ModSpd>0){
				r.x = 96 + pawns[i].XPos * 96;
				r.y = 464;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[1], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[1], &r);
			}
			if(pawns[i].ModStr>0){
				r.x = 112 + pawns[i].XPos * 96;
				r.y = 464;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[2], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[2], &r);
			}
			if(pawns[i].ModArmor>0){
				r.x = 128 + pawns[i].XPos * 96;
				r.y = 464;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[3], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[3], &r);
			}
		} else {
			//ddObj->ddsb->Blt(&r, ddGfx->Monsters[ddGfx->vMonster->at(pawns[i].Picture+1).index], &ddGfx->vMonster->at(pawns[i].Picture+1).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Monsters[ddGfx->vMonster->at(pawns[i].Picture + 1).index]->texture, &ddGfx->vMonster->at(pawns[i].Picture + 1).r, &r);
		}
	}

	//display active monsters
	for(i=3;i<pawnCount;i++){
		if(pawns[i].Alive){
			r.x = 80 + pawns[i].XPos * 96;
			r.y = 170 + pawns[i].YPos * 96;	
      r.w=96; 
      r.h=96;
			//ddObj->ddsb->Blt(&r, ddGfx->Monsters[ddGfx->vMonster->at(pawns[i].Picture).index], &ddGfx->vMonster->at(pawns[i].Picture).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Monsters[ddGfx->vMonster->at(pawns[i].Picture).index]->texture, &ddGfx->vMonster->at(pawns[i].Picture).r, &r);

			//Draw any status effects
      r.w=16;
      r.h=16;
			if(pawns[i].ModAgl<0) {
				r.x = 80 + pawns[i].XPos * 96;
				r.y = 250 + pawns[i].YPos * 96;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[4], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[4], &r);
			}
			if(pawns[i].ModSpd<0){
				r.x = 96 + pawns[i].XPos * 96;
				r.y = 250 + pawns[i].YPos * 96;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[5], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[5], &r);
			}
			if(pawns[i].ModStr<0){
				r.x = 112 + pawns[i].XPos * 96;
				r.y = 250 + pawns[i].YPos * 96;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[6], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[6], &r);
			}
			if(pawns[i].ModArmor<0){
				r.x = 128 + pawns[i].XPos * 96;
				r.y = 250 + pawns[i].YPos * 96;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[7], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[7], &r);
			}
			if(pawns[i].ActionCounter<0){
				r.x = 144 + pawns[i].XPos * 96;
				r.y = 250 + pawns[i].YPos * 96;
				//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0], &ddGfx->aBattleStatus[8], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->aBattleStatus[8], &r);
			}

			//draw monster name
			text.drawText(display->renderer,470,370+(i-3)*15,0,pawns[i].Name);

			//draw monster health bar
			box.DrawBox(580,374+(i-3)*15,625,382+(i-3)*15,15);
			if(pawns[i].HP>0)	box.DrawBox(581,375+(i-3)*15,581+(int)((float)43*(float)(pawns[i].HP/(float)pawns[i].MaxHP)),381+(i-3)*15,70,true);
		}
	}

	//Display player stats
	r.x=460; r.w=32;
	for(i=0;i<3;i++){
		if(i==0) {
			r.y=15; r.h=32;
			//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(i*16).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(i * 16).r, &r);
			sprintf(str,"H: %d/%d\nM: %d/%d\nA:",pawns[i].HP,pawns[i].MaxHP,pawns[i].MP,pawns[i].MaxMP);
			text.drawText(display->renderer,495,15,0,str);
			box.DrawBox(520,50,622,60,15);
			box.DrawBox(521,51,521+(int)pawns[i].Awareness,59,70,true);

		} else {
			r.y=(i-1)*36+66; r.h=32;
			//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(i*16).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(i * 16).r, &r);
			sprintf(str,"H: %d/%d\nA:",pawns[i].HP,pawns[i].MaxHP);
			text.drawText(display->renderer,495,(i-1)*36+66,0,str);
			box.DrawBox(520,(i-1)*36+85,622,(i-1)*36+95,15);
			box.DrawBox(521,(i-1)*36+86,521+(int)pawns[i].Awareness,(i-1)*36+94,70,true);
		}

	}

	//Show potion counts
	sprintf(str,"%d",HPotion);
  if(strlen(str)<2) text.drawText(display->renderer, 498, 180, 0, str);
  else text.drawText(display->renderer, 494, 180, 0, str);
	sprintf(str,"%d",MPotion);
  if(strlen(str)<2) text.drawText(display->renderer, 554, 180, 0, str);
  else text.drawText(display->renderer, 550, 180, 0, str);


	//Highlight BattleActor
	if(BattleActor>-1 && BattleActor<3){
		box.DrawBox(80+pawns[BattleActor].XPos*96,384,176+pawns[BattleActor].XPos*96,480,80,false,colorCounter);

		//highlight target monster
		if(mouse){
			for(i=0;i<6;i++){
        if(checkMouse(SingleMonster[i])){
					for(j=3;j<pawnCount;j++){
						if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){
							box.DrawBox(80+pawns[j].XPos*96,170+pawns[j].YPos*96,176+pawns[j].XPos*96,266+pawns[j].YPos*96,136,false,colorCounter);
              box.DrawBox(468, 371 + (j - 3) * 15, 627, 385 + (j - 3) * 15, 136, false, colorCounter);
						}
					}
				}
			}
		}

	} else if (BattleActor>2){
		box.DrawBox(80+pawns[BattleActor].XPos*96,170+pawns[BattleActor].YPos*96,176+pawns[BattleActor].XPos*96,266+pawns[BattleActor].YPos*96,80,false,colorCounter);
	}


	//display mouse cursor
  /*1
	if(mouse){
		r.top=diObj->MouseY() - ddGfx->CursorY[7];
		r.left=diObj->MouseX() - ddGfx->CursorX[7];
		r.bottom=r.top+24;
		r.right=r.left+16;
		ddObj->ddsb->Blt(&r, ddGfx->Cursor,&ddGfx->aCursor[7],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
  */

	return true;
}

int cDA2BattleEngine::Logic(){
	int currentPawn;
	int targetPawn;
	int damage;
	int i,j;
	int attackCount;
	bool bClick;
	bool bOpen;
	bool allDead;
	double d;
	double multiAttack;
	BattleAnimStruct bas;
	
	diObj->pollEvents();

	//=========================
	//Check for end of battle
	//=========================
	CheckDeath();
	allDead=true;
	for(i=0;i<3;i++) if(pawns[i].Alive) allDead=false;
	if(allDead){
		//end battle - players dead
		return 2;
	}

	allDead=true;
	for(i=3;i<pawnCount;i++) if(pawns[i].Alive) allDead=false;
	if(allDead){
		//end battle - monsters dead
		CalcRewards();
		return 3;
	}

	//Get next active pawn
	while(!GetNextCharacter(currentPawn)){
		//do nothing
	}

	BattleActor=currentPawn;

	//Do monster turn
	if(currentPawn>2){
		//If monster can attack, then monster does attack
		if(MonsterAttack(currentPawn,targetPawn,damage)){
			
			//Setup animation
			//A whiff has 0 damage
			if(damage==0)	bas.damage[0].graphic=0;
			else bas.damage[0].graphic=1;
			bas.damage[0].effect=damage;
			bas.damage[0].pawn=targetPawn;
			bas.targetCount=1;

			action->push_back(bas);
			BattleAnimTimer=SDL_GetTicks();
			battleStack.Push(BattleAnim);
			pawns[targetPawn].HP-=damage;
			if(pawns[targetPawn].HP<0) pawns[targetPawn].HP=0;
		
		//Otherwise, monster tries to move
		} else {
			
			for(i=0;i<3;i++){
				bOpen=true;
				for(j=3;j<pawnCount;j++){
					if(j==currentPawn) continue;
					if(pawns[j].YPos==1 && pawns[j].XPos==i && pawns[j].Alive) bOpen=false;
				}
				if(bOpen) {
					pawns[currentPawn].YPos=1;
					pawns[currentPawn].XPos=i;
					break;
				}
			}

		}
		pawns[currentPawn].ActionCounter-=300;

	//Do player turn
	} else {

		if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
			diObj->LockKey(KEY_ESCAPE);
			return 0;
		}
    if(diObj->KeyPress(KEY_G) && !diObj->CheckLock(KEY_G)){
      diObj->LockKey(KEY_G);
      GShot();
    }
		
		if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
			//pawns[currentPawn].ActionCounter-=300;
			diObj->LockMouse(0);
			//return true;


			//==========================
			//Check for Attack
			//==========================

			if(pawns[currentPawn].Ranged) i=0;
			else i=3;

			bClick=false;
			for(i=i;i<6;i++){
        if(diObj->MouseX()>SingleMonster[i].x && diObj->MouseX() <= SingleMonster[i].x+SingleMonster[i].w &&
           diObj->MouseY()>SingleMonster[i].y && diObj->MouseY() <= SingleMonster[i].y+SingleMonster[i].h){
					bClick=true;
					break;
				}
			}

			bool bSwing=false;
			if(bClick){
				//return 0;
				switch(i) {
				case 0:
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive && pawns[j].XPos==0 && pawns[j].YPos==0){
							PlayerAttack(currentPawn,j,damage);
							bSwing=true;
							break;
						}
					}
					break;
				case 1:
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive && pawns[j].XPos==1 && pawns[j].YPos==0){
							PlayerAttack(currentPawn,j,damage);
							bSwing=true;
							break;
						}
					}
					break;
				case 2:
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive && pawns[j].XPos==2 && pawns[j].YPos==0){
							PlayerAttack(currentPawn,j,damage);
							bSwing=true;
							break;
						}
					}
					break;
				case 3:
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive && pawns[j].XPos==0 && pawns[j].YPos==1){
							PlayerAttack(currentPawn,j,damage);
							bSwing=true;
							break;
						}
					}
					break;
				case 4:
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive && pawns[j].XPos==1 && pawns[j].YPos==1){
							PlayerAttack(currentPawn,j,damage);
							bSwing=true;
							break;
						}
					}
					break;
				case 5:
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive && pawns[j].XPos==2 && pawns[j].YPos==1){
							PlayerAttack(currentPawn,j,damage);
							bSwing=true;
							break;
						}
					}
					break;
				default:
					break;
				}

				//Setup animation
				//A whiff has 0 damage
				if(bSwing){
					if(damage==0)	bas.damage[0].graphic=0;
					else bas.damage[0].graphic=1;
					bas.damage[0].effect=damage;
					bas.damage[0].pawn=j;
					bas.targetCount=1;
					action->push_back(bas);
					battleStack.Push(BattleAnim);
					
					
					pawns[j].HP-=damage;
					if(pawns[j].HP<0)pawns[j].HP=0;

					//multiattack for thief
					if(currentPawn==1) {

						attackCount=0;
						multiAttack = (0.02 * (float)(pawns[currentPawn].Agility+pawns[currentPawn].ModAgl))-1;
						while(multiAttack>1){
							attackCount++;
							multiAttack-=1;
						}
						d=(double)rand();
						d/=RAND_MAX;
						if(d<multiAttack) attackCount++;

						while(attackCount>0){
							PlayerAttack(currentPawn,j,damage);
							if(damage==0)	bas.damage[0].graphic=0;
							else bas.damage[0].graphic=1;
							bas.damage[0].effect=damage;
							bas.damage[0].pawn=j;
							bas.targetCount=1;
							action->push_back(bas);
							battleStack.Push(BattleAnim);

							pawns[j].HP-=damage;
							if(pawns[j].HP<0)pawns[j].HP=0;
							attackCount--;
						}
					}

					BumpAwareness(currentPawn);
					pawns[currentPawn].ActionCounter-=300;

					BattleAnimTimer=SDL_GetTicks();

				}

			}

			//==========================
			//Check for Action Buttons
			//==========================

			bClick=false;
			for(i=0;i<6;i++){
        if(checkMouse(ActionButton[i])){
					bClick=true;
					break;
				}
			}

			if(bClick){
				switch(i){
				case 0:
					if(currentPawn!=0) return true;
					battleStack.Push(BattleSpell);
					break;
				case 1:
					//push skill state
					skillPawn=currentPawn;
					battleStack.Push(BattleSkill);
					break;
				case 2:
					//push pass/defend state
					BumpAwareness(currentPawn);
					pawns[currentPawn].ActionCounter-=300;
					break;
				case 3:	//push health potion
					if(HPotion==0) return true;
					HPotion--;
					for(j=39;j>=0;j--){
						if(party->PartyItems[j]==62){
							party->PartyItems[j]=0;
							break;
						}
					}
					pawns[currentPawn].HP+=(pawns[currentPawn].MaxHP/2);
					if(pawns[currentPawn].HP>pawns[currentPawn].MaxHP) pawns[currentPawn].HP=pawns[currentPawn].MaxHP;
					pawns[currentPawn].ActionCounter-=300;
					break;
				case 4: //push magic potion
					if(currentPawn!=0 || MPotion==0) return true;
					MPotion--;
					for(j=39;j>=0;j--){
						if(party->PartyItems[j]==60){
							party->PartyItems[j]=0;
							break;
						}
					}
					j=pawns[currentPawn].MaxMP/4;
					if(j>50) j=50;
					pawns[currentPawn].MP+=j;
					if(pawns[currentPawn].MP>pawns[currentPawn].MaxMP) pawns[currentPawn].MP=pawns[currentPawn].MaxMP;
					pawns[currentPawn].ActionCounter-=300;
					break;
				case 5:
					//push run state
					if(!CanRun) {
						//dialog
						dialog.SetWindow(198,155,"You cannot flee from this battle!",false);
						battleStack.Push(BattleDialog);
						return true;
					} else {
						if( ((int)(100.0*rand()/(RAND_MAX+1.0))) < 50 ) {
							//flee successful
							CalcRewards(true);
							return 4;
						} else {
							for(j=0;j<3;j++) if(pawns[j].HP>0) pawns[j].ActionCounter-=300;
							pawns[currentPawn].ActionCounter-=300;
						}
					}
					break;
				default:
					break;
				}
			}


			//==========================
			//Check for Player Movement
			//==========================

			bClick=false;
			for(i=0;i<3;i++){
        if(checkMouse(SinglePlayer[i])){
					bClick=true;
					break;
				}
			}

			if(bClick){
				if(currentPawn!=i) return true;
				if(pawns[currentPawn].BackRow) pawns[currentPawn].BackRow=false;
				else pawns[currentPawn].BackRow=true;
				BumpAwareness(currentPawn);
				pawns[currentPawn].ActionCounter-=300;
			}


		} //end mouse press

	} //end player turn

	//Check if only one player left alive
	if(pawns[0].Alive && !pawns[1].Alive && !pawns[2].Alive){
		LastAlive=1;
	} else if(pawns[1].Alive && !pawns[0].Alive && !pawns[2].Alive){
		LastAlive=2;
	} else if(pawns[2].Alive && !pawns[0].Alive && !pawns[1].Alive){
		LastAlive=3;
	} else {
		LastAlive=0;
	}

	return 1;
}
	
bool cDA2BattleEngine::BattleAnimation(){

	int i;
	int x,y;
	char str[8];
	SDL_Rect r;

	//Time battle animations. This function returns true until a second has passed
	//This means BattleAnimTimer must be set prior to calling this function
	Uint32 ElapsedTime=SDL_GetTicks()-BattleAnimTimer;
	if(ElapsedTime>1000 || ElapsedTime<0)	{
		return false;
	}

	for(i=0;i<action->at(0).targetCount;i++){

		//Get message to display
		if(action->at(0).damage[i].effect==0) strcpy(str,"Miss!");
		else sprintf(str,"%d",action->at(0).damage[i].effect);

		switch(action->at(0).damage[i].graphic){
		case 0: //whiff

			//animate quickly for first half second or so
			x=pawns[action->at(0).damage[i].pawn].XPos*96+130-(strlen(str)*4);
			if(ElapsedTime<252 && ElapsedTime>0)	y=pawns[action->at(0).damage[i].pawn].YPos*96+242-(int)(ElapsedTime/6);
			else y=pawns[action->at(0).damage[i].pawn].YPos*96+200;
      text.drawText(display->renderer, x + 1, y + 1, 0, str, 2);
      text.drawText(display->renderer, x, y, 0, str);

			break;
		case 1: //regular damage
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:

			//show attack icon
			r.x=pawns[action->at(0).damage[i].pawn].XPos*96+114; 
			r.y=pawns[action->at(0).damage[i].pawn].YPos*96+192; 
			r.w=32; 
			r.h=32;
			//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(action->at(0).damage[i].graphic).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(action->at(0).damage[i].graphic).r, &r);

			//animate quickly for first half second or so
			x=pawns[action->at(0).damage[i].pawn].XPos*96+130-(strlen(str)*4);
			if(ElapsedTime<252 && ElapsedTime>0)	y=pawns[action->at(0).damage[i].pawn].YPos*96+242-(int)(ElapsedTime/6);
			else y=pawns[action->at(0).damage[i].pawn].YPos*96+200;
      text.drawText(display->renderer, x + 1, y + 1, 0, str, 2);
      text.drawText(display->renderer, x, y, 0, str, 0);
			
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			//show attack icon
			r.x=pawns[action->at(0).damage[i].pawn].XPos*96+114; 
			r.y=pawns[action->at(0).damage[i].pawn].YPos*96+192; 
			r.w=32; 
			r.h=32;
			//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(action->at(0).damage[6].graphic).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
      SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(action->at(0).damage[6].graphic).r, &r);
			break;
			
		default: //do nothing
			break;
		}
	}

	return true;
}

bool cDA2BattleEngine::MonsterAttack(int monster, int &target, int &damage){

	double dHitPercent;
	int iHitPercent;
	double BaseDam;
	double RandDam;
	double TotalDam;
	double Defense;
	int i;

	//if monster cannot attack, return false
	if(pawns[monster].YPos!=1) return false;

	//find player target
	while(true){
		//i=rand()%100;
		i=(int)(100.0*rand()/(RAND_MAX+1.0));
		if(i<15){
			if(pawns[0].Alive) {
				target=0;
				break;
			}
		} else if(i<45){
			if(pawns[1].Alive) {
				target=1;
				break;
			}
		} else {
			if(pawns[2].Alive){
				target=2;
				break;
			}
		}
	}

	//Base hit percentage
	dHitPercent = (60-pawns[monster].Level*4) + pawns[monster].Agility + pawns[monster].ModAgl;

	//Subtract player agility
	dHitPercent -= ((double)(pawns[target].Agility+pawns[target].ModAgl)/2.0);

	//Add player encumbrance
	dHitPercent += ((double)pawns[target].Encumbrance/((double)(pawns[target].Strength+pawns[target].ModStr)/4.0));

	iHitPercent = (int)(dHitPercent+0.5);
	if(iHitPercent>95) iHitPercent=95;
	if(iHitPercent<25) iHitPercent=25;

	if( ((int)(100.0*rand()/(RAND_MAX+1.0))) < iHitPercent ) {
		BaseDam = (double)(pawns[monster].Strength + pawns[monster].ModStr) / 2.0;
		RandDam = (BaseDam*rand()/(RAND_MAX+1.0));
	  TotalDam = BaseDam+RandDam;

		Defense = (pawns[target].Armor + pawns[target].ModArmor) / (pawns[monster].Level*40);
		Defense *= damage;
		damage = (int)(TotalDam-Defense+0.5);
		
		//half the damage for being in the back row.
		if(pawns[target].BackRow) damage=(int)(((double)damage/2.0)+0.5);
		
		if(damage<1) damage=1;
		
	} else {
		damage=0;
	}

	return true;

}

bool cDA2BattleEngine::PlayerAttack(int player, int monster, int &damage){

	int iHitPercent;
	double BaseDam;
	double RandDam;
	double MaxDam;
	int Range;
	double TotalDam;

	//Base hit percentage
	iHitPercent = 80 + ((pawns[player].Level - pawns[monster].Level*3)*3) + pawns[player].WeaponBal;
	if(iHitPercent<5) iHitPercent=5;

	if( (rand()%100)<iHitPercent ) {

		if(player==2) MaxDam = (double)(pawns[player].WeaponStr * (pawns[player].Strength + pawns[player].ModStr)) / 20.0 + 1.0;
		else MaxDam = (double)(pawns[player].WeaponStr * (pawns[player].Strength + pawns[player].ModStr)) / 30.0 + 1.0;
		if(MaxDam<1.01) MaxDam=1.01;

		BaseDam = MaxDam*0.85;
		if(BaseDam<1.01) BaseDam=1.01;
		Range = (int)(MaxDam+0.5) - (int)(BaseDam);
		RandDam = (Range*rand()/(RAND_MAX+1.0));
		TotalDam = (int)BaseDam + RandDam;
		
		//half the damage for being in the back row unless using ranged weapon.
		if(pawns[player].BackRow && !pawns[player].Ranged) TotalDam/=2;

		damage=(int)(TotalDam+0.5);
		if(damage<1) damage=1;
		
	} else {
		damage=0;
	}

	return true;

}

void cDA2BattleEngine::CheckDeath(){
	int i;
	for(i=0;i<pawnCount;i++){
		if(pawns[i].HP<1) pawns[i].Alive=false;
		else pawns[i].Alive=true;
	}
}


bool cDA2BattleEngine::SpellRender(){
	int i,j;
	int count;
	char str[12];
	SDL_Rect r;

	int spellNum=party->SpellList[ListSelection+ListOffset];

	if(ListSelection>=0){
		box.DrawBox(464,232+(ListSelection*16),616,248+(ListSelection*16),1,true);
	}
	
	count=0;
	for(i=ListOffset;i<party->SpellCount;i++){
		sprintf(str,"%d MP",party->Spells[party->SpellList[i]].MPCost);
		if(pawns[0].MP >= party->Spells[party->SpellList[i]].MPCost){
			//Have enough MP, draw bright spellname
      text.drawText(display->renderer, 466, 232 + count * 16, 0, party->Spells[party->SpellList[i]].SpellName);
      text.drawText(display->renderer, 576, 232 + count * 16, 0, str);
		} else {
			//Not enough MP, draw grayed out
      text.drawText(display->renderer, 466, 232 + count * 16, 0, party->Spells[party->SpellList[i]].SpellName, 1);
      text.drawText(display->renderer, 576, 232 + count * 16, 0, str, 1);
		}
		count++;
		if(count>=8) break;
	}

	//Add scroll bars if list is bigger than window
	r.x=616;
	r.w=16;
	if(ListOffset>0) { //draw up scrollbar
		r.y=216;
		r.h=16;
		//ddObj->ddsb->Blt(&r, ddGfx->Windows,&ddGfx->aWindows[11],DDBLT_WAIT,NULL);
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &r);

	} 
	if(count<party->SpellCount) { //draw down scrollbar
		r.y=344;
		r.h=16;
		//ddObj->ddsb->Blt(&r, ddGfx->Windows,&ddGfx->aWindows[12],DDBLT_WAIT,NULL);
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &r);
	} 

	//Highlight ListSelection
	if(ListSelection>=0){


		//Check if target(s) was selected
		switch(party->Spells[spellNum].Type){
		case 1: //one player
		case 2: //one player, battle only
			for(i=0;i<3;i++){
        if(checkMouse(SinglePlayer[i])){
					if(!pawns[i].Alive && (spellNum==25 || spellNum==26)){
            box.DrawBox(SinglePlayer[i], 88, false, colorCounter);
					} else if(pawns[i].Alive && spellNum!=25 && spellNum!=26) {
            box.DrawBox(SinglePlayer[i], 88, false, colorCounter);
					}
				}
			}
			break;
		case 3: //one monster
			for(i=0;i<6;i++){
        if(checkMouse(SingleMonster[i])) {
					for(j=3;j<pawnCount;j++){
						if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){
              box.DrawBox(SingleMonster[i], 88, false, colorCounter);
              box.DrawBox(468, 371 + (j - 3) * 15, 627, 385 + (j - 3) * 15, 136, false, colorCounter);
						}
					}
				}
			}
			break;
		case 4: //row monsters
			for(i=0;i<2;i++){
        if(checkMouse(RowMonster[i])) {
					for(j=3;j<pawnCount;j++){
						if(pawns[j].YPos==i && pawns[j].Alive){
              box.DrawBox(RowMonster[i], 88, false, colorCounter);
							break;
						}
					}
				}
			}
			break;
		case 5: //all monsters
      if(checkMouse(AllMonster)) {
        box.DrawBox(AllMonster, 88, false, colorCounter);
			}
			break;
		case 6: //all players
      if(checkMouse(RowPlayer)) {
        box.DrawBox(RowPlayer, 88, false, colorCounter);
			}
			break;
		case 7: //dead players?
			for(i=0;i<3;i++){
        if(checkMouse(SinglePlayer[i]) && !pawns[i].Alive) {
          box.DrawBox(SinglePlayer[i], 88, false, colorCounter);
				}
			}
			break;
		default:
			break;
		}

	}

	//display mouse cursor
  /*1
	r.y=diObj->MouseY() - ddGfx->CursorY[7];
	r.x=diObj->MouseX() - ddGfx->CursorX[7];
	r.h=24;
	r.w=16;
	ddObj->ddsb->Blt(&r, ddGfx->Cursor,&ddGfx->aCursor[7],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
  */

	return true;
}

int cDA2BattleEngine::SpellLogic(){
	int i,j;
	BattleAnimStruct bas;
	int damage, graphic, stat;
	int spellNum=party->SpellList[ListSelection+ListOffset];

	diObj->pollEvents();

	//Check if canceled (right mouse click or ESC)
	if(diObj->MousePress(1) && !diObj->CheckLockMouse(1))	{
		diObj->LockMouse(1);
		return 0;
	}
	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)) {
		diObj->LockKey(KEY_ESCAPE);
		return 0;
	}

	
	//Check user click
	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		diObj->LockMouse(0);
		//FILE *f=fopen("BattleSpell.txt","wt");
		//fprintf(f,"%d,%d,%d,%d\n",diObj->MouseX(),diObj->MouseY(),ListSelection,party->Spells[ListSelection+ListOffset].Type);
		//fclose(f);

		//Check if scroll bars selected
		if(ListOffset>0 && diObj->MouseX()>616 && diObj->MouseX()<=632 && diObj->MouseY()>=216 && diObj->MouseY()<=232){ //up
			ListOffset--;
			ListSelection=-1;
			return 1;
		}
		if(ListOffset+8<party->SpellCount && diObj->MouseX()>616 && diObj->MouseX()<=632 && diObj->MouseY()>=344 && diObj->MouseY()<=360){ //down
			ListOffset++;
			ListSelection=-1;
			return 1;
		}


		//Check if list is selected;
		if(diObj->MouseX()>462 && diObj->MouseX()<=616 && diObj->MouseY()>=232 && diObj->MouseY()<=360){

			i=(diObj->MouseY()-232)/16;
			if(pawns[0].MP>=party->Spells[party->SpellList[i+ListOffset]].MPCost) ListSelection=i; //+ListOffset;
			return 1;

		}
			
		if(ListSelection>=0){

			if(party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost>pawns[0].MP) return 1;

			//Check if target(s) was selected
			switch(party->Spells[spellNum].Type){
			case 1: //one player
			case 2:
				for(i=0;i<3;i++){
          if(checkMouse(SinglePlayer[i])) {

						if(pawns[i].Alive && spellNum!=25 && spellNum!=26){

							PlayerSpell(spellNum,damage,graphic,i,stat);
							bas.damage[0].graphic=graphic;
							bas.damage[0].effect=damage;
							bas.damage[0].pawn=i;
							bas.targetCount=1;
							action->push_back(bas);
							switch(stat){
								case 0: pawns[i].HP+=damage; break;
								case 1: pawns[i].ModStr+=damage; break;
								case 2: pawns[i].ModAgl+=damage; break;
								case 3: pawns[i].ModInt+=damage; break;
								case 4: pawns[i].ModSpd+=damage; break;
								case 5: pawns[i].ModArmor+=damage; break;
								case 6: pawns[i].ActionCounter+=damage; break;
								default: break;
							}
						} else if(!pawns[i].Alive && (spellNum==25 || spellNum==26)) {

							PlayerSpell(spellNum,damage,graphic,i,stat);
							bas.damage[0].graphic=graphic;
							bas.damage[0].effect=damage;
							bas.damage[0].pawn=i;
							bas.targetCount=1;
							action->push_back(bas);
							pawns[i].Alive=true;
							pawns[i].HP+=damage;

						} else {
							continue;
						}
						pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
						if(pawns[i].HP>pawns[i].MaxHP)pawns[i].HP=pawns[i].MaxHP;
						if(pawns[0].ActionCounter>=300) pawns[0].ActionCounter-=300;
						ListSelection=-1;
						BumpAwareness(0);
						return 2;
					}
				}
				break;
			case 3: //one monster
				for(i=0;i<6;i++){
          if(checkMouse(SingleMonster[i])) {
						for(j=3;j<pawnCount;j++){
							if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){
								PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic,j,stat);
								bas.damage[0].graphic=graphic;
								bas.damage[0].effect=damage;
								bas.damage[0].pawn=j;
								bas.targetCount=1;

								action->push_back(bas);
								switch(stat){
								case 0: pawns[j].HP-=damage; break;
								case 1: pawns[j].ModStr-=damage; break;
								case 2: pawns[j].ModAgl-=damage; break;
								case 3: pawns[j].ModInt-=damage; break;
								case 4: pawns[j].ModSpd-=damage; break;
								case 5: pawns[j].ModArmor-=damage; break;
								case 6: pawns[j].ActionCounter-=damage; break;
								default: break;
								}
								pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
								if(pawns[0].ActionCounter>=300) pawns[0].ActionCounter-=300;
								ListSelection=-1;

								BumpAwareness(0);

								return 2;
							}
						}
					}
				}
				break;
			case 4: //row monsters
				for(i=0;i<2;i++){
          if(checkMouse(RowMonster[i])) {
						bas.targetCount=0;
						for(j=3;j<pawnCount;j++){
							if(pawns[j].YPos==i && pawns[j].Alive){
								PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic,j,stat);
								bas.damage[bas.targetCount].graphic=graphic;
								bas.damage[bas.targetCount].effect=damage;
								bas.damage[bas.targetCount].pawn=j;
								bas.targetCount++;
								switch(stat){
								case 0: pawns[j].HP-=damage; break;
								case 1: pawns[j].ModStr-=damage; break;
								case 2: pawns[j].ModAgl-=damage; break;
								case 3: pawns[j].ModInt-=damage; break;
								case 4: pawns[j].ModSpd-=damage; break;
								case 5: pawns[j].ModArmor-=damage; break;
								case 6: pawns[j].ActionCounter-=damage; break;
								default: break;
								}
							}
						}
						if(bas.targetCount>0){
							action->push_back(bas);
							pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
							if(pawns[0].ActionCounter>=300) pawns[0].ActionCounter-=300;
							ListSelection=-1;
							BumpAwareness(0);
							return 2;
						}
					}
				}
				break;
			case 5: //all monsters
        if(checkMouse(AllMonster)) {
					bas.targetCount=0;
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive){
							PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic,j,stat);
							bas.damage[bas.targetCount].graphic=graphic;
							bas.damage[bas.targetCount].effect=damage;
							bas.damage[bas.targetCount].pawn=j;
							bas.targetCount++;
							switch(stat){
							case 0: pawns[j].HP-=damage; break;
							case 1: pawns[j].ModStr-=damage; break;
							case 2: pawns[j].ModAgl-=damage; break;
							case 3: pawns[j].ModInt-=damage; break;
							case 4: pawns[j].ModSpd-=damage; break;
							case 5: pawns[j].ModArmor-=damage; break;
							case 6: pawns[j].ActionCounter-=damage; break;
							default: break;
							}
						}
					}
					if(bas.targetCount>0){
						action->push_back(bas);
						pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
						if(pawns[0].ActionCounter>=300) pawns[0].ActionCounter-=300;
						BumpAwareness(0);
						ListSelection=-1;
						return 2;
					}
				}
				break;
			case 6: //all players
        if(checkMouse(RowPlayer)) {
					bas.targetCount=0;
					for(i=0;i<3;i++){
						if(pawns[i].Alive) {
							PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic,i,stat);
							bas.damage[bas.targetCount].graphic=graphic;
							bas.damage[bas.targetCount].effect=damage;
							bas.damage[bas.targetCount].pawn=i;
							bas.targetCount++;
							switch(stat){
							case 0: pawns[i].HP+=damage; break;
							case 1: pawns[i].ModStr+=damage; break;
							case 2: pawns[i].ModAgl+=damage; break;
							case 3: pawns[i].ModInt+=damage; break;
							case 4: pawns[i].ModSpd+=damage; break;
							case 5: pawns[i].ModArmor+=damage; break;
							case 6: pawns[i].ActionCounter+=damage; break;
							default: break;
							}
							if(pawns[i].HP>pawns[i].MaxHP)pawns[i].HP=pawns[i].MaxHP;
						}
					}
					if(bas.targetCount>0){
						action->push_back(bas);
						pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
						if(pawns[0].ActionCounter>=300) pawns[0].ActionCounter-=300;
						BumpAwareness(0);
						ListSelection=-1;
						return 2;
					}
				}
				break;
			case 7: //dead players?
				break;
			default:
				break;
			}

		}

	}//end mouse click

	return 1;

}

bool cDA2BattleEngine::PlayerSpell(int spell, int &effect, int &graphic, int target, int &stat){
	stat=0; //0 = HP, 1=ModStr, 2=ModAgl, 3=ModInt, 4=ModSpeed, 5=ModArmor, 6=ActionCounter, 0 by default
	switch(spell){
	case 0: //burn
		effect = (int)(10 * (pawns[0].Intelligence + pawns[0].ModInt) / 16);
		graphic=2;
		break;
	case 1:
		effect = (int)(10 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=2;
		break;
	case 2:
	case 3: //rain of fire
		effect = (int)(25 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=2;
		break;
	case 4: //shock
		effect = (int)(25 * (pawns[0].Intelligence + pawns[0].ModInt) / 16);
		graphic=5;
		break;
	case 5: //lightning
	case 7: //acid rain
		effect = (int)(40 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=5;
		break;
	case 6: //storm
		effect = (int)(10 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=5;
		break;
	case 8: //blindness
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 8);
		if(effect > (int)(pawns[target].Agility*3/4)) effect = (int)(pawns[target].Agility*3/4);
		graphic = 6;
		stat=2;
		break;
	case 9: //weakness
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 10);
		if(effect > (int)(pawns[target].Strength*3/4)) effect = (int)(pawns[target].Strength*3/4);
		graphic = 6;
		stat=1;
		break;
	case 12: //stop time
	case 10: //paralyze
		effect = 900;
		graphic = 6;
		stat=6;
		break;
	case 11: //wave of fire
		effect = (int)(60 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=2;
		break;
	case 13: //slow
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 16);
		if(effect > (int)(pawns[target].Speed*3/4)) effect = (int)(pawns[target].Speed*3/4);
		graphic = 6;
		stat=4;
		break;
	case 14: //death blast
		break;
	case 27:
		effect = (int)(60 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=5;
		break;
	case 15: //strength
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 8);
		graphic = 6;
		stat = 1;
		break;
	case 16: //haste
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 16);
		graphic = 6;
		stat = 4;
		break;
	case 17: //agility
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 8);
		graphic = 6;
		stat = 2;
		break;
	case 18: //defense
		effect = (int)((pawns[0].Intelligence + pawns[0].ModInt) / 8);
		graphic = 6;
		stat = 5;
		break;
	case 19: //pain
		effect = (int)(40 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=5;
		break;
	case 20: //heal all
		effect = (int)(40 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		if(effect > 300) effect = 300;
		graphic=3;
		break;
	case 21: //heal
		effect = (int)(10 * (pawns[0].Intelligence + pawns[0].ModInt) / 25);
    if(effect > 50) effect = 50;
    graphic=3;
    break;
	case 22: //great heal
		effect = (int)(25 * (pawns[0].Intelligence + pawns[0].ModInt) / 25);
    if(effect > 150) effect = 150;
    graphic=3;
    break;
	case 23: //massive heal
		effect = (int)(50 * (pawns[0].Intelligence + pawns[0].ModInt) / 25);
    if(effect > 350) effect = 350;
    graphic=3;
    break;
	case 24: //full heal
		effect = 5000;
    graphic=3;
    break;
	case 25: //resurrect
		effect = 1;
    graphic=3;
    break;
	case 26: //full revive
		effect = 5000;
    graphic=3;
		break;
	case 28:
		effect = (int)(80 * (pawns[0].Intelligence + pawns[0].ModInt) / 20);
		graphic=5;
		break;
	case 29:
	default:
		break;
	}

	return true;
}

bool cDA2BattleEngine::SkillRender(){
	int i,j;
	
	for(i=0;i<pawns[skillPawn].SAP+1;i++){
		//Show available skills
		if(pawns[skillPawn].Awareness >= (int)(100/(pawns[skillPawn].SAP+1)*(i+1)) ){
			text.drawText(display->renderer,466,232+i*16,0,skills[skillPawn][i]);
		} else {
			//Grey out not enough Awareness
      text.drawText(display->renderer, 466, 232 + i * 16, 0, skills[skillPawn][i], 1);
		}
	}

	//Highlight ListSelection
	if(ListSelection>=0){

		box.DrawBox(464,232+(ListSelection*16),630,248+(ListSelection*16),15);

		//Check if target(s) was selected
		switch(skillTargets[skillPawn][ListSelection]){
		case 1: //one player, current player
			if(checkMouse(SinglePlayer[skillPawn]) && pawns[skillPawn].Alive) {
        box.DrawBox(SinglePlayer[skillPawn], 88, false, colorCounter);
			}
			break;
		case 2: //one player, any player
			for(i=0;i<3;i++){
        if(checkMouse(SinglePlayer[i]) && pawns[i].Alive) {
          box.DrawBox(SinglePlayer[i], 88, false, colorCounter);
				}
			}
			break;
		case 3: //one monster
			if(pawns[skillPawn].Ranged) i=0;
			else i=3;
			for(i=i;i<6;i++){
        if(checkMouse(SingleMonster[i])) {
					for(j=3;j<pawnCount;j++){
						if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){
              box.DrawBox(SingleMonster[i], 88, false, colorCounter);
              box.DrawBox(468, 371 + (j - 3) * 15, 627, 385 + (j - 3) * 15, 136, false, colorCounter);
						}
					}
				}
			}
			break;
		case 4: //row monsters
			if(pawns[skillPawn].Ranged) i=0;
			else i=1;
			for(i=i;i<2;i++){
        if(checkMouse(RowMonster[i])) {
					for(j=3;j<pawnCount;j++){
						if(pawns[j].YPos==i && pawns[j].Alive){
              box.DrawBox(RowMonster[i], 88, false, colorCounter);
							break;
						}
					}
				}
			}
			break;
		case 5: //all monsters
      if(checkMouse(AllMonster)) {
        box.DrawBox(AllMonster, 88, false, colorCounter);
			}
			break;
		case 6: //all players
      if(checkMouse(RowPlayer)) {
        box.DrawBox(RowPlayer, 88, false, colorCounter);
			}
			break;
		default:
			break;
		}

	}

	//display mouse cursor
  /*1
	RECT r;
	r.top=diObj->MouseY() - ddGfx->CursorY[7];
	r.left=diObj->MouseX() - ddGfx->CursorX[7];
	r.bottom=r.top+24;
	r.right=r.left+16;
	ddObj->ddsb->Blt(&r, ddGfx->Cursor,&ddGfx->aCursor[7],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
  */

	return true;
}

int cDA2BattleEngine::SkillLogic(){
	int i,j,k,n;
	int attackCount;
	double d,r;
	BattleAnimStruct bas;
	int damage;

	diObj->pollEvents();

	//Check if canceled (right mouse click or ESC)
	if(diObj->MousePress(1) && !diObj->CheckLockMouse(1))	{
		diObj->LockMouse(1);
		return 0;
	}
	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)) {
		diObj->LockKey(KEY_ESCAPE);
		return 0;
	}

	
	//Check user click
	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		diObj->LockMouse(0);

		//Check if list is selected;
		if(diObj->MouseX()>462 && diObj->MouseX()<632 && diObj->MouseY()>=232 && diObj->MouseY()<=360){

			i=(diObj->MouseY()-232)/16;
			if(pawns[skillPawn].Awareness >= (int)(100/(pawns[skillPawn].SAP+1)*(i+1)) ) ListSelection=i;
			return 1;

		}
			
		if(ListSelection>=0){

			switch(skillPawn){
				
			//Mage skills
			case 0:
				
				switch(ListSelection){

				case 0: //50% MP to HP, Mage only
					if(checkMouse(SinglePlayer[0])) {

						//we got this far, skill has been chosen, no going back.
						battleStack.Pop();

						bas.damage[0].graphic=3;
						bas.damage[0].effect=1;
						bas.damage[0].pawn=0;
						bas.targetCount=1;

						action->push_back(bas);
						battleStack.Push(BattleAnim);

						pawns[0].HP+=pawns[0].MP/2;
						if(pawns[0].HP>pawns[0].MaxHP) pawns[0].HP=pawns[0].MaxHP;
						pawns[0].MP-=pawns[0].MP/2;
						pawns[0].ActionCounter-=300;
						pawns[0].Awareness=0;
						ListSelection=-1;
						return 2;
					}
					break;
				case 1: //+10% MP, Mage only
          if(checkMouse(SinglePlayer[0])) {
						
						//we got this far, skill has been chosen, no going back.
						battleStack.Pop();
						
						n=(int)(pawns[0].MaxMP/10.0+0.5);
						bas.damage[0].graphic=3;
						bas.damage[0].effect=n;
						bas.damage[0].pawn=0;
						bas.targetCount=1;

						action->push_back(bas);
						battleStack.Push(BattleAnim);

						pawns[0].MP+=n;
						if(pawns[0].MP>pawns[0].MaxMP)pawns[0].MP=pawns[0].MaxMP;
						pawns[0].ActionCounter-=300;
						pawns[0].Awareness=0;
						ListSelection=-1;
						return 2;
					}
					break;
				case 2: //50% HP to MP, Mage only
          if(checkMouse(SinglePlayer[0])) {

						//we got this far, skill has been chosen, no going back.
						battleStack.Pop();

						bas.damage[0].graphic=3;
						bas.damage[0].effect=1;
						bas.damage[0].pawn=0;
						bas.targetCount=1;

						action->push_back(bas);
						battleStack.Push(BattleAnim);

						pawns[0].MP+=pawns[0].HP/2;
						if(pawns[0].MP>pawns[0].MaxMP) pawns[0].MP=pawns[0].MaxMP;
						pawns[0].HP-=pawns[0].HP/2;
						pawns[0].ActionCounter-=300;
						pawns[0].Awareness=0;
						ListSelection=-1;
						return 2;
					}
					break;
				case 3: //+25% HP and MP, Mage only
					if(checkMouse(SinglePlayer[0])) {

						//we got this far, skill has been chosen, no going back.
						battleStack.Pop();

						bas.damage[0].graphic=3;
						bas.damage[0].effect=1;
						bas.damage[0].pawn=0;
						bas.targetCount=1;

						action->push_back(bas);
						battleStack.Push(BattleAnim);

						pawns[0].HP+=pawns[0].HP/4;
						if(pawns[0].HP>pawns[0].MaxHP) pawns[0].HP=pawns[0].MaxHP;
						pawns[0].MP+=pawns[0].MP/4;
						if(pawns[0].MP>pawns[0].MaxMP) pawns[0].MP=pawns[0].MaxMP;
						pawns[0].ActionCounter-=300;
						pawns[0].Awareness=0;
						ListSelection=-1;
						return 2;
					}
					break;
				case 4: //Strong Attack (halves monster damage)
					for(i=0;i<6;i++){
						if(checkMouse(SingleMonster[i])) {

							//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

							for(j=3;j<pawnCount;j++){
								if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){
								
									bas.damage[0].graphic=1;
									bas.damage[0].effect=pawns[j].HP/2;
									bas.damage[0].pawn=j;
									bas.targetCount=1;

									action->push_back(bas);
									battleStack.Push(BattleAnim);

									pawns[j].HP-=pawns[j].HP/2;
									pawns[0].ActionCounter-=300;
									pawns[0].Awareness=0;
									ListSelection=-1;

									return 2;
								}
							}
						}
					}
					break;
				case 5: //Two spells
					battleStack.Pop();
					battleStack.Push(BattleSpell);
					battleStack.Push(BattleSpell);
					pawns[0].ActionCounter-=300;
					pawns[0].Awareness=0;
					ListSelection=-1;
					return 1;
					break;
				default:
					break;
				}
				break;
			
			//Thief skills
			case 1:
				switch(ListSelection){
				case 0: //Escape
					//push run state
					if(!CanRun) {
						//dialog
						dialog.SetWindow(198,155,"You cannot flee from this battle!",false);
						ListSelection=-1;
						battleStack.Push(BattleDialog);
						return 1;
					} else {
						pawns[1].Awareness=0;
						CalcRewards(true);
						ListSelection=-1;
						return 4;
					}
					break;
				case 1: //Quick Jabs (2x attack count)
					if(pawns[1].Ranged) i=0;
					else i=3;
					for(i=i;i<6;i++){
						if(checkMouse(SingleMonster[i])){

							//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

							//Determine how many attacks are made (multi-attacks allowed)
							attackCount=0;
							for(j=0;j<2;j++){
								d=0.02*(pawns[1].Agility+pawns[1].ModAgl);
								if(d<1) {
									attackCount++;
									continue;
								}
								while(d>1){
									attackCount++;
									d-=1.0;
								}
								r=rand()%100;
								r/=100;
								if(r<d) attackCount++;
							}

							for(j=3;j<pawnCount;j++){
								if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){

									for(k=0;k<attackCount;k++){

										PlayerAttack(1,j,damage);
										if(damage==0)	bas.damage[0].graphic=0;
										else bas.damage[0].graphic=1;
										bas.damage[0].effect=damage;
										bas.damage[0].pawn=j;
										bas.targetCount=1;
										action->push_back(bas);
										battleStack.Push(BattleAnim);
									
										pawns[j].HP-=damage;
										if(pawns[j].HP<0) pawns[j].HP=0;

									}

									pawns[1].ActionCounter-=300;
									pawns[1].Awareness=0;
									ListSelection=-1;

									return 2;
								}
							}

						}
					}
					break;
				case 2: //Stab leg (Halves monster speed and guaranteed damage)
					if(pawns[1].Ranged) i=0;
					else i=3;
					for(i=i;i<6;i++){
						if(checkMouse(SingleMonster[i])){

							//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

							for(j=3;j<pawnCount;j++){
								if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){

									damage=0;
									while(damage==0) PlayerAttack(1,j,damage);
									bas.damage[0].graphic=1;
									bas.damage[0].effect=damage;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);
									
									pawns[j].HP-=damage;
									if(pawns[j].HP<0) pawns[j].HP=0;

									//apply skill modifier
									bas.damage[0].graphic=6;
									bas.damage[0].effect=(int)(pawns[j].Speed+pawns[j].ModSpd)/2;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);

									pawns[j].ModSpd-=(int)(pawns[j].Speed+pawns[j].ModSpd)/2;
									if(abs(pawns[j].ModSpd)>=pawns[j].Speed) pawns[j].ModSpd=-pawns[j].Speed+1;
									
									pawns[1].ActionCounter-=300;
									pawns[1].Awareness=0;
									ListSelection=-1;

									return 2;
								}
							}

						}
					}
					break;
				case 3: //Gouge Eyes (Halves monster agl and guaranteed damage)
					if(pawns[1].Ranged) i=0;
					else i=3;
					for(i=i;i<6;i++){
						if(checkMouse(SingleMonster[i])){

							//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

							for(j=3;j<pawnCount;j++){
								if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){

									damage=0;
									while(damage==0) PlayerAttack(1,j,damage);
									bas.damage[0].graphic=1;
									bas.damage[0].effect=damage;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);
									
									pawns[j].HP-=damage;
									if(pawns[j].HP<0) pawns[j].HP=0;

									//apply skill modifier
									bas.damage[0].graphic=6;
									bas.damage[0].effect=(int)(pawns[j].Agility+pawns[j].ModAgl)/2;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);

									pawns[j].ModAgl-=(int)(pawns[j].Agility+pawns[j].ModAgl)/2;
									if(abs(pawns[j].ModAgl)>=pawns[j].Agility) pawns[j].ModAgl=-pawns[j].Agility+1;
									
									pawns[1].ActionCounter-=300;
									pawns[1].Awareness=0;
									ListSelection=-1;

									return 2;
								}
							}

						}
					}
					break;

				case 4: //Cripple arm (Halves monster str and guaranteed damage)
					if(pawns[1].Ranged) i=0;
					else i=3;
					for(i=i;i<6;i++){
						if(checkMouse(SingleMonster[i])){

							//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

							for(j=3;j<pawnCount;j++){
								if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){

									damage=0;
									while(damage==0) PlayerAttack(1,j,damage);
									bas.damage[0].graphic=1;
									bas.damage[0].effect=damage;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);
									
									pawns[j].HP-=damage;
									if(pawns[j].HP<0) pawns[j].HP=0;

									//apply skill modifier
									bas.damage[0].graphic=6;
									bas.damage[0].effect=(int)(pawns[j].Strength+pawns[j].ModStr)/2;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);

									pawns[j].ModStr-=(int)(pawns[j].Strength+pawns[j].ModStr)/2;
									if(abs(pawns[j].ModStr)>=pawns[j].Strength) pawns[j].ModStr=-pawns[j].Strength+1;
									
									pawns[1].ActionCounter-=300;
									pawns[1].Awareness=0;
									ListSelection=-1;

									return 2;
								}
							}

						}
					}
					break;

				default:
					break;
				}
				break;

			//Warrior skills
			case 2:
				switch(ListSelection){
				case 0: //Berserk 
					if(checkMouse(SinglePlayer[2])) {
						pawns[2].ModSpd+=(int)(pawns[2].Speed/2);
						pawns[2].ActionCounter-=300;
						pawns[2].Awareness=0;
						ListSelection=-1;
						//no animation yet
						return 0;
					}
					break;

				case 1: //Pummel (2x attack)
					if(pawns[2].Ranged) i=0;
					else i=3;
					for(i=i;i<6;i++){
						if(checkMouse(SingleMonster[i])){

							//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

							for(j=3;j<pawnCount;j++){
								if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){

									PlayerAttack(2,j,damage);
									if(damage==0)	bas.damage[0].graphic=0;
									else bas.damage[0].graphic=1;
									bas.damage[0].effect=damage;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);
									
									pawns[j].HP-=damage;
									if(pawns[j].HP<0)pawns[j].HP=0;

									PlayerAttack(2,j,damage);
									if(damage==0)	bas.damage[0].graphic=0;
									else bas.damage[0].graphic=1;
									bas.damage[0].effect=damage;
									bas.damage[0].pawn=j;
									bas.targetCount=1;
									action->push_back(bas);
									battleStack.Push(BattleAnim);
									BattleAnimTimer=SDL_GetTicks();
									
									pawns[j].HP-=damage;
									if(pawns[j].HP<0)pawns[j].HP=0;

									pawns[2].ActionCounter-=300;
									pawns[2].Awareness=0;
									ListSelection=-1;

									return 2;
								}
							}


						}
					}
					break;

				case 2: //Catch Breath (+25% health)
					if(checkMouse(SinglePlayer[2])) {
						pawns[2].HP+=(int)(pawns[2].MaxHP/4);
						if(pawns[2].HP>pawns[2].MaxHP) pawns[2].HP=pawns[2].MaxHP;
						pawns[2].ActionCounter-=300;
						pawns[2].Awareness=0;
						ListSelection=-1;
						//no animation yet
						return 0;
					}
					break;

				case 3: //Great Swing (attack first row)
					if(checkMouse(RowMonster[1])) {

						//we got this far, skill has been chosen, no going back.
							battleStack.Pop();

						bas.targetCount=0;
						for(j=3;j<pawnCount;j++){
							if(pawns[j].YPos==1 && pawns[j].Alive){
								PlayerAttack(2,j,damage);
								if(damage==0)	bas.damage[bas.targetCount].graphic=0;
								bas.damage[bas.targetCount].graphic=1;
								bas.damage[bas.targetCount].effect=damage;
								bas.damage[bas.targetCount].pawn=j;
								bas.targetCount++;
								pawns[j].HP-=damage;
							}
						}
						if(bas.targetCount>0){
							action->push_back(bas);
							battleStack.Push(BattleAnim);

							pawns[2].ActionCounter-=300;
							pawns[2].Awareness=0;
							ListSelection=-1;
							return 2;
						}
					}
					break;

				case 4: //Maraud (4x random attacks)
          if(checkMouse(AllMonster)) {

						//we got this far, skill has been chosen, no going back.
						battleStack.Pop();

						j=0;  //number of successful hits
						k=0;  //number of attempted hits
						while(j<4){
							
							//if no success after 10000 attempts, success is likely not possible.
							k++;
							if(k==10000) return 1;

							n=rand()%(pawnCount-3)+3;
							if(!pawns[n].Alive) continue;
							if(!pawns[2].Ranged && pawns[n].YPos==0) continue;
							
							PlayerAttack(2,n,damage);
							if(damage==0)	bas.damage[0].graphic=0;
							bas.damage[0].graphic=1;
							bas.damage[0].effect=damage;
							bas.damage[0].pawn=n;
							bas.targetCount=1;
							pawns[n].HP-=damage;
							action->push_back(bas);
							battleStack.Push(BattleAnim);

							j++;
						}
						pawns[2].ActionCounter-=300;
						pawns[2].Awareness=0;
						ListSelection=-1;
						return 2;
					}
					break;

				case 5: //Annihilate (5x random attacks)
					if(checkMouse(AllMonster)) {

						//we got this far, skill has been chosen, no going back.
						battleStack.Pop();

						j=0;  //number of successful hits
						k=0;  //number of attempted hits
						while(j<5){
							
							//if no success after 10000 attempts, success is likely not possible.
							k++;
							if(k==10000) return 1;

							n=rand()%(pawnCount-3)+3;
							if(!pawns[n].Alive) continue;
							if(!pawns[2].Ranged && pawns[n].YPos==0) continue;
							
							PlayerAttack(2,n,damage);
							if(damage==0)	bas.damage[0].graphic=0;
							bas.damage[0].graphic=1;
							bas.damage[0].effect=damage;
							bas.damage[0].pawn=n;
							bas.targetCount=1;
							pawns[n].HP-=damage;
							action->push_back(bas);
							battleStack.Push(BattleAnim);

							j++;
						}
						pawns[2].ActionCounter-=300;
						pawns[2].Awareness=0;
						ListSelection=-1;
						return 2;
					}
					break;

				default:
					break;
				}
				break;

			default:
				break;
			}

			if(party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost>pawns[0].MP) return 1;

			//Check if target(s) was selected
			switch(party->Spells[party->SpellList[ListSelection+ListOffset]].Type){
			case 1: //one player
				for(i=0;i<3;i++){
					if(checkMouse(SinglePlayer[i]) && pawns[i].Alive) {
						/*PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic);
						bas.damage[0].graphic=graphic;
						bas.damage[0].effect=damage;
						bas.damage[0].pawn=i;
						bas.targetCount=1;

						action->push_back(bas);
						pawns[i].HP+=damage;
						pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
						if(pawns[i].HP>pawns[i].MaxHP)pawns[i].HP=pawns[i].MaxHP;
						pawns[0].ActionCounter-=300;
						ListSelection=-1;
						return 2;*/
					}
				}
				break;
			case 2: //one player, battle only
			case 3: //one monster
				for(i=0;i<6;i++){
					if(checkMouse(SingleMonster[i])) {
						/*for(j=3;j<pawnCount;j++){
							if(pawns[j].XPos==(i%3) && pawns[j].YPos==(i/3) && pawns[j].Alive){
								PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic);
								bas.damage[0].graphic=graphic;
								bas.damage[0].effect=damage;
								bas.damage[0].pawn=j;
								bas.targetCount=1;

								action->push_back(bas);
								pawns[j].HP-=damage;
								pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
								pawns[0].ActionCounter-=300;
								ListSelection=-1;

								return 2;
							}
						}*/
					}
				}
				break;
			case 4: //row monsters
				for(i=0;i<2;i++){
					if(checkMouse(RowMonster[i])) {
						/*bas.targetCount=0;
						for(j=3;j<pawnCount;j++){
							if(pawns[j].YPos==i && pawns[j].Alive){
								PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic);
								bas.damage[bas.targetCount].graphic=graphic;
								bas.damage[bas.targetCount].effect=damage;
								bas.damage[bas.targetCount].pawn=j;
								bas.targetCount++;
								pawns[j].HP-=damage;
							}
						}
						if(bas.targetCount>0){
							action->push_back(bas);
							pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
							pawns[0].ActionCounter-=300;
							ListSelection=-1;
							return 2;
						}*/
					}
				}
				break;
			case 5: //all monsters
				if(checkMouse(AllMonster)) {
					/*bas.targetCount=0;
					for(j=3;j<pawnCount;j++){
						if(pawns[j].Alive){
							PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic);
							bas.damage[bas.targetCount].graphic=graphic;
							bas.damage[bas.targetCount].effect=damage;
							bas.damage[bas.targetCount].pawn=j;
							bas.targetCount++;
							pawns[j].HP-=damage;
						}
					}
					if(bas.targetCount>0){
						action->push_back(bas);
						pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
						pawns[0].ActionCounter-=300;
						ListSelection=-1;
						return 2;
					}*/
				}
				break;
			case 6: //all players
				if(checkMouse(RowPlayer)) {
					/*bas.targetCount=0;
					for(i=0;i<3;i++){
						if(pawns[i].Alive) {
							PlayerSpell(party->SpellList[ListSelection+ListOffset],damage,graphic);
							bas.damage[bas.targetCount].graphic=graphic;
							bas.damage[bas.targetCount].effect=damage;
							bas.damage[bas.targetCount].pawn=i;
							bas.targetCount++;
							pawns[i].HP+=damage;
							if(pawns[i].HP>pawns[i].MaxHP)pawns[i].HP=pawns[i].MaxHP;
						}
					}
					if(bas.targetCount>0){
						action->push_back(bas);
						pawns[0].MP-=party->Spells[party->SpellList[ListSelection+ListOffset]].MPCost;
						pawns[0].ActionCounter-=300;
						ListSelection=-1;
						return 2;
					}*/
				}
				break;
			case 7: //dead players?
				break;
			default:
				break;
			}

		}

	}//end mouse click

	return 1;

}

bool cDA2BattleEngine::checkMouse(SDL_Rect& r){
  if(diObj->MouseX() >= r.x && diObj->MouseX() <= r.x+r.w && 
     diObj->MouseY() >= r.y && diObj->MouseY() <= r.y+r.h){
    return true;
  }
  return false;
}

void cDA2BattleEngine::BattleOver(int won){
	char str[512];
	char str2[128];

	strcpy(str,"");
	switch(won){
	case 0:
		strcat(str,"The battle is won!\n\n");

		if(pawns[0].Alive) {
			sprintf(str2,"  %s earned %d experience.",party->Players[0].PlayerName,pXP[0]);
			if(LevelUp[0]) strcat(str2,"  LEVEL UP!\n");
			else strcat(str2,"\n");
			strcat(str,str2);
		}

		if(pawns[1].Alive) {
			sprintf(str2,"  %s earned %d experience.",party->Players[1].PlayerName,pXP[1]);
			if(LevelUp[1]) strcat(str2,"  LEVEL UP!\n");
			else strcat(str2,"\n");
			strcat(str,str2);
		}

		if(pawns[2].Alive) {
			sprintf(str2,"  %s earned %d experience.",party->Players[2].PlayerName,pXP[2]);
			if(LevelUp[2]) strcat(str2,"  LEVEL UP!\n");
			else strcat(str2,"\n");
			strcat(str,str2);
		}

		sprintf(str2,"\nGold Found: %d pieces.",goldWon);
		strcat(str,str2);

		break;
	case 1:
		sprintf(str2,"Laying in pools of blood, the mangled corpses of %s and %s stare at %s apathetically.",party->Players[1].PlayerName,party->Players[2].PlayerName,party->Players[0].PlayerName);
		strcat(str,str2);
		sprintf(str2," With the last of his strength, %s raises his arm to protect his head from the incoming blow.",party->Players[0].PlayerName);
		strcat(str,str2);
		strcat(str," He knows it is pointless, as the fatal blow comes crashing down and fractures his skull.");
		strcat(str," The darkness is instantaneous.");
		break;
	case 2:
		sprintf(str2,"%s's confusion lasted only a moment. She was so careful, like a thief should be.",party->Players[1].PlayerName);
		strcat(str,str2);
		sprintf(str2," How could she have lost. %s lay dead beside her, but she could no longer see where %s fell in the melee.",party->Players[2].PlayerName,party->Players[0].PlayerName);
		strcat(str,str2);
		sprintf(str2," As %s sat rapidly bleeding out, confusion quickly turned to calmness. She could not remember what was troubling her.",party->Players[1].PlayerName);
		strcat(str,str2);
		strcat(str," Her opponents only watched as her eyes closed and she slid into darkness.");
		break;
	case 3:
		sprintf(str2,"%s spat a final curse at his enemies, a small triumph amidst a fatal defeat.",party->Players[2].PlayerName);
		strcat(str,str2);
		sprintf(str2," %s's carcass is unrecognizable, and %s's body is already being dragged away.",party->Players[0].PlayerName,party->Players[1].PlayerName);
		strcat(str,str2);
		sprintf(str2," %s tried one last time to lift his weapon, but it had become too heavy.",party->Players[2].PlayerName);
		strcat(str,str2);
		sprintf(str2," Gritting his teeth against the final attack, %s was suddenly thrust into darkness.",party->Players[2].PlayerName);
		strcat(str,str2);
		break;
	default:
		break;
	}

	party->BackRow[0]=pawns[0].BackRow;
	party->BackRow[1]=pawns[1].BackRow;
	party->BackRow[2]=pawns[2].BackRow;
	textWin.SetWindow(47,10,str);
	textWin.Render();
}

void cDA2BattleEngine::CalcRewards(bool fled){

	int i,j;
	int xp,lv;
	int surv;
	float chkl;

	if(!fled){
		xp=0;	lv=0;	goldWon=0;
		for(i=3;i<pawnCount;i++){
			xp+=pawns[i].Experience;
			lv+=pawns[i].Level;
			goldWon+=pawns[i].Gold;
		}

		surv=0;
		for(i=0;i<3;i++) if(pawns[i].Alive) surv++;

		lv/=(pawnCount-3);
		xp/=surv;
	}

	for(i=0;i<3;i++){
		if(!fled){
			if(pawns[i].Alive){
				j=(lv*3) - pawns[i].Level;
				if(j<-9){
					chkl=0.1f;
				} else if(j<0){
					chkl=1-fabs((double)j*0.1);
				} else if(j==0){
					chkl=1.0f;
				} else {
					chkl = -150*(j+25)/((j+25)*(j+25))+7;
				}

				pXP[i]=(int)(chkl*xp+0.5);
				party->Players[i].Experience+=pXP[i];
				if(party->Players[i].Experience>499){
					party->Players[i].TP+=3;
					party->Players[i].Experience-=500;
					party->Players[i].Level++;
					if(party->Players[i].Level%10==0 && party->Players[i].Level<60) party->Players[i].SAP++;
					LevelUp[i]=true;
				}
			}
		}
		party->Players[i].MP=pawns[i].MP;
		party->Players[i].HP=pawns[i].HP;
		party->Players[i].CS=pawns[i].Awareness;
	
	}

	if(!fled)	party->Gold+=goldWon;

}
		

	