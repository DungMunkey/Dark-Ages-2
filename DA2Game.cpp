#include "DA2Game.h"

bool cDA2Game::Init(CDisplay* d, cDA2Gfx *gfx, cDA2Input *inp, cItemController *itm, cDA2Music *mus, sConf* con){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((ItemList=itm)==NULL) return false;
	if((MusicObj=mus)==NULL) return false;

  colorCounter=0;
	animCounter=0;
	animator=0;
	PlayerCam.SetCamera(400,320);
	Player.X=400;
	Player.Y=320;
	Player.Map=22;
	Player.imageOffset=0;
	Player.dir=0;
	Player.frame=0;
	renderSpeed=8;
	renderCounter=1;
	RoofOff=false;
	bBorder=true;
	//1 AnimTimer=timeGetTime();
	//1 NPCTimer=timeGetTime();
	bAnimateNow=false;
	playerRender=0;
	text.Init(gfx);
	textWindow.Init(d,gfx,inp);
	box.Init(d,gfx);
	dialog.Init(d,gfx,inp);
	listDialog.Init(d,gfx,inp);
	trainDialog.Init(d,gfx,inp);
	castDialog.Init(d,gfx,inp);
	Party.Init(d,gfx,inp,itm,&colorCounter);
	shop.Init(d,gfx,inp,&Party,itm);
	journal.Init(d,gfx,inp,&Flags[0]);
	options.Init(d,gfx,inp);
  options.SetConf(con);
  options.Update();
	loadSave.Init(d,gfx,inp,&Player,&Party,&Maps[0],&Flags[0],&animCounter,&journal,&renderSpeed,&options);
	intro.Init(d,gfx,inp,&Party,&animCounter);
	battleEngine.Init(d,gfx,inp,&Party,itm);
  conf=con;
	

	mainStack.Push(GameRender);

	Cursor[0][0]=7;	Cursor[1][0]=0;	Cursor[2][0]=1;
	Cursor[0][1]=6; Cursor[1][1]=7; Cursor[2][1]=2;
	Cursor[0][2]=5; Cursor[1][2]=4; Cursor[2][2]=3;

	listDialog.SetWindow(198,155);
	listDialog.AddItem("Resume Game");
	listDialog.AddItem("Save Game");
	listDialog.AddItem("Load Game");
	listDialog.AddItem("Options");
	listDialog.AddItem("View Keyboard Commands");
	listDialog.AddItem("Exit Game");

	castDialog.SetWindow(198,135,&Party);
	killMe.map=false;
  postCutScene=0;
	preText=0;

	FPS=0;
	FPSCounter=0;
	//FPSTimer=0;
	//RenderTimer=0;

	//PlayerCounter=0;

	PlayerTimer=0;
	xSpeed=0;
	ySpeed=0;
	xProgress=0;
	yProgress=0;
	//1 msTime=timeGetTime();

  ticks=0;
  lastTicks=SDL_GetTicks();
  tickFPS=0;
  tickNPC=0;
  tickX=0;
  tickY=0;


	LARGE_INTEGER li;
	DWORD_PTR threadAffMask = SetThreadAffinityMask(GetCurrentThread(), 1);
 	QueryPerformanceFrequency(&li);
	freq=li.QuadPart/1000.0;
	QueryPerformanceCounter(&li);
 	SetThreadAffinityMask(GetCurrentThread(),threadAffMask );
	fps64=li.QuadPart;

  canvas = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
  collide = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
  SDL_SetColorKey(collide, SDL_TRUE, 0);

	bDebug=false;
	return true;
}

int cDA2Game::GoGame(){
	char str[1024];
	int i;

  unsigned int curTicks=SDL_GetTicks();
  ticks=curTicks - lastTicks;
  lastTicks=curTicks;
  tickFPS+=ticks;
  if(tickFPS>1000){
    FPS=FPSCounter;
    FPSCounter=0;
    tickFPS-=1000;
  }

	MusicObj->CheckState();

  tickAnim+=ticks;
	if(tickAnim > 125){
		tickAnim-=125;
    colorCounter++;
    if(colorCounter > 7) colorCounter=0;
		animCounter++;
    if(animCounter > 3) {
      animCounter=0;
    }
	}

  /*
  FILE* xf=fopen("stack.txt","at");
  fprintf(xf,"---\n");
  fprintf(xf,"postCutScene: %d\n",postCutScene);
  mainStack.SetStateFirst();
  while(mainStack.Process()!=NoGameStack){
    switch(mainStack.Process()){
    case GameRender: fprintf(xf,"GameRender\n"); break;
		case TextWindow: fprintf(xf,"TextWindow\n"); break;
		case StatScreen: fprintf(xf,"StatScreen\n"); break;
		case ShopScreen: fprintf(xf,"ShopScreen\n"); break;
		case JournalScreen: fprintf(xf,"JournalScreen\n"); break;
		case SaveDialog: fprintf(xf,"SaveDialog\n"); break;
		case LoadDialog: fprintf(xf,"LoadDialog\n"); break;
		case OptionsScreen: fprintf(xf,"OptionsScreen\n"); break;
		case IntroScreen: fprintf(xf,"IntroScreen\n"); break;
		case InnDialog: fprintf(xf,"InnDialog\n"); break;
		case BasicDialog: fprintf(xf,"BasicDialog\n"); break;
		case SpellDialog: fprintf(xf,"SpellDialog\n"); break;
		case QuitDialog: fprintf(xf,"QuitDialog\n"); break;
		case BattleChoiceDialog: fprintf(xf,"BattleChoiceDialog\n"); break;
		case SurpriseAttackDialog: fprintf(xf,"SurpriseAttackDialog\n"); break;
		case TrainDialog: fprintf(xf,"TrainDialog\n"); break;
		case CastDialog: fprintf(xf,"CastDialog\n"); break;
		case GameMenu: fprintf(xf,"GameMenu\n"); break;
		case ChangeTheMap: fprintf(xf,"ChangeTheMap\n"); break;
		case ScriptBattle: fprintf(xf,"ScriptBattle\n"); break;
		case BattleMain: fprintf(xf,"BattleMain\n"); break;
		case CutSceneA: fprintf(xf,"CutSceneA\n"); break;
		case CutSceneB: fprintf(xf,"CutSceneB\n"); break;
		default: fprintf(xf,"default\n"); break;
    }
    if(!mainStack.SetStateNext()) break;
	}
  fclose(xf);
  */

	bool bFlip=false;
	mainStack.SetStateFirst();
	if(mainStack.Process()==NoGameStack) return 0;
	while(mainStack.Process()!=NoGameStack){

		switch(mainStack.Process()){
		case GameRender:
			if(Render()) bFlip=true;
			break;
		case TextWindow:
			if(bFlip) textWindow.Render();
			break;
		case StatScreen:
			if(Party.Render()) bFlip=true;
			break;
		case ShopScreen:
			if(shop.Render()) bFlip=true;
			break;
		case JournalScreen:
			if(journal.Render()) bFlip=true;
			break;
		case SaveDialog:
			if(bFlip) loadSave.Render(false);
			break;
		case LoadDialog:
			if(bFlip) loadSave.Render(true);
			break;
		case OptionsScreen:
			if(options.Render()) bFlip=true;
			break;
		case IntroScreen:
			if(intro.Render()) bFlip=true;
			break;
		case InnDialog:
		case BasicDialog:
		case SpellDialog:
		case QuitDialog:
		case BattleChoiceDialog:
		case SurpriseAttackDialog:
			if(bFlip) dialog.Render();
			break;
		case TrainDialog:
			if(bFlip){
				if(bTrainDialog) dialog.Render();
				else trainDialog.Render();
			}
			break;
		case CastDialog:
			if(bFlip) castDialog.Render();
			break;
		case GameMenu:
			if(bFlip) listDialog.Render();
			break;
		case ChangeTheMap:
		case ScriptBattle:
		case BattleMain:
			//do nothing
			break;
		case CutSceneA:
			//do nothing
			break;
		case CutSceneB:
			if(cutScene.Render()) bFlip=true;
			break;
		default:
			break;
		}
		if(!mainStack.SetStateNext()) break;
	}

	if(bFlip) {
    FPSCounter++;
    SDL_SetRenderTarget(display->renderer, NULL);
    SDL_RenderCopy(display->renderer, canvas, NULL, NULL);
    SDL_RenderPresent(display->renderer);
    SDL_SetRenderTarget(display->renderer, canvas);
		//ddObj->ddsp->Flip(NULL,DDFLIP_WAIT); //|DDFLIP_NOVSYNC);
	}

	mainStack.SetStateLast();
	if(mainStack.Process()!=NoGameStack){
		switch(mainStack.Process()){
		case GameRender:
			return Logic();
			break;
		case TextWindow:
			Player.Moving=false;
			if(!textWindow.Logic()) {

				//wait for unpress
				while( diObj->CheckLockMouse(0) || diObj->CheckLockMouse(1) || diObj->CheckLock(KEY_SPACE)) diObj->pollEvents();
				mainStack.Pop();

				//if text before battle break now and remaining text handled elsewhere.
				if(preText>0) {
					preText--;
					if(preText==0) break;
				}

				//otherwise display next text
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str); 
					textWindow.SetWindow(48,258,str);
					mainStack.Push(TextWindow);
					vText.erase(vText.begin());

					//special case, remove npc/obj after last text
				} else if(killMe.map>0){
					if(killMe.npc) Maps[killMe.map].NPCArray[killMe.num].NStatus=0;
					else Maps[killMe.map].ObjArray->at(killMe.num).Status=0;
					killMe.map=0;
				}

			}
			break;
		case ChangeTheMap:
			ChangeMap(newMap.map,newMap.x,newMap.y);
			mainStack.Pop();
			break;
		case StatScreen:
			i=Party.Logic();
			if(i==0){
				mainStack.Pop();
				mainStack.Push(GameRender);
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			} else if(i==2){
				if(!DropItem()){
					string s="Not enough room to drop an item. Try moving to another spot.";
					textWindow.SetWindow(48,258,&s[0]);
					mainStack.Push(TextWindow);
				}
			}
			break;
		case ShopScreen:
			if(!shop.Logic()){
				mainStack.Pop();
				mainStack.Push(GameRender);
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str);  
					textWindow.SetWindow(48,258,str);
					mainStack.Push(TextWindow);
					vText.erase(vText.begin());
				}
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			}
			break;
		case JournalScreen:
			if(!journal.Logic()){
				mainStack.Pop();
				mainStack.Push(GameRender);
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			}
			break;
		case SaveDialog:
			if(!loadSave.Logic(false)) {
				mainStack.Pop();
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			}
			break;
		case LoadDialog:
			switch(loadSave.Logic(true)){
			case 0:
				mainStack.Pop();
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
				break;
			case 2:
				mainStack.Pop();
				ChangeMap(Player.Map,Player.X,Player.Y);
				textParser.setNames(Party.Players[0].PlayerName,Party.Players[1].PlayerName,Party.Players[2].PlayerName);
				renderSpeed=options.Options.GameSpeed;
				bBorder=options.Options.Border;
				MusicObj->SetVolume(options.Options.MusicVolume);
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			default:
				break;
			}
			break;
		case OptionsScreen:
			if(!options.Logic()){
				mainStack.Pop();
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
				renderSpeed=options.Options.GameSpeed;
				bBorder=options.Options.Border;
				MusicObj->SetVolume(options.Options.MusicVolume);
				mainStack.Push(GameMenu);
			}
			break;
		case IntroScreen:
			if(!intro.Logic()){
				mainStack.Pop();
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			}
			break;
		case InnDialog:
			if(!dialog.Logic()){
				if(dialog.GetButton()){
					if(Party.Gold<InnCost){
						//display error message
						dialog.SetWindow(198,155,"You don't have enough gold pieces!",false);
						mainStack.Pop();
						mainStack.Push(BasicDialog);
						break;
					} else {
						Party.Gold-=InnCost;
						for(i=0;i<3;i++){
							Party.Players[i].HP=Party.Players[i].MaxHP;
							Party.Players[i].MP=Party.Players[i].MaxMP;
						}
					}
				}
				while(diObj->CheckLockMouse(0))	diObj->pollEvents();
				mainStack.Pop();
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str);  
					textWindow.SetWindow(48,258,str);
					mainStack.Push(TextWindow);
					vText.erase(vText.begin());
				}
			}
			break;
		case SpellDialog:
			if(!dialog.Logic()){
				if(dialog.GetButton()){
					if(Party.Gold<SpellCost){
						//display broke-ass message
						dialog.SetWindow(198,155,"You don't have enough gold pieces!",false);
						mainStack.Pop();
						mainStack.Push(BasicDialog);
						break;
					} else if(Party.Spells[SpellNum].Known){
						//display known message
						dialog.SetWindow(198,155,"You already know that spell!",false);
						mainStack.Pop();
						mainStack.Push(BasicDialog);
						break;
					} else {
						Party.Gold-=SpellCost;
						Party.Spells[SpellNum].Known=true;
						Party.SpellList[Party.SpellCount++]=SpellNum;
					}
				}
				while(diObj->CheckLockMouse(0))	diObj->pollEvents();
				mainStack.Pop();
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str);  
					textWindow.SetWindow(48,258,str);
					mainStack.Push(TextWindow);
					vText.erase(vText.begin());
				}
			}
			break;
		case BasicDialog:
			if(!dialog.Logic()){
				while(diObj->CheckLockMouse(0))	diObj->pollEvents();
				mainStack.Pop();
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str);  
					textWindow.SetWindow(48,258,str);
					mainStack.Push(TextWindow);
					vText.erase(vText.begin());
				}
			}
			break;
		case QuitDialog:
			if(!dialog.Logic()){
				while(diObj->CheckLockMouse(0))	diObj->pollEvents();
				mainStack.Pop();
				if(dialog.GetButton()) {
					mainStack.Pop();
					return 0;
				} else {
					mainStack.Push(GameMenu);
				}
			}
			break;
		case TrainDialog:
			if(bTrainDialog){
				if(!dialog.Logic()){
					while(diObj->CheckLockMouse(0))	diObj->pollEvents();
					bTrainDialog=false;
				}
			} else {
				if(!trainDialog.Logic()){
					while(diObj->CheckLockMouse(0))	diObj->pollEvents();
					mainStack.Pop();
					if(vText.size()>0){
						textParser.parseText(&vText[0][0],str);  
						textWindow.SetWindow(48,258,str);
						mainStack.Push(TextWindow);
						vText.erase(vText.begin());
					}
				}
			}
			break;
		case CastDialog:
			if(!castDialog.Logic()) {
				mainStack.Pop();
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
			}
			break;
		case BattleChoiceDialog:
			if(!dialog.Logic()){
				mainStack.Pop();
				while(diObj->CheckLockMouse(0))	diObj->pollEvents();
				if(dialog.GetButton()){
					mainStack.Pop();
					mainStack.Push(BattleMain);
					battleEngine.Load(Maps[MapWin[1][1]].BattleNum);
				}
			}
			break;
		case SurpriseAttackDialog:
			if(!dialog.Logic()) {
				while(diObj->CheckLockMouse(0))	diObj->pollEvents();
				mainStack.Pop();
				mainStack.Pop();
				mainStack.Push(BattleMain);
				battleEngine.Load(Maps[MapWin[1][1]].BattleNum);
			}
			break;
		case GameMenu:
			if(!listDialog.Logic()){
				while(diObj->CheckLockMouse(0) || diObj->CheckLockMouse(1) || diObj->CheckLock(KEY_ESCAPE)) diObj->pollEvents();
				mainStack.Pop();
				switch(listDialog.GetSelection()){
				case 1: //Save
					mainStack.Push(SaveDialog);
					break;
				case 2: //Load
					mainStack.Push(LoadDialog);
					break;
				case 3: //Options
					mainStack.Push(OptionsScreen);
					break;
				case 4: //Keyboard
					break;
				case 5: //Exit
					dialog.SetWindow(198,155,"Are you sure you want to quit?");
					mainStack.Push(QuitDialog);
					break;
				default:
					break;
				}
			}
			break;
		case ScriptBattle:
			if(preText>0){			
				textParser.parseText(&vText[0][0],str);  
				textWindow.SetWindow(48,258,str);
				mainStack.Push(TextWindow);
				vText.erase(vText.begin());
				//preText--;
			} else {
				mainStack.Pop();
				mainStack.Pop();
				mainStack.Push(BattleMain);
			}
			break;
		case BattleMain:
			//1 MusicObj->ChangeSong(-1);
			switch(battleEngine.GoBattle()){
			case 0:
			case 3:
				
				mainStack.Pop();
				mainStack.Push(GameRender);
        if(postCutScene>0){
          mainStack.Push(CutSceneA);
          postCutScene=0;
        } else {
          MusicObj->ChangeSong(Player.Map-1);
        }
				while(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLockMouse(0))	diObj->pollEvents();
				//otherwise display next text
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str); 
					textWindow.SetWindow(48,258,str);
					mainStack.Push(TextWindow);
					vText.erase(vText.begin());

					//special case, remove npc/obj after last text
				} else if(killMe.map>0){
					if(killMe.npc) Maps[killMe.map].NPCArray[killMe.num].NStatus=0;
					else Maps[killMe.map].ObjArray->at(killMe.num).Status=0;
					killMe.map=0;
				}
				break;
			case 2:
				MusicObj->ChangeSong(Player.Map-1);
				vText.clear();
				mainStack.Pop();
				return 2;
				break;
			default:
				break;
			}
			break;
		case CutSceneA:
			mainStack.Pop();
			mainStack.Pop();
			mainStack.Push(CutSceneB);
			break;
		case CutSceneB:
			if(!cutScene.Logic()){
        MusicObj->ChangeSong(Player.Map-1);
				mainStack.Pop();
				mainStack.Push(GameRender);
				//special case, remove npc/obj after last text
				if(killMe.map>0){
					if(killMe.npc) Maps[killMe.map].NPCArray[killMe.num].NStatus=0;
					else Maps[killMe.map].ObjArray->at(killMe.num).Status=0;
					killMe.map=0;
				}
			}
			break;
		case WaitOnMouse:
			diObj->pollEvents();
			if( diObj->CheckLockMouse(0) || diObj->CheckLockMouse(1) ) break;
			mainStack.Pop();
			if(vText.size()>0){
				textParser.parseText(&vText[0][0],str); 
				textWindow.SetWindow(48,258,str);
				mainStack.Push(TextWindow);
				vText.erase(vText.begin());
			}
			break;
		case WaitOnKeyboard:
			diObj->pollEvents();
			if(diObj->CheckLock(KEY_ESCAPE) || diObj->CheckLock(KEY_SPACE)) break;
			mainStack.Pop();
			break;
		default:
			break;
		}
	}

	return 1;

}


//==================
//Game Loading
//==================

void cDA2Game::LoadMaps(){
	int x;
	int y;
	int z;

	int i;
	short count;
	ObjectType o;

	FILE *f;

	//read map data
	f=fopen("data/map.dat","rb");
	i=0;
	while(!feof(f)){
		i++;
		for(z=0;z<4;z++){
			for(y=0;y<96;y++){
				for(x=0;x<96;x++){
					fread(&Maps[i].MapArray[x][y][z],1,2,f);
				}
			}
		}
	}
	fclose(f);
	
	//read list data
	f=fopen("data/lis.dat","rt");
	i=0;
	int j=0;
	char str[10];
	while(!feof(f)){
		i++;
		for(x=0;x<256;x++) {
			fscanf(f,"%d\n",&j);
			Maps[i].MapKey[x]=j;
		}
		fscanf(f,"%s\n",str);
	}
	fclose(f);

	f=fopen("data/npc.dat","rb");
	i=0;
	while(!feof(f)){
		i++;
		fread(&count,1,2,f);
		if(feof(f)) break;
		Maps[i].NPCCount=count;
		if(Maps[i].NPCCount>0) Maps[i].NPCArray = new NPCType[Maps[i].NPCCount];
		for(j=0;j<Maps[i].NPCCount;j++){
			fread(&Maps[i].NPCArray[j],1,sizeof(NPCType),f);
			Maps[i].NPCArray[j].MapXLoc=Maps[i].NPCArray[j].MapXLoc/32+1;
			Maps[i].NPCArray[j].MapYLoc=Maps[i].NPCArray[j].MapYLoc/32+1;
			Maps[i].NPCArray[j].XMax=Maps[i].NPCArray[j].XMax/32+1;
			Maps[i].NPCArray[j].XMin=Maps[i].NPCArray[j].XMin/32+1;
			Maps[i].NPCArray[j].YMax=Maps[i].NPCArray[j].YMax/32+1;
			Maps[i].NPCArray[j].YMin=Maps[i].NPCArray[j].YMin/32+1;
			Maps[i].NPCArray[j].PicType--;
		}
	}
	fclose(f);

  f=fopen("data/obj.dat","rb");
	i=0;
	while(!feof(f)){
		i++;
		fread(&count,1,2,f);
		if(feof(f)) break;
		for(j=0;j<count;j++){
			fread(&o,1,sizeof(ObjectType),f);
			o.Xco=o.Xco/32+1;
			o.Yco=o.Yco/32+1;
			Maps[i].ObjArray->push_back(o);
		}
	}
	fclose(f);

	int idx;
	f=fopen("data/tlk.idx","rt");
	for(i=1;i<500;i++){
		fscanf(f,"%d\n",&idx);
		NPCFileIndex[i]=idx-1;
	};
	fclose(f);
	NPCFileIndex[0]=0;

	f=fopen("data/osc.idx","rt");
	for(i=1;i<500;i++){
		fscanf(f,"%d\n",&idx);
		ObjFileIndex[i]=idx-1;
	};
	fclose(f);
	ObjFileIndex[0]=0;

	//read random battles
	f=fopen("data/randbat.dat","rt");
	for(i=1;i<500;i++){
		fscanf(f,"%d\n",&idx);
		Maps[i].BattleNum=idx;
	}
	fclose(f);

	//Set cutscene here because it needs map info
	cutScene.Init(display,ddGfx,diObj,&Maps[0],MusicObj,&animCounter);

}

void cDA2Game::NewGame(){
	FILE *f;
	char line[256];
	char *tok;
	int x;

	int i;

	//Load game flags
	for(i=0;i<300;i++) Flags[i]=0;

	//Load Player Item data
	f=fopen("data/player1.itm","rt");
	for(i=0;i<3;i++){
		fscanf(f,"%d,%d,%d,%d,%d,%d,%d,%d\n",&Party.Players[i].Gear.Weapon,&Party.Players[i].Gear.Shield,&Party.Players[i].Gear.Armor,&Party.Players[i].Gear.Helmet,
			                                   &Party.Players[i].Gear.Boots,&Party.Players[i].Gear.Hands,&Party.Players[i].Gear.Legs,&Party.Players[i].Gear.Ring);
	}
	fclose(f);

	//Load Player data
	f=fopen("data/pstats.txt","rt");
	for(i=0;i<3;i++){
		fscanf(f,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",&Party.Players[i].BaseStr,&Party.Players[i].BaseAgl,&Party.Players[i].BaseInt,&Party.Players[i].BaseSpeed,
														&Party.Players[i].ModStr,&Party.Players[i].ModAgl,&Party.Players[i].ModInt,&Party.Players[i].ModSpeed,
														&Party.Players[i].CS,&Party.Players[i].SAP,&Party.Players[i].TP,&Party.Players[i].HP,&Party.Players[i].MP,
														&Party.Players[i].MaxHP,&Party.Players[i].MaxMP,&Party.Players[i].Armor,&Party.Players[i].Encumbrance,&Party.Players[i].Experience,
														&Party.Players[i].Level,Party.Players[i].PlayerName);
		//if(Party.Players[i].CS>10) Party.Players[i].CS = 10;
	}
	fclose(f);
	textParser.setNames(Party.Players[0].PlayerName,Party.Players[1].PlayerName,Party.Players[2].PlayerName);

	//Load Party Item data
	f=fopen("data/partyitm.itm","rt");
	for(i=0;i<40;i++)	fscanf(f,"%d\n",&Party.PartyItems[i]);
	fclose(f);

	//Load spells
	Party.SpellCount=0;
	f=fopen("data/spells.txt","rt");
	for(i=0;i<30;i++){
		fgets(line,256,f);
		tok=strtok(line,",\"\n");
		x=atoi(tok);
		if(x>0){
			Party.Spells[i].Known=true;
			Party.SpellList[Party.SpellCount++]=i;
		} else {
			Party.Spells[i].Known=false;
		}
		tok=strtok(NULL,",\"\n");
		Party.Spells[i].Type=atoi(tok);
		tok=strtok(NULL,",\"\n");
		Party.Spells[i].MPCost=atoi(tok);
		tok=strtok(NULL,",\"\n");
		strcpy(Party.Spells[i].SpellName,tok);
	}
	fclose(f);

	//Set initial variables
	Party.Gold=500;
	Player.X=1920;
	Player.Y=1920;
	Player.Map=22;
	ChangeMap(Player.Map,Player.X,Player.Y);
	SetRoofMask();

	options.Options.GameSpeed=8;
	renderSpeed=options.Options.GameSpeed;

	intro.SetDefault();
	mainStack.Push(IntroScreen);

  //cout << "NewGame() ok" << endl;

}

bool cDA2Game::Render() {

	//if(!CheckRender()) return false;
	//if(msTime-RenderTimer<5) return false;
	//RenderTimer=msTime;

	int x,y,z;
	int Tile;
	SDL_Rect r;
	//RECT r2;
	HRESULT hr;
	bool ShowRoof=true;

	int TrueMap,TrueX,TrueY;

  //SDL_SetRenderTarget(display->renderer, canvas);
  SDL_RenderClear(display->renderer);

	for(z=0;z<4;z++){
		for(y=PlayerCam.TilePosY-8;y<PlayerCam.TilePosY+8;y++){
			for(x=PlayerCam.TilePosX-10;x<PlayerCam.TilePosX+11;x++){
				ShowRoof=true;
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
						TrueMap=MapWin[1][1];	TrueX=x; TrueY=y;
						if(z==3 && RoofMask[x][y]!=1) ShowRoof=false;
					}
				}
				Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[TrueX][TrueY][z]];
				if(Tile==0) continue;
				if(z==3 && !ShowRoof) continue;
				if(Tile>2000) Tile+=animCounter;
				
				r.x=(x-(PlayerCam.TilePosX-10))*32-PlayerCam.OffsetX;
				r.y=(y-(PlayerCam.TilePosY-8))*32-PlayerCam.OffsetY;
				r.w=32;
				r.h=32;
				if(r.x>639 || r.y>479) continue;
        SDL_RenderCopy(display->renderer, ddGfx->Tiles[ddGfx->vTile->at(Tile).index]->texture, &ddGfx->vTile->at(Tile).r, &r);
				//hr=ddObj->ddsb->Blt(&r, ddGfx->Tiles[ddGfx->vTile->at(Tile).index], &ddGfx->vTile->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			
			}
		
		}

		//display objects
		if(z==1) ObjRender(display->renderer);
		if(z==1) NPCRender();


		//display player character
		if(z==1) {
			r.x=304;
			r.y=224;
			r.w=32;
			r.h=32;
      SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(Player.imageOffset + (Player.dir * 4) + Player.frame).r, &r);
			//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(Player.imageOffset+(Player.dir*4)+Player.frame).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		}

	}

	//Test text display
	char txt[128];
	//sprintf(txt,"Map: %d\nX-Coord: %d\nY-Coord: %d\nFPS: %d\nxs: %d  ys:%d",MapWin[1][1],PlayerCam.TilePosX,PlayerCam.TilePosY,FPS,xSpeed,ySpeed);
	//text.drawText(display->renderer,50,50,0,txt);

	//Display border
	if(bBorder){
		r.x=0;
		r.y=0;
		r.h=480;
		r.w=640;
    SDL_RenderCopy(display->renderer, ddGfx->Border->texture, &r, &r);
		//ddObj->ddsb->Blt(&r,ddGfx->Border,&r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}

	//display the mouse cursor
	x=1;y=1;
  if(diObj->mouseX > (diObj->mouseY+480)/2.25) x++;
  if(diObj->mouseX < (diObj->mouseY-1040)/-2.5) x--;
  if(diObj->mouseY > (diObj->mouseX*0.25)+160) y++;
  if(diObj->mouseY < (diObj->mouseX*-0.25)+320) y--;
  r.y=diObj->mouseY - ddGfx->CursorY[Cursor[x][y]];
  r.x=diObj->mouseX - ddGfx->CursorX[Cursor[x][y]];
  r.h=24;
  r.w=16;
  SDL_RenderCopy(display->renderer, ddGfx->Cursor->texture, &ddGfx->aCursor[Cursor[x][y]], &r);
  //ddObj->ddsb->Blt(&r, ddGfx->Cursor,&ddGfx->aCursor[Cursor[x][y]],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	//sprintf(txt,"MouseX: %d  MouseY: %d  Cursor: %d",diObj->MouseX(),diObj->MouseY(),Cursor[x][y]);
	//text.drawText(display->renderer,50,35,0,txt);


	//f=fopen("Render.txt","at");
	//fprintf(f,"Render ok\n");
	//fclose(f);
	
  //SDL_SetRenderTarget(display->renderer, NULL);
  //SDL_RenderCopy(display->renderer, canvas, NULL, NULL);

	return true;
}

void cDA2Game::GShot(){
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
  1*/
}

void cDA2Game::NPCLogic(){
	int i,mx,my;
	int dir;
	int TrueMap;
	int Tile;

	for(my=0;my<3;my++){
		for(mx=0;mx<3;mx++){
			TrueMap=MapWin[mx][my];
			for(i=0;i<Maps[TrueMap].NPCCount;i++){
				if(Maps[TrueMap].NPCArray[i].Movable==0) continue;
				if(Maps[TrueMap].NPCArray[i].NStatus==0) continue;

				if(Maps[TrueMap].NPCArray[i].InMotion){
					Maps[TrueMap].NPCArray[i].PicFrame++;
					if(Maps[TrueMap].NPCArray[i].PicFrame==8) Maps[TrueMap].NPCArray[i].PicFrame=0;
					Maps[TrueMap].NPCArray[i].xoffset+=Maps[TrueMap].NPCArray[i].xmove;
					Maps[TrueMap].NPCArray[i].yoffset+=Maps[TrueMap].NPCArray[i].ymove;
					Maps[TrueMap].NPCArray[i].InMotion++;
					if(Maps[TrueMap].NPCArray[i].InMotion==33){
						Maps[TrueMap].NPCArray[i].InMotion=0;
						Maps[TrueMap].NPCArray[i].MapXLoc+=(Maps[TrueMap].NPCArray[i].xoffset/32);
						Maps[TrueMap].NPCArray[i].MapYLoc+=(Maps[TrueMap].NPCArray[i].yoffset/32);
						Maps[TrueMap].NPCArray[i].xoffset=0;
						Maps[TrueMap].NPCArray[i].yoffset=0;
					}
				} else {
					dir=rand()%100;
					switch(dir){
					case 0:
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc][Maps[TrueMap].NPCArray[i].MapYLoc-1][1]];
						if(ddGfx->vTile->at(Tile).solid) break;
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc][Maps[TrueMap].NPCArray[i].MapYLoc-1][2]];
						if(ddGfx->vTile->at(Tile).solid) break;
						if(Maps[TrueMap].NPCArray[i].MapYLoc>Maps[TrueMap].NPCArray[i].YMin){
							Maps[TrueMap].NPCArray[i].InMotion=1;
							Maps[TrueMap].NPCArray[i].ymove=-1;
							Maps[TrueMap].NPCArray[i].xmove=0;
							Maps[TrueMap].NPCArray[i].PicNum=8;
						}
						break;
					case 1:
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc+1][Maps[TrueMap].NPCArray[i].MapYLoc][1]];
						if(ddGfx->vTile->at(Tile).solid) break;
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc+1][Maps[TrueMap].NPCArray[i].MapYLoc][2]];
						if(ddGfx->vTile->at(Tile).solid) break;
						if(Maps[TrueMap].NPCArray[i].MapXLoc<Maps[TrueMap].NPCArray[i].XMax){
							Maps[TrueMap].NPCArray[i].InMotion=1;
							Maps[TrueMap].NPCArray[i].xmove=1;
							Maps[TrueMap].NPCArray[i].ymove=0;
							Maps[TrueMap].NPCArray[i].PicNum=4;
						}
						break;
					case 2:
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc][Maps[TrueMap].NPCArray[i].MapYLoc+1][1]];
						if(ddGfx->vTile->at(Tile).solid) break;
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc][Maps[TrueMap].NPCArray[i].MapYLoc+1][2]];
						if(ddGfx->vTile->at(Tile).solid) break;
						if(Maps[TrueMap].NPCArray[i].MapYLoc<Maps[TrueMap].NPCArray[i].YMax){
							Maps[TrueMap].NPCArray[i].InMotion=1;
							Maps[TrueMap].NPCArray[i].ymove=1;
							Maps[TrueMap].NPCArray[i].xmove=0;
							Maps[TrueMap].NPCArray[i].PicNum=0;
						}
						break;
					case 3:
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc-1][Maps[TrueMap].NPCArray[i].MapYLoc][1]];
						if(ddGfx->vTile->at(Tile).solid) break;
						Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[Maps[TrueMap].NPCArray[i].MapXLoc-1][Maps[TrueMap].NPCArray[i].MapYLoc][2]];
						if(ddGfx->vTile->at(Tile).solid) break;
						if(Maps[TrueMap].NPCArray[i].MapXLoc>Maps[TrueMap].NPCArray[i].XMin){
							Maps[TrueMap].NPCArray[i].InMotion=1;
							Maps[TrueMap].NPCArray[i].xmove=-1;
							Maps[TrueMap].NPCArray[i].ymove=0;
							Maps[TrueMap].NPCArray[i].PicNum=12;
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

}

void cDA2Game::NPCRender(){
	int i,x,y,mx,my;
	int TrueMap;
	int Tile;
	SDL_Rect r;

  r.w=32;
  r.h=32;
	for(my=0;my<3;my++){
		for(mx=0;mx<3;mx++){
			TrueMap=MapWin[mx][my];
			for(i=0;i<Maps[TrueMap].NPCCount;i++){

				//Don't render if not visible
				if(Maps[TrueMap].NPCArray[i].NStatus==0) continue;

				x=Maps[TrueMap].NPCArray[i].MapXLoc+96*(mx-1);
				y=Maps[TrueMap].NPCArray[i].MapYLoc+96*(my-1);
				Tile=Maps[TrueMap].NPCArray[i].PicType+Maps[TrueMap].NPCArray[i].PicNum+Maps[TrueMap].NPCArray[i].PicFrame/2;

				if(x-(PlayerCam.TilePosX-10) >=0 && x-(PlayerCam.TilePosX-10) <=20 &&
					 y-(PlayerCam.TilePosY-8) >=0 && y-(PlayerCam.TilePosY-8) <=16) {
					r.x=(x-(PlayerCam.TilePosX-10))*32-PlayerCam.OffsetX+Maps[TrueMap].NPCArray[i].xoffset;
					r.y=(y-(PlayerCam.TilePosY-8))*32-PlayerCam.OffsetY+Maps[TrueMap].NPCArray[i].yoffset;
          SDL_RenderCopy(display->renderer, ddGfx->Chars[ddGfx->vNPC->at(Tile).index]->texture, &ddGfx->vNPC->at(Tile).r, &r);
					//ddObj->ddsb->Blt(&r, ddGfx->Chars[ddGfx->vNPC->at(Tile).index], &ddGfx->vNPC->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
				}
			}
		}
	}

}


void cDA2Game::ObjRender(SDL_Renderer *surf, bool bSolid){
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
				if(bSolid && !Maps[TrueMap].ObjArray->at(i).Solid) continue;
				x=Maps[TrueMap].ObjArray->at(i).Xco+96*(mx-1);
				y=Maps[TrueMap].ObjArray->at(i).Yco+96*(my-1);
				Tile=Maps[TrueMap].ObjArray->at(i).ObjectPic;

        if(x - (PlayerCam.TilePosX - 10) >= 0 && x - (PlayerCam.TilePosX - 10) <= 20 &&
           y - (PlayerCam.TilePosY - 8) >= 0 && y - (PlayerCam.TilePosY - 8) <= 16) {
          r.x=(x - (PlayerCam.TilePosX - 10)) * 32 - PlayerCam.OffsetX;
          r.y=(y - (PlayerCam.TilePosY - 8)) * 32 - PlayerCam.OffsetY;
          if(Tile>0) SDL_RenderCopy(surf, ddGfx->Objects[ddGfx->vObj->at(Tile).index]->texture, &ddGfx->vObj->at(Tile).r, &r);
          //if(Tile>0) surf->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(Tile).index], &ddGfx->vObj->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
          if(bHighlightObject && !Maps[TrueMap].ObjArray->at(i).HideTab) box.DrawBox(r.x, r.y, r.x + 32, r.y + 32, 88, false, colorCounter);
        }
			}
		}
	}

}

void cDA2Game::ObjRender(SDL_Surface *surf, bool bSolid){
  int i, x, y, mx, my;
  int TrueMap;
  int Tile;
  SDL_Rect r;

  r.w=32;
  r.h=32;
  for(my=0; my<3; my++){
    for(mx=0; mx<3; mx++){
      TrueMap=MapWin[mx][my];
      for(i=0; i<Maps[TrueMap].ObjArray->size(); i++){
        if(Maps[TrueMap].ObjArray->at(i).Status == 0) continue;
        if(bSolid && !Maps[TrueMap].ObjArray->at(i).Solid) continue;
        x=Maps[TrueMap].ObjArray->at(i).Xco + 96 * (mx - 1);
        y=Maps[TrueMap].ObjArray->at(i).Yco + 96 * (my - 1);
        Tile=Maps[TrueMap].ObjArray->at(i).ObjectPic;

        if(x - (PlayerCam.TilePosX - 10) >= 0 && x - (PlayerCam.TilePosX - 10) <= 20 &&
           y - (PlayerCam.TilePosY - 8) >= 0 && y - (PlayerCam.TilePosY - 8) <= 16) {
          r.x=(x - (PlayerCam.TilePosX - 10)) * 32 - PlayerCam.OffsetX;
          r.y=(y - (PlayerCam.TilePosY - 8)) * 32 - PlayerCam.OffsetY;
          if(Tile>0) SDL_BlitSurface(ddGfx->Objects[ddGfx->vObj->at(Tile).index]->surface, &ddGfx->vObj->at(Tile).r, surf, &r);
        }
      }
    }
  }

}

bool cDA2Game::CheckRender(){
  /*1 probably safe to delete
	renderCounter++;
	if(renderCounter>=renderSpeed){
		playerRender++;
		if(playerRender>=((8-renderSpeed)/2+2) ){
			if(Player.Moving) Player.frame++;
			if(Player.frame>3)Player.frame=0;
			playerRender=0;
		}
		if(bAnimateNow){
      colorCounter++;
      if(colorCounter > 7) colorCounter=0;
			animCounter++;
			if(animCounter>3) animCounter=0;
			bAnimateNow=false;
		};
		renderCounter=0;
		return true;
	}
	return false;
  */
}

//==============
// Check mouse vs. game element
//==============

bool cDA2Game::NPCClick(int atX, int atY, int &MapNum, int &NPCNum){
	int i,x,y,mx,my;
	int TrueMap;
	RECT r;

	for(my=0;my<3;my++){
		for(mx=0;mx<3;mx++){
			TrueMap=MapWin[mx][my];
			for(i=0;i<Maps[TrueMap].NPCCount;i++){

				//Don't function if not visible
				if(Maps[TrueMap].NPCArray[i].NStatus==0) continue;

				x=Maps[TrueMap].NPCArray[i].MapXLoc+96*(mx-1);
				y=Maps[TrueMap].NPCArray[i].MapYLoc+96*(my-1);

				if(x-(PlayerCam.TilePosX-10) >=0 && x-(PlayerCam.TilePosX-10) <=20 &&
					 y-(PlayerCam.TilePosY-8) >=0 && y-(PlayerCam.TilePosY-8) <=16) {
					r.left=(x-(PlayerCam.TilePosX-10))*32-PlayerCam.OffsetX+Maps[TrueMap].NPCArray[i].xoffset;
					r.top=(y-(PlayerCam.TilePosY-8))*32-PlayerCam.OffsetY+Maps[TrueMap].NPCArray[i].yoffset;
					r.right=r.left+32;
					r.bottom=r.top+32;
					if( atX>=r.left && atX <=r.right && atY>=r.top && atY<=r.bottom){
						MapNum=TrueMap;
						NPCNum=i;
						return true;
					}
				}
			}
		}
	}

	return false;

}

int cDA2Game::ObjClick(int atX, int atY, int &MapNum, int &ObjNum){
	int i,x,y,mx,my;
	int TrueMap;
	RECT r;

	for(my=0;my<3;my++){
		for(mx=0;mx<3;mx++){
			TrueMap=MapWin[mx][my];
			for(i=0;i<Maps[TrueMap].ObjArray->size();i++){

				//Don't check if not visible
				if(Maps[TrueMap].ObjArray->at(i).Status==0) continue;

				x=Maps[TrueMap].ObjArray->at(i).Xco+96*(mx-1);
				y=Maps[TrueMap].ObjArray->at(i).Yco+96*(my-1);

				r.left=(x-(PlayerCam.TilePosX-10))*32-PlayerCam.OffsetX;
				r.top=(y-(PlayerCam.TilePosY-8))*32-PlayerCam.OffsetY;
				r.right=r.left+32;
				r.bottom=r.top+32;
				if(r.left>256 && r.right<384 && r.top>176 && r.bottom<304){
					if( atX>=r.left && atX <=r.right && atY>=r.top && atY<=r.bottom){
						MapNum=TrueMap;
						ObjNum=i;
						return Maps[TrueMap].ObjArray->at(i).Status;
					}
				}
			}
		}
	}

	return 0;

}

bool cDA2Game::Logic() {

	int x,y;
	EventTag eTag;
	int map, npc, obj;
	char str[1024];
	int randomCounter=0;

  tickX+=ticks;
  tickY+=ticks;
  tickNPC+=ticks;
	if(tickNPC > 33){
		tickNPC-=33;
		NPCLogic();
	}

	Player.Moving=false;
	diObj->pollEvents();

	if(diObj->MousePress(1) && !diObj->CheckLockMouse(1)){
		diObj->LockMouse(1);
		if(NPCClick(diObj->MouseX(),diObj->MouseY(),map,npc)){
			eTag=NPCScript(map,npc);
			switch(eTag){
			case Normal:
				if(vText.size()>0){
					textParser.parseText(&vText[0][0],str); 
					textWindow.SetWindow(48,258,str);
					vText.erase(vText.begin());
					mainStack.Push(TextWindow);
				}
				break;
			case Barter:
				mainStack.Pop();
				mainStack.Push(ShopScreen);
				break;
			case Inn:
				mainStack.Push(InnDialog);
				break;
			case Train:
				mainStack.Push(TrainDialog);
				break;
			case BattleNow:
				break;
			case Failure:
			default:
				string s;
				s="NPCScript failed.";
				textWindow.SetWindow(48,258,&s[0]);
				mainStack.Push(TextWindow);
				break;
			}
		} else {
			switch(ObjClick(diObj->MouseX(),diObj->MouseY(),map,obj)){
			case 1:
				eTag=ObjScript(map,obj);
				switch(eTag){
				case Normal:
					if(vText.size()>0){
						textParser.parseText(&vText[0][0],str); 
						textWindow.SetWindow(48,258,str);
						vText.erase(vText.begin());
						mainStack.Push(TextWindow);
					}
					break;
				case Spell:
					mainStack.Push(SpellDialog);
					break;
				case BattleNow:
					break;
				case Failure:
				default:
					string s;
					s="ObjScript failed.";
					textWindow.SetWindow(48,258,&s[0]);
					mainStack.Push(TextWindow);
					break;
				}
				break;
			case 2:
				if(Party.AddItem(Maps[map].ObjArray->at(obj).ObjectPic)){
					Maps[map].ObjArray->erase(Maps[map].ObjArray->begin()+obj);
				}
				break;
			default:
				break;
			}
		}
	}

	if(diObj->MousePress(0)){

		diObj->LockMouse(0);
		
		xSpeed=0;
		ySpeed=0;
    bool bX=false;
		if(diObj->MouseX() > (diObj->MouseY()+480)/2.25) {
			if(diObj->MouseX()>500) {
        xSpeed+=3;
        if(tickX >= 1 * renderSpeed) bX=true;
			}	else if(diObj->MouseX()>410) {
				xSpeed+=2;
        if(tickX >= 2 * renderSpeed) bX=true;
			}	else {
				xSpeed+=1;
        if(tickX >= 3 * renderSpeed) bX=true;
			}
		}
		if(diObj->MouseX() < (diObj->MouseY()-1040)/-2.5) {
			if(diObj->MouseX()<140) {
				xSpeed-=3;
        if(tickX >= 1 * renderSpeed) bX=true;
			}	else if(diObj->MouseX()<230) {
				xSpeed-=2;
        if(tickX >= 2 * renderSpeed) bX=true;
			}	else {
				xSpeed-=1;
        if(tickX >= 3 * renderSpeed)  bX=true;
			}
		}
    if(bX)tickX=0;

		if(bX) {
			if(xSpeed>0){
				PlayerCam.BumpCamera(1,0);
				Player.X+=1;
				if(!CheckCollide(1)){
					PlayerCam.BumpCamera(-1,0);
					Player.X--;
				}
				Player.Moving=true;
			} else if(xSpeed<0){
				PlayerCam.BumpCamera(-1,0);
				Player.X-=1;
				if(!CheckCollide(3)){
					PlayerCam.BumpCamera(1,0);
					Player.X++;
				}
				Player.Moving=true;
			}
		}
	
    bool bY=false;
		if(diObj->MouseY() > (diObj->MouseX()*0.25)+160) {
			if(diObj->MouseY()>380) {
				ySpeed+=3;
        if(tickY >= 1*renderSpeed) bY=true;
			}	else if(diObj->MouseY()>310) {
				ySpeed+=2;
        if(tickY >= 2*renderSpeed) bY=true;
			}	else {
				ySpeed+=1;
        if(tickY >= 3*renderSpeed) bY=true;
			}
		}
		if(diObj->MouseY() < (diObj->MouseX()*-0.25)+320) {
			if(diObj->MouseY()<100) {
				ySpeed-=3;
        if(tickY >= 1*renderSpeed) bY=true;
			} else if(diObj->MouseY()<170) {
				ySpeed-=2;
        if(tickY >= 2 * renderSpeed) bY=true;
			} else {
				ySpeed-=1;
        if(tickY >= 3 * renderSpeed) bY=true;
			}
		}
    if(bY)tickY=0;

		if(bY) {
			if(ySpeed>0){
				PlayerCam.BumpCamera(0,1);
				Player.Y+=1;
				if(!CheckCollide(0)){
					PlayerCam.BumpCamera(0,-1);
					Player.Y--;
				}
				Player.Moving=true;
			} else if(ySpeed<0){
				PlayerCam.BumpCamera(0,-1);
				Player.Y-=1;
				if(!CheckCollide(2)){
					PlayerCam.BumpCamera(0,1);
					Player.Y++;
				}
				Player.Moving=true;
			}
		}

    if(xSpeed>0) Player.dir=1;
    else if(xSpeed<0) Player.dir=3;
    if(ySpeed>0 && abs(ySpeed)>abs(xSpeed)) Player.dir=0;
    else if(ySpeed<0 && abs(ySpeed)>abs(xSpeed)) Player.dir=2;

    if(Player.Moving) xProgress++; 
    if(xProgress > 11) {
      Player.frame++;
      if(Player.frame > 3) Player.frame=0;
      xProgress=0;
    }
      
	}

	if(diObj->KeyPress(KEY_S)){
		mainStack.Push(SaveDialog);
		return true;
	}

	if(diObj->KeyPress(KEY_L)){
		mainStack.Push(LoadDialog);
		return true;
	}

	if(diObj->KeyPress(KEY_Z)){
		mainStack.SetStateLast();
		mainStack.Pop();
		mainStack.Push(StatScreen);
		return true;
	}

	if(diObj->KeyPress(KEY_B)){
		mainStack.SetStateLast();
		mainStack.Pop();
		mainStack.Push(BattleMain);
		battleEngine.Load(13);
		return true;
	}

	if(diObj->KeyPress(KEY_C)){
		castDialog.Update();
		mainStack.Push(CastDialog);
		return true;
	}

	if(diObj->KeyPress(KEY_ESCAPE)==true) {
		diObj->LockKey(KEY_ESCAPE);
		mainStack.Push(GameMenu);
		return true;
	}

	if(diObj->KeyPress(KEY_J) && !diObj->CheckLock(KEY_J)){
		diObj->LockKey(KEY_J);
		mainStack.SetStateLast();
		mainStack.Pop();
		mainStack.Push(JournalScreen);
		journal.SetPlayerPos(MapWin[1][1],PlayerCam.CameraX,PlayerCam.CameraY);
		return true;
	}

  if(diObj->KeyPress(KEY_G) && !diObj->CheckLock(KEY_G)){
		diObj->LockKey(KEY_G);
		GShot();
		return true;
	}

	if(diObj->KeyPress(KEY_TAB)) bHighlightObject=true;
	else bHighlightObject=false;

	if(diObj->KeyPress(KEY_ADD)==true && !diObj->CheckLock(KEY_ADD)) {
		diObj->LockKey(KEY_ADD);
		renderSpeed++;
		if(renderSpeed>4) renderSpeed=4;
		options.Options.GameSpeed=renderSpeed;
	}
	if(diObj->KeyPress(KEY_SUBTRACT)==true && !diObj->CheckLock(KEY_SUBTRACT)) {
		diObj->LockKey(KEY_SUBTRACT);
		renderSpeed--;
		if(renderSpeed<1) renderSpeed=1;
		options.Options.GameSpeed=renderSpeed;
	}
	if(diObj->KeyPress(KEY_RIGHT)==true) {
		PlayerCam.BumpCamera(1,0);
		Player.X+=1;
		if(!CheckCollide(1)){
			PlayerCam.BumpCamera(-1,0);
			Player.X--;
		}
		Player.dir=1;
		Player.Moving=true;
	}
	if(diObj->KeyPress(KEY_LEFT)==true) {
		PlayerCam.BumpCamera(-1,0);
		Player.X-=1;
		if(!CheckCollide(3)){
			PlayerCam.BumpCamera(1,0);
			Player.X++;
		}
		Player.dir=3;
		Player.Moving=true;
	}
	if(diObj->KeyPress(KEY_UP)==true) {
		PlayerCam.BumpCamera(0,-1);
		Player.Y-=1;
		if(!CheckCollide(2)){
			PlayerCam.BumpCamera(0,1);
			Player.Y++;
		}
		Player.dir=2;
		Player.Moving=true;
	}
	if(diObj->KeyPress(KEY_DOWN)==true) {
		PlayerCam.BumpCamera(0,1);
		Player.Y+=1;
		if(!CheckCollide(0)){
			PlayerCam.BumpCamera(0,-1);
			Player.Y--;
		}
		Player.dir=0;
		Player.Moving=true;
	}

	if(PlayerCam.CameraX<0) ChangeMapWalking(-1,0);
	if(PlayerCam.CameraX>3071) ChangeMapWalking(1,0);
	if(PlayerCam.CameraY<0)	ChangeMapWalking(0,-1);
	if(PlayerCam.CameraY>3071) ChangeMapWalking(0,1);
	
	//Check for the roof
	if(!CheckCollide(4)){
		if(!RoofOff){
			RoofOff=true;
			RoofTileX=RoofX;
			RoofTileY=RoofY;
			FlipRoof(RoofTileX,RoofTileY);
		}
	} else {
		if(RoofOff){
			RoofOff=false;
			FlipRoof(RoofTileX,RoofTileY);
		}
	}
	PlayerCam.BumpCamera(0,0);

	if(Player.Moving && Maps[MapWin[1][1]].BattleNum>0){
		while(randomCounter-->0){
			if(randBattleCounter < 1600) randBattleCounter++;
			else if(randBattleCounter < 3200) CheckBattle(20000.0);
			else if(randBattleCounter < 6500) CheckBattle(10000.0);
			else CheckBattle(1.0);
		}
	}

	return true;
}


//============
// Changing the Map
//============

void cDA2Game::ChangeMapWalking(int xdir, int ydir){

	int x,y;

	//must add special cases for some maps

	if(xdir<0){
		PlayerCam.CameraX+=3072;
		for(x=0;x<3;x++){
			for(y=0;y<3;y++){
				MapWin[x][y]-=1;
				if(MapWin[x][y]<0)MapWin[x][y]=0;
			}
		}
    if(MapWin[1][1]==483) MapWin[2][1]=484;
		SetRoofMask();
	}
	if(xdir>0){
		PlayerCam.CameraX-=3072;
		for(x=0;x<3;x++){
			for(y=0;y<3;y++){
				MapWin[x][y]+=1;
        if(MapWin[x][y]>482 && MapWin[x][y]<485){
          //do nothing;
        } else if(MapWin[x][y]>400){
          MapWin[x][y]=0;
        }
			}
		}
		SetRoofMask();
	}
	if(ydir<0){
		PlayerCam.CameraY+=3072;
		for(x=0;x<3;x++){
			MapWin[x][2]=MapWin[x][1];
			MapWin[x][1]=MapWin[x][0];
			MapWin[x][0]-=20;
      if(MapWin[x][0]<0){
        if(MapWin[x][1]==484) MapWin[x][0]=482;
        else if(MapWin[x][1]==485) MapWin[x][0]=484;
        else MapWin[x][0]=0;
      }
		}
		SetRoofMask();
	}
	if(ydir>0){
		PlayerCam.CameraY-=3072;
		for(x=0;x<3;x++){
			MapWin[x][0]=MapWin[x][1];
			MapWin[x][1]=MapWin[x][2];
			MapWin[x][2]+=20;
      if(MapWin[x][2]>400){
        if(MapWin[x][1]==482) MapWin[x][2]=484;
        else if(MapWin[x][1]==484) MapWin[x][2]=485;
        else MapWin[x][2]=0;
      }
		}
		SetRoofMask();
	}
	Player.Map=MapWin[1][1];
	Player.X=PlayerCam.CameraX;
	Player.Y=PlayerCam.CameraY;
	MusicObj->ChangeSong(Player.Map-1);
}

void cDA2Game::ChangeMap(int map, int xpos, int ypos){

	//Add special cases
	if(map<=400){
		MapWin[0][0]=map-21; if(MapWin[0][0]<0)MapWin[0][0]=0;
		MapWin[1][0]=map-20; if(MapWin[1][0]<0)MapWin[1][0]=0;
		MapWin[2][0]=map-19; if(MapWin[2][0]<0)MapWin[2][0]=0;
		MapWin[0][1]=map-1; if(MapWin[0][1]<0)MapWin[0][1]=0;
		MapWin[1][1]=map;
		MapWin[2][1]=map+1; if(MapWin[2][1]>400)MapWin[2][1]=0;
		MapWin[0][2]=map+19; if(MapWin[0][2]>400)MapWin[0][2]=0;
		MapWin[1][2]=map+20; if(MapWin[1][2]>400)MapWin[1][2]=0;
		MapWin[2][2]=map+21; if(MapWin[2][2]>400)MapWin[2][2]=0;
	}

	if(map>400){
		MapWin[0][0]=0; MapWin[1][0]=0; MapWin[2][0]=0;
		MapWin[0][1]=0; MapWin[1][1]=map; MapWin[2][1]=0;
		MapWin[0][2]=0; MapWin[1][2]=0; MapWin[2][2]=0;
    if(map==482) {
      MapWin[0][2]=483;
      MapWin[1][2]=484;
    } else if(map==483) {
      MapWin[2][0]=482;
      MapWin[2][1]=484;
      MapWin[2][2]=485;
    } else if(map==484) {
      MapWin[0][1]=483;
      MapWin[1][0]=484; 
      MapWin[1][2]=485;
    } else if(map==485){
      MapWin[0][0]=483;
      MapWin[1][0]=484;
    }
	}

	Player.Map=MapWin[1][1];
	Player.X=xpos;
	Player.Y=ypos;
	PlayerCam.SetCamera(xpos,ypos);
	SetRoofMask();
	MusicObj->ChangeSong(Player.Map-1);

}

//============
// Roof Handling functions
//============

void cDA2Game::SetRoofMask(){
	int x,y;
	int Tile;

	for(y=0;y<96;y++){
		for(x=0;x<96;x++){
			Tile = Maps[MapWin[1][1]].MapKey[Maps[MapWin[1][1]].MapArray[x][y][3]];
			if(Tile>0) RoofMask[x][y]=1;
			else RoofMask[x][y]=0;
		}
	}
	RoofOff=false;

}

//Recursive algorithm to set roof to 1 (visible) or 2 (hidden). 0=no roof.
void cDA2Game::FlipRoof(int X, int Y){

	if(RoofOff){
		if(RoofMask[X][Y]==1) {
			RoofMask[X][Y]=2;
			FlipRoof(X-1,Y);
			FlipRoof(X+1,Y);
			FlipRoof(X,Y-1);
			FlipRoof(X,Y+1);
		}
	} else {
		if(RoofMask[X][Y]==2) {
			RoofMask[X][Y]=1;
			FlipRoof(X-1,Y);
			FlipRoof(X+1,Y);
			FlipRoof(X,Y-1);
			FlipRoof(X,Y+1);
		}
	}

}


//=============
// Collision detection
//=============

bool cDA2Game::CheckCollide(int dir){

	//return true;
	int x,y,z,z1,z2;
	int Tile;
	SDL_Rect r;

  //checking movement or roof collisions
	if(dir<4){
		z1=1;
		z2=3;
	} else {
		z1=3;
		z2=4;
	}


	//clear the collision surface
  SDL_FillRect(collide, NULL, 0x000000);

	//Blt relevant tiles
	for(z=z1;z<z2;z++){
		for(y=PlayerCam.TilePosY-2;y<PlayerCam.TilePosY+2;y++){
			for(x=PlayerCam.TilePosX-2;x<PlayerCam.TilePosX+2;x++){
				if(y<0){
					if(x<0){
						Tile = Maps[MapWin[0][0]].MapKey[Maps[MapWin[0][0]].MapArray[x+96][y+96][z]];
					} else if(x>95){
						Tile = Maps[MapWin[2][0]].MapKey[Maps[MapWin[2][0]].MapArray[x-96][y+96][z]];
					} else {
						Tile = Maps[MapWin[1][0]].MapKey[Maps[MapWin[1][0]].MapArray[x][y+96][z]];
					}
				} else if(y>95){
					if(x<0){
						Tile = Maps[MapWin[0][2]].MapKey[Maps[MapWin[0][2]].MapArray[x+96][y-96][z]];
					} else if(x>95){
						Tile = Maps[MapWin[2][2]].MapKey[Maps[MapWin[2][2]].MapArray[x-96][y-96][z]];
					} else {
						Tile = Maps[MapWin[1][2]].MapKey[Maps[MapWin[1][2]].MapArray[x][y-96][z]];
					}
				} else {
					if(x<0){
						Tile = Maps[MapWin[0][1]].MapKey[Maps[MapWin[0][1]].MapArray[x+96][y][z]];
					} else if(x>95){
						Tile = Maps[MapWin[2][1]].MapKey[Maps[MapWin[2][1]].MapArray[x-96][y][z]];
					} else {
						Tile = Maps[MapWin[1][1]].MapKey[Maps[MapWin[1][1]].MapArray[x][y][z]];
					}
				}
				if(Tile==0) continue;
				if(z<3 && !ddGfx->vTile->at(Tile).solid) continue;
				if(Tile>2000) Tile+=animCounter;
				
				r.x=(x-(PlayerCam.TilePosX-2))*32-PlayerCam.OffsetX+256;
				r.y=(y-(PlayerCam.TilePosY-2))*32-PlayerCam.OffsetY+192;
				r.w=32;
				r.h=32;
				//ddObj->ddsb->BltFast((x-20)*32-5, (y+5)*32-5, ddGfx->Tiles[ddGfx->vTile->at(Tile).index], &ddGfx->vTile->at(Tile).r, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
				//ddObj->ddsc->BltFast(&r, ddGfx->Tiles[ddGfx->vTile->at(Tile).index], &ddGfx->vTile->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
				//ddObj->ddsc->Blt(&r, ddGfx->Tiles[ddGfx->vTile->at(Tile).index], &ddGfx->vTile->at(Tile).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
        SDL_BlitSurface(ddGfx->Tiles[ddGfx->vTile->at(Tile).index]->surface, &ddGfx->vTile->at(Tile).r, collide, &r);
			}
		}
    if(z == 1) ObjRender(collide, true);
		//if(z==1) ObjRender(ddObj->ddsc,true);
	}

	//Check for collision
	//ddObj->ddsc->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_READONLY, NULL);
  Uint32 pixelColor;
  Uint8 red, green, blue, alpha;

	switch(dir){
		case 0:
			for(x=312;x<328;x++){
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + 255 * collide->pitch + x*4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) return false;
				//if ( ((BYTE *) ddsd.lpSurface)[255 * ddsd.lPitch + x] !=0) {
				//	ddObj->ddsc->Unlock(NULL);
				//	return false;
				//}
			}
			break;
		case 1:
			for(y=224;y<256;y++){
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + y * collide->pitch + 327 * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) return false;
				//if ( ((BYTE *) ddsd.lpSurface)[y * ddsd.lPitch + 327] !=0) {
				//	ddObj->ddsc->Unlock(NULL);
				//	return false;
				//}
			}
			break;
		case 2:
			for(x=312;x<328;x++){
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + 224 * collide->pitch + x * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) return false;
				//if ( ((BYTE *) ddsd.lpSurface)[224 * ddsd.lPitch + x] !=0) {
				//	ddObj->ddsc->Unlock(NULL);
				//	return false;
				//}
			}
			break;
		case 3:
			for(y=224;y<256;y++){
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + y * collide->pitch + 312 * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) return false;
				//if ( ((BYTE *) ddsd.lpSurface)[y * ddsd.lPitch + 312] !=0) {
				//	ddObj->ddsc->Unlock(NULL);
				//	return false;
				//}
			}
			break;
		case 4:
			for(y=224;y<256;y++){
				//left
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + y * collide->pitch + 312 * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) {
				//if ( ((BYTE *) ddsd.lpSurface)[y * ddsd.lPitch + 312] !=0) {
					RoofX= (312-256+PlayerCam.OffsetX)/32 + PlayerCam.TilePosX-2;
					RoofY= (y-192+PlayerCam.OffsetY)/32 + PlayerCam.TilePosY-2;
					//ddObj->ddsc->Unlock(NULL);
					return false;
				}
				//right
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + y * collide->pitch + 327 * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) {
				//if ( ((BYTE *) ddsd.lpSurface)[y * ddsd.lPitch + 327] !=0) {
					RoofX= (327-256+PlayerCam.OffsetX)/32 + PlayerCam.TilePosX-2;
					RoofY= (y-192+PlayerCam.OffsetY)/32 + PlayerCam.TilePosY-2;
					//ddObj->ddsc->Unlock(NULL);
					return false;
				}
			}
			for(x=312;x<328;x++){
				//top
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + 224 * collide->pitch + x * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) {
				//if ( ((BYTE *) ddsd.lpSurface)[224 * ddsd.lPitch + x] !=0) {
					RoofX= (x-256+PlayerCam.OffsetX)/32 + PlayerCam.TilePosX-2;
					RoofY= (224-192+PlayerCam.OffsetY)/32 + PlayerCam.TilePosY-2;
					//ddObj->ddsc->Unlock(NULL);
					return false;
				}
				//bottom
        pixelColor = *(Uint32*)((Uint8*)collide->pixels + 255 * collide->pitch + x * 4);
        SDL_GetRGBA(pixelColor, collide->format, &red, &green, &blue, &alpha);
        if(red>0 || green > 0 || blue > 0 /*|| alpha > 0*/) {
				//if ( ((BYTE *) ddsd.lpSurface)[255 * ddsd.lPitch + x] !=0) {
					RoofX= (x-256+PlayerCam.OffsetX)/32 + PlayerCam.TilePosX-2;
					RoofY= (255-192+PlayerCam.OffsetY)/32 + PlayerCam.TilePosY-2;
					//ddObj->ddsc->Unlock(NULL);
					return false;
				}
			}
			break;

		default:
			break;
	}
	//ddObj->ddsc->Unlock(NULL);
	return true;
}


//============
// Additional Functions
//============
bool cDA2Game::DropItem(){
	bool bOccupied;
	int j,x,y;
	ObjectType newObj;
	int Tile;
	int TrueMap;
	int TrueX,TrueY;

	//drop item on ground if there is space.
	for(y=PlayerCam.TilePosY-1;y<PlayerCam.TilePosY+2;y++){
		for(x=PlayerCam.TilePosX-1;x<PlayerCam.TilePosX+2;x++){
			
			if(y<0){
				if(x<0){
					TrueMap=MapWin[0][0]; TrueX=x+96; TrueY=y+96;
				} else if(x>95){
					TrueMap=MapWin[2][0]; TrueX=x-96; TrueY=y+96;
				} else {
					TrueMap=MapWin[1][0]; TrueX=x; TrueY=y+96;
				}
			} else if(y>95){
				if(x<0){
					TrueMap=MapWin[0][2]; TrueX=x+96; TrueY=y-96;
				} else if(x>95){
					TrueMap=MapWin[2][2]; TrueX=x-96; TrueY=y-96;
				} else {
					TrueMap=MapWin[1][2]; TrueX=x; TrueY=y-96;
				}
			} else {
				if(x<0){
					TrueMap=MapWin[0][1]; TrueX=x+96; TrueY=y;
				} else if(x>95){
					TrueMap=MapWin[2][1]; TrueX=x-96; TrueY=y;
				} else {
					TrueMap=MapWin[1][1]; TrueX=x; TrueY=y;
				}
			}

			Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[TrueX][TrueY][1]];
			if(ddGfx->vTile->at(Tile).solid) continue;
			Tile = Maps[TrueMap].MapKey[Maps[TrueMap].MapArray[TrueX][TrueY][2]];
			if(ddGfx->vTile->at(Tile).solid) continue;

			bOccupied=false;
			for(j=0;j<Maps[TrueMap].ObjArray->size();j++){
				if(Maps[TrueMap].ObjArray->at(j).Xco==TrueX && Maps[TrueMap].ObjArray->at(j).Yco==TrueY){
					bOccupied=true;
					break;
				}
			}
			
			if(bOccupied) continue;
			newObj.HideTab=0;
			newObj.ObjectPic=Party.DropItem;
			newObj.Solid=0;
			newObj.Status=2;
			newObj.Xco=TrueX;
			newObj.Yco=TrueY;
			Maps[TrueMap].ObjArray->push_back(newObj);
			Party.RemoveItem(Party.DropItem);
			return true;
		}
	}
	return false;
}


//============
// Script Handling
//============

EventTag cDA2Game::NPCScript(int map, int npc){

	bool bOK,bShop=false,bInn=false,bTrain=false,bChangeMap=false,bBattle=false;
  int val1, val2, val3, val4;
	int scriptNum;
	int i;
	char str[1024];
	char *tok;
	
	preText=0;
	string s;

	FILE *f;
	f=fopen("data/tlk.dat","rt");
	long fpos = NPCFileIndex[map];
	fseek(f,fpos,SEEK_SET);

	while(!feof(f)){

		fgets(str,512,f);
		if(feof(f)) {
			fclose(f);
			return Failure;
		}
		if(strlen(str)==0) continue;
		if(str==NULL) continue;
		if(str[0]=='b') {
			fclose(f);
			return Failure;
		}
		
		tok=strtok(str,"\",");
 
		if(strcmp(tok,"*")==0) {

			//Get npc number
			fscanf(f,"\"%d\"\n",&val1);
			if(val1!=npc) continue;

			//Get AND flags
			fscanf(f,"\"%d\"\n",&val1);
			bOK=true;
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]!=val3) bOK=false;
			}
			if(!bOK) continue;

			//Get OR flags
			fscanf(f,"\"%d\"\n",&val1);
			bOK=true;
			if(val1>0) bOK=false;
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]==val3) bOK=true;
			}
			if(!bOK) continue;

			//Get !OR flags
			fscanf(f,"\"%d\"\n",&val1);
			bOK=true;
			if(val1>0) bOK=false;
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]!=val3) bOK=true;
			}
			if(!bOK) continue;

			//Get !AND flags
			bOK=true;
			fscanf(f,"\"%d\"\n",&val1);
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]==val3) bOK=false;
			}
			if(!bOK) continue;

			//Get script action
			fscanf(f,"\"%d\"\n",&scriptNum);
      switch(scriptNum){
			case 0:
				break;
			case 1:
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				//Maps[map].NPCArray[npc].NStatus=0;
				break;
			case 2:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.AddItem(val1)){
					s = "You have no room in your inventory! Sell or drop something first.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				//Maps[map].NPCArray[npc].NStatus=0;
				break;
			case 3:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.AddItem(val1)){
					s = "You have no room in your inventory! Sell or drop something first.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				break;
			case 4:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.RemoveItem(val1))	continue;
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				//Maps[map].NPCArray[npc].NStatus=0;
				break;
			case 5:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.RemoveItem(val1))	continue;
				break;
			case 6:
				fscanf(f,"\"%d\"\n",&val1);
				Party.Gold+=val1;
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				//Maps[map].NPCArray[npc].NStatus=0;
				break;
			case 7:
				fscanf(f,"\"%d\"\n",&val1);
				Party.Gold+=val1;
				break;
			case 8:
				fscanf(f,"\"%d\"\n",&val1);
				if(Party.Gold<val1){
					s = "No matter how many times you count your coins, you just don't have enough gold.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				Party.Gold-=val1;
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				//Maps[map].NPCArray[npc].NStatus=0;
				break;
			case 9:
				fscanf(f,"\"%d\"\n",&val1);
				if(Party.Gold<val1){
					s = "No matter how many times you count your coins, you just don't have enough gold.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				Party.Gold-=val1;
				break;
			case 10:
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(!Party.SwapItem(val1,val2)) continue;
				break;
      case 11:
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(Party.Gold<val1){
					s = "No matter how many times you count your coins, you just don't have enough gold.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				if(!Party.AddItem(val2)){
					s = "You have no room in your inventory! Sell or drop something first.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				Party.Gold-=val1;
				break;
			case 12:
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(!Party.RemoveItem(val1))	continue;
				Party.Gold+=val2;
				break;
			case 13:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.CheckItem(val1)) continue;
				break;
			case 14: //ChangeMap
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				fscanf(f,"\"%d\"\n",&val3);
				newMap.map=val1;
				newMap.x=val2+48;
				newMap.y=val3+48;
				bChangeMap=true;
				break;

			case 15: //barter
				shop.Clear();
				fscanf(f,"\"%d\"\n",&val1);
				shop.SetType(val1);
				fscanf(f,"\"%d\"\n",&val1);
				for(i=0;i<val1;i++){
					fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
					shop.SetItem(val2);
					shop.SetItem(val3);
				}
				bShop=true;
				break;

			case 16:  //Inn
        fscanf(f,"\"%d\"\n",&InnCost);
				sprintf(str,"Take a rest for %d gold pieces?",InnCost);
				dialog.SetWindow(198,155,str);
				bInn=true;
				break;

			case 17:  //Trainer
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				switch(val1){
				case 1:
					sprintf(str,"I can train your strength for %d gold pieces per training point.",val2);
					break;
				case 2:
					sprintf(str,"I can train your agility for %d gold pieces per training point.",val2);
					break;
				case 3:
					sprintf(str,"I can train your intelligence for %d gold pieces per training point.",val2);
					break;
				case 4:
					sprintf(str,"I can train your speed for %d gold pieces per training point.",val2);
					break;
				default:
					sprintf(str,"Training script error.");
					break;
				}
				dialog.SetWindow(198,155,str,false);
				trainDialog.SetWindow(198,155,val2,val1,&Party);
				bTrain=true;
				bTrainDialog=true;
				break;
        
			case 18:
				fscanf(f,"\"%d\"\n",&val1);
				if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				break;

			case 19:
				fscanf(f,"\"%d\"\n",&val1);
				if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
				if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				break;

			case 22: //Battle, then NPC disappears
				fscanf(f,"\"%d\"\n",&val1);
				if(val1==-1){
					fscanf(f,"\"%d\"\n",&preText);
					for(i=0;i<preText;i++){
						fgets(str,512,f);
						tok=strtok(str,"\"\n");
						s=tok;
						vText.push_back(s);
					}
					//fscanf(f,"\"%d\"\n",&val1);
				}
				fscanf(f,"\"%d\"\n",&val1);
				battleEngine.Load(val1);
        mainStack.Push(ScriptBattle);
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				bBattle=true;
				break;

			case 23: //NPC other map activates/deactivates
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(Maps[val1].NPCArray[val2].NStatus==0) Maps[val1].NPCArray[val2].NStatus=1;
				else Maps[val1].NPCArray[val2].NStatus=0;
				break;

			case 24: //rest the party
				Party.Players[0].HP=Party.Players[0].MaxHP;
				Party.Players[1].HP=Party.Players[1].MaxHP;
				Party.Players[2].HP=Party.Players[2].MaxHP;
				Party.Players[0].MP=Party.Players[0].MaxMP;
				break;

			case 25: //Battle, NPC stays
				fscanf(f,"\"%d\"\n",&val1);
				if(val1==-1){
					fscanf(f,"\"%d\"\n",&preText);
					for(i=0;i<preText;i++){
						fgets(str,512,f);
						tok=strtok(str,"\"\n");
						s=tok;
						vText.push_back(s);
					}
					//fscanf(f,"\"%d\"\n",&val1);
				}
				fscanf(f,"\"%d\"\n",&val1);
				battleEngine.Load(val1);
				mainStack.Push(ScriptBattle);
				bBattle=true;
				break;

			case 26: //Obj and NPC disappear
				fscanf(f,"\"%d\"\n",&val1);
				Maps[map].ObjArray->at(val1).Status=0;
				Maps[map].NPCArray[npc].NStatus=0;
				break;

			case 28: //Two objects, any map, change pic
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				Maps[map].ObjArray->at(val1).ObjectPic=val2;
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				Maps[map].ObjArray->at(val1).ObjectPic=val2;
				break;
        
			case 30: //Battle, then NPC and object vanish
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				battleEngine.Load(val2);
				mainStack.Push(ScriptBattle);
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				Maps[map].NPCArray[npc].NStatus=0;
				Maps[map].ObjArray->at(val1).Status=0;
				bBattle=true;
        break;
        
			case 31: //NPC disappears, & map changes
				killMe.map=map;
				killMe.num=npc;
				killMe.npc=true;
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				newMap.map=val1;
				newMap.x=val2+48;
				newMap.y=val3+48;
				bChangeMap=true;
				break;
				
			case 32: //De/activate object other map
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(Maps[val1].ObjArray->at(val2).Status==0) Maps[val1].ObjArray->at(val2).Status=1;
				else Maps[val1].ObjArray->at(val2).Status=0;
				break;

			default:
				break;
			}

			//Change flags
			if(!bShop){
				fscanf(f,"\"%d\"\n",&val1);
				for(i=0;i<val1;i++){
					fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
					Flags[val2]=val3;
				}
			}

			//Set up text boxes
			fscanf(f,"\"%d\"\n",&val1);
			if(val1==-1){
				int JournalID=0;
				bool bDoneJournal=false;
				while(!bDoneJournal){
					fscanf(f,"\"%d\"\n",&val1);
					switch(val1){
					case 0:
						fscanf(f,"\"%d\"\n",&val2);
						JournalID=journal.GetID(0);
						journal.SetID(JournalID,val2);
						journal.SetActive(JournalID,1);
						break;
					case 1:
						fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetDifficulty(JournalID,val3);
						break;
					case 2:
						fscanf(f,"\"%d\",\"%d\",\"%d\"\n",&val2,&val3,&val4);
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetEndPoint(JournalID,val3,val4);
						break;
					case 4:
						fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetMainQuest(JournalID,val3);
						break;
					case 5:
						fscanf(f,"\"%d\",",&val2);
						fgets(str,1024,f);
						tok=strtok(str,"\"\n");
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.AddText(JournalID,tok);
						break;
					case 6:
						fscanf(f,"\"%d\"\n",&val2);
						JournalID=journal.GetID(val2);
						if(JournalID>-1){
							journal.SetDifficulty(JournalID,0);
							journal.SetEndPoint(JournalID,0,0);
						}
						break;
					case 7:
						fscanf(f,"\"%d\",",&val2);
						fgets(str,1024,f);
						tok=strtok(str,"\"\n");
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetTitle(JournalID,tok);
						break;
					case 8:
						fscanf(f,"\"%d\",",&val2);
						fgets(str,1024,f);
						tok=strtok(str,"\"\n");
						JournalID=journal.GetID(val2);
						if(JournalID>-1) break;

						JournalID=journal.GetID(0);
						journal.SetID(JournalID,val2);
						journal.SetActive(JournalID,1);
						journal.SetTitle(JournalID,tok);
						tok=strtok(NULL,"\",\n");
						journal.SetDifficulty(JournalID,atoi(tok));
						tok=strtok(NULL,"\",\n");
						val3=atoi(tok);
						tok=strtok(NULL,"\",\n");
						journal.SetEndPoint(JournalID,val3,atoi(tok));
						tok=strtok(NULL,"\"\n"); //this should be an unwanted comma
						tok=strtok(NULL,"\"\n"); //do not parse commas because they are part of the text
						journal.AddText(JournalID,tok);
						tok=strtok(NULL,"\",\n");
						journal.SetMainQuest(JournalID,atoi(tok));
						break;
					case 9:
						bDoneJournal=true;
						break;
					default:
						break;
					}
				}
				journal.Reset();
				fscanf(f,"\"%d\"\n",&val1);
			}
     
			for(i=0;i<val1;i++){
				fgets(str,512,f);
				tok=strtok(str,"\"\n");
				s=tok;
				vText.push_back(s);
			}

			//Check for cutscenes
			fgets(str,512,f);
			tok=strtok(str,"\"\n");
			if(tok[0]=='%'){
				fscanf(f,"\"%d\"\n",&val1);
				cutScene.LoadScene(val1,Party.Players[0].PlayerName,Party.Players[1].PlayerName,Party.Players[2].PlayerName);
				mainStack.Push(CutSceneA);
			}

			break;

		}
	}

	fclose(f);
	if(bChangeMap) mainStack.Push(ChangeTheMap);
	if(bShop) return Barter;
	if(bInn) return Inn;
	if(bBattle) return BattleNow;
	if(bTrain) return Train;
	return Normal;

}


EventTag cDA2Game::ObjScript(int map, int obj){
	bool bOK,bSpell=false,bChangeMap=false,bBattle=false;
  int val1, val2, val3, val4;
	int scriptNum;
	int i;
	char str[1024];
	char *tok;

	preText=0;
	string s;

	FILE *f;
	f=fopen("data/osc.dat","rt");
	long fpos = ObjFileIndex[map];
	fseek(f,fpos,SEEK_SET);

	while(!feof(f)){

		fgets(str,512,f);
		if(feof(f)) {
			fclose(f);
			return Failure;
		}
		if(strlen(str)==0) continue;
		if(str[0]=='b') {
			fclose(f);
			return Failure;
		}
		
		tok=strtok(str,"\",");
 
		if(strcmp(tok,"*")==0) {

			//Get obj number
			fscanf(f,"\"%d\"\n",&val1);
			if(val1!=obj) continue;

			//Get AND flags
			fscanf(f,"\"%d\"\n",&val1);
			bOK=true;
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]!=val3) bOK=false;
			}
			if(!bOK) continue;

			//Get OR flags
			fscanf(f,"\"%d\"\n",&val1);
			bOK=true;
			if(val1>0) bOK=false;
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]==val3) bOK=true;
			}
			if(!bOK) continue;

			//Get !OR flags
			fscanf(f,"\"%d\"\n",&val1);
			bOK=true;
			if(val1>0) bOK=false;
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]!=val3) bOK=true;
			}
			if(!bOK) continue;

			//Get !AND flags
			bOK=true;
			fscanf(f,"\"%d\"\n",&val1);
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				if(Flags[val2]==val3) bOK=false;
			}
			if(!bOK) continue;

			//Get script action
			fscanf(f,"\"%d\"\n",&scriptNum);
      switch(scriptNum){
			case 0:
				break;
			case 1:
				killMe.map=map;
				killMe.num=obj;
				killMe.npc=false;
				//Maps[map].ObjArray->at(obj).Status=0;
				break;
			case 2:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.AddItem(val1)){
					s = "You have no room in your inventory! Sell or drop something first.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				killMe.map=map;
				killMe.num=obj;
				killMe.npc=false;
				//Maps[map].ObjArray->at(obj).Status=0;
				break;
			case 3:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.AddItem(val1)){
					s = "You have no room in your inventory! Sell or drop something first.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				break;
			case 4:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.RemoveItem(val1))	continue;
				killMe.map=map;
				killMe.num=obj;
				killMe.npc=false;
				//Maps[map].ObjArray->at(obj).Status=0;
				break;
			case 5:
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.RemoveItem(val1))	continue;
				break;
			case 6:
				fscanf(f,"\"%d\"\n",&val1);
				Party.Gold+=val1;
				killMe.map=map;
				killMe.num=obj;
				killMe.npc=false;
				//Maps[map].ObjArray->at(obj).Status=0;
				break;
			case 7:
				fscanf(f,"\"%d\"\n",&val1);
				Party.Gold+=val1;
				break;
			case 8:
				fscanf(f,"\"%d\"\n",&val1);
				if(Party.Gold<val1){
					s = "No matter how many times you count your coins, you just don't have enough gold.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				Party.Gold-=val1;
				killMe.map=map;
				killMe.num=obj;
				killMe.npc=false;
				//Maps[map].ObjArray->at(obj).Status=0;
				break;
			case 9:
				fscanf(f,"\"%d\"\n",&val1);
				if(Party.Gold<val1){
					s = "No matter how many times you count your coins, you just don't have enough gold.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				Party.Gold-=val1;
				break;
			case 10:
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(!Party.SwapItem(val1,val2)) continue;
				break;
      case 11:
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(Party.Gold<val1){
					s = "No matter how many times you count your coins, you just don't have enough gold.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				if(!Party.AddItem(val2)){
					s = "You have no room in your inventory! Sell or drop something first.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				}
				Party.Gold-=val1;
				break;
			case 12: //Trade item for gold
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(!Party.RemoveItem(val1))	continue;
				Party.Gold+=val2;
				break;

			case 13: //Have item in invetory
				fscanf(f,"\"%d\"\n",&val1);
				if(!Party.CheckItem(val1)) continue;
				break;

			case 14: //ChangeMap
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				fscanf(f,"\"%d\"\n",&val3);
				newMap.map=val1;
				newMap.x=val2+48;
				newMap.y=val3+48;
				bChangeMap=true;
				break;

			case 15: //change object pic
        fscanf(f,"\"%d\"\n",&val1);
        Maps[map].ObjArray->at(obj).ObjectPic=val1;
				break;

			case 16:  //activate portal with stone
        fscanf(f,"\"%d\"\n",&val1); //this value not used
				if(!Party.RemoveItem(233)) {
					s = "You need a portal stone to activate the portal.";
					vText.push_back(s);
					fclose(f);
					return Normal;
				} else {
					Maps[map].ObjArray->at(obj).ObjectPic=459;
				}
				break;

			case 17: //Buy spell
				fscanf(f,"\"%d\"\n",&SpellNum);
			  fscanf(f,"\"%d\"\n",&SpellCost);
				sprintf(str,"Hidden among the shelves is the %s spell. You can purchase a copy for %d gold pieces.",Party.Spells[SpellNum].SpellName,SpellCost);
				dialog.SetWindow(198,155,str);
				bSpell=true;
				break;

			case 18: //De/Activate one object
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				break;

			case 19: //De/Activate two objects
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				break;

			case 20: //De/Activate three objects
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				break;

			case 21: //De/Activate four objects
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				fscanf(f,"\"%d\"\n",&val1);
        if(Maps[map].ObjArray->at(val1).Status==0) Maps[map].ObjArray->at(val1).Status=1;
				else Maps[map].ObjArray->at(val1).Status=0;
				break;

			case 22:  //Battle then disappear
				fscanf(f,"\"%d\"\n",&val1);
				if(val1==-1){
					fscanf(f,"\"%d\"\n",&preText);
					for(i=0;i<preText;i++){
						fgets(str,512,f);
						tok=strtok(str,"\"\n");
						s=tok;
						vText.push_back(s);
					}
					//fscanf(f,"\"%d\"\n",&val1);
				} 
				fscanf(f,"\"%d\"\n",&val1);
				battleEngine.Load(val1);
        if(val1==61) postCutScene=1; //special case, end of game
				mainStack.Push(ScriptBattle);
				killMe.map=map;
				killMe.num=obj;
				killMe.npc=false;
				bBattle=true;
				break;

			case 23: //De/Activate object on another map
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				if(Maps[val1].ObjArray->at(val2).Status==0) Maps[val1].ObjArray->at(val2).Status=1;
				else Maps[val1].ObjArray->at(val2).Status=0;
				break;

			case 24: //rest party
				Party.Players[0].HP=Party.Players[0].MaxHP;
				Party.Players[1].HP=Party.Players[1].MaxHP;
				Party.Players[2].HP=Party.Players[2].MaxHP;
				Party.Players[0].MP=Party.Players[0].MaxMP;
				break;

			case 25: //battle, object remains
				fscanf(f,"\"%d\"\n",&val1);
				if(val1==-1){
					fscanf(f,"\"%d\"\n",&preText);
					for(i=0;i<preText;i++){
						fgets(str,512,f);
						tok=strtok(str,"\"\n");
						s=tok;
						vText.push_back(s);
						//vEventQueue.push_back(TextBox);
					}
					//fscanf(f,"\"%d\"\n",&val1);
				}
				fscanf(f,"\"%d\"\n",&val1);
				battleEngine.Load(val1);
				mainStack.Push(ScriptBattle);
				bBattle=true;
				break;

			case 27: //change object pic of specified on this map
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				Maps[map].ObjArray->at(val1).ObjectPic=val2;
				break;

			case 28: //change two object pics
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				Maps[map].ObjArray->at(val1).ObjectPic=val2;
				fscanf(f,"\"%d\"\n",&val1);
				fscanf(f,"\"%d\"\n",&val2);
				Maps[map].ObjArray->at(val1).ObjectPic=val2;
				break;

			case 29: //copy spell (free spell)
				fscanf(f,"\"%d\"\n",&val1);
				if(Party.Players[0].HP<1 && Party.Players[1].HP>0){
					sprintf(str,"$ finds what appears to be a spell, but @ is dead and thus it cannot be transcribed. She puts it back.");
					vText.push_back(str);
				} else if(Party.Players[0].HP<1 && Party.Players[2].HP>0){
					sprintf(str,"^ finds what appears to be a spell, but @ is dead and thus it cannot be transcribed. He puts it back.");
					vText.push_back(str);
				} else if(Party.Spells[val1].Known){
					sprintf(str,"@ finds a copy of the %s spell. Because he already knows the spell, he puts it back.",Party.Spells[val1].SpellName);
					vText.push_back(str);
				} else {
					Party.Spells[val1].Known=true;
					Party.SpellList[Party.SpellCount++]=val1;
					sprintf(str,"@ finds a copy of the %s spell and quickly transcribes it to his spell book.",Party.Spells[val1].SpellName);
					vText.push_back(str);
				}
				break;

			case 32: //De/Activate one object on other map
				fscanf(f,"\"%d\",\"%d\"\n",&val1,&val2);
        if(Maps[val1].ObjArray->at(val2).Status==0) Maps[val1].ObjArray->at(val2).Status=1;
				else Maps[val1].ObjArray->at(val2).Status=0;
				break;

			case 33: //battle script, change object pic
				fscanf(f,"\"%d\"\n",&val1);
				battleEngine.Load(val1);
				mainStack.Push(ScriptBattle);
				fscanf(f,"\"%d\"\n",&val1);
				Maps[map].ObjArray->at(obj).ObjectPic=val1;
				bBattle=true;
				break;

			case 34: //Special case
				fscanf(f,"\"%d\"\n",&val1);
				if(Maps[map].ObjArray->at(obj).ObjectPic==546) {
					Flags[val1] += 1;
					Maps[map].ObjArray->at(obj).ObjectPic=547;
				} else {
					if(Flags[val1]==0){
						s = "The container is empty. You need to find one of the blue orbs.";
						vText.push_back(s);
						fclose(f);
						return Normal;
					} else {
						Flags[val1] -= 1;
						Maps[map].ObjArray->at(obj).ObjectPic=546;
					}
				}
				fscanf(f,"\"%d\",\"%d\"\n",&val1,&val2);
				if(Maps[map].ObjArray->at(val1).Status==0){
					Maps[map].ObjArray->at(val1).Status=1;
					Maps[map].ObjArray->at(val2).Status=1;
				} else {
					Maps[map].ObjArray->at(val1).Status=0;
					Maps[map].ObjArray->at(val2).Status=0;
				}
				break;
			default:
				break;
			}

			//Change flags
			fscanf(f,"\"%d\"\n",&val1);
			for(i=0;i<val1;i++){
				fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
				Flags[val2]=val3;
			}

			//Set up text boxes
			fscanf(f,"\"%d\"\n",&val1);
			if(val1==-1){
				int JournalID=0;
				bool bDoneJournal=false;
				while(!bDoneJournal){
					fscanf(f,"\"%d\"\n",&val1);
					switch(val1){
					case 0:
						fscanf(f,"\"%d\"\n",&val2);
						JournalID=journal.GetID(0);
						journal.SetID(JournalID,val2);
						journal.SetActive(JournalID,1);
						break;
					case 1:
						fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetDifficulty(JournalID,val3);
						break;
					case 2:
						fscanf(f,"\"%d\",\"%d\",\"%d\"\n",&val2,&val3,&val4);
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetEndPoint(JournalID,val3,val4);
						break;
					case 4:
						fscanf(f,"\"%d\",\"%d\"\n",&val2,&val3);
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetMainQuest(JournalID,val3);
						break;
					case 5:
						fscanf(f,"\"%d\",",&val2);
						fgets(str,1024,f);
						tok=strtok(str,"\"\n");
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.AddText(JournalID,tok);
						break;
					case 6:
						fscanf(f,"\"%d\"\n",&val2);
						JournalID=journal.GetID(val2);
						if(JournalID>-1){
							journal.SetDifficulty(JournalID,0);
							journal.SetEndPoint(JournalID,0,0);
						}
						break;
					case 7:
						fscanf(f,"\"%d\",",&val2);
						fgets(str,1024,f);
						tok=strtok(str,"\"\n");
						JournalID=journal.GetID(val2);
						if(JournalID>-1) journal.SetTitle(JournalID,tok);
						break;
					case 8:
						fscanf(f,"\"%d\",",&val2);
						fgets(str,1024,f);
						tok=strtok(str,"\"\n");
						JournalID=journal.GetID(val2);
						if(JournalID>-1) break;

						JournalID=journal.GetID(0);
						journal.SetID(JournalID,val2);
						journal.SetActive(JournalID,1);
						journal.SetTitle(JournalID,tok);
						tok=strtok(NULL,"\",\n");
						journal.SetDifficulty(JournalID,atoi(tok));
						tok=strtok(NULL,"\",\n");
						val3=atoi(tok);
						tok=strtok(NULL,"\",\n");
						journal.SetEndPoint(JournalID,val3,atoi(tok));
						tok=strtok(NULL,"\"\n"); //this should be an unwanted comma
						tok=strtok(NULL,"\"\n"); //do not parse commas because they are part of the text
						journal.AddText(JournalID,tok);
						tok=strtok(NULL,"\",\n");
						journal.SetMainQuest(JournalID,atoi(tok));
						break;
					case 9:
						bDoneJournal=true;
						break;
					default:
						break;
					}
				}
				journal.Reset();
				fscanf(f,"\"%d\"\n",&val1);
			}
      
			for(i=0;i<val1;i++){
				fgets(str,512,f);
				tok=strtok(str,"\"\n");
				s=tok;
				vText.push_back(s);
			}

			//Check for cutscenes
			fgets(str,512,f);
			tok=strtok(str,"\"\n");
			if(tok[0]=='%'){
				fscanf(f,"\"%d\"\n",&val1);
				cutScene.LoadScene(val1,Party.Players[0].PlayerName,Party.Players[1].PlayerName,Party.Players[2].PlayerName);
				if(postCutScene==0) mainStack.Push(CutSceneA);
			}

			break;

		}
	}

	fclose(f);
	if(bChangeMap) mainStack.Push(ChangeTheMap);
	if(bBattle) return BattleNow;
	if(bSpell) return Spell;
	return Normal;

}

void cDA2Game::CheckBattle(double chance){
	int i;

	i=((int)(chance*rand()/(RAND_MAX+1.0)));

	if( i < 50 ){

		//give the option
		i=((int)(100.0*rand()/(RAND_MAX+1.0)));

		if( i < 50 ) {
			dialog.SetWindow(198,155,"You chance upon some enemies. By Krom's marbles, they do not see you! Shall you attack?");
			mainStack.Push(BattleChoiceDialog);
		} else {
			dialog.SetWindow(198,155,"Enemies surprise you!",false);
			mainStack.Push(SurpriseAttackDialog);
		}
		randBattleCounter=0;

	} else {
		randBattleCounter++;
	}

}