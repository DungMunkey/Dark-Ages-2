#include "DA2Party.h"

cDA2Party::cDA2Party(){
	int i;
	for(i=0;i<40;i++) PartyItems[i]=0;
	Gold=0;
	ActivePlayer=0;
	packOffset=0;
	Selection=-1;
	Highlight=-1;
	TakeOffItem=-1;
	BackRow[0]=BackRow[1]=BackRow[2]=false;
}

cDA2Party::~cDA2Party(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	ItemList=NULL;
}

bool cDA2Party::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cItemController *itm, int *cc){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((ItemList=itm)==NULL) return false;
  if((colorCounter=cc) == NULL) return false;
	if(!text.Init(gfx)) return false;
	if(!box.Init(d,ddGfx)) return false;
	if(!window.Init(d,ddGfx)) return false;

	//Close button
	clickList[0].x=485; clickList[0].y=416; clickList[0].w=139; clickList[0].h=48;
	//Previous character
	clickList[1].x=25; clickList[1].y=25; clickList[1].w=16; clickList[1].h=38;
	//Next character
	clickList[2].x=322; clickList[2].y=25; clickList[2].w=16; clickList[2].h=38;
	//Scroll item list up
  clickList[3].x=599; clickList[3].y=215; clickList[3].w=16; clickList[3].h=16;
	//Scroll item list down
  clickList[4].x=599; clickList[4].y=391; clickList[4].w=16; clickList[4].h=16;
	//Item List
  clickList[5].x=360; clickList[5].y=231; clickList[5].w=239; clickList[5].h=159;
	//Weapon
  clickList[6].x=41; clickList[6].y=275; clickList[6].w=32; clickList[6].h=32;
	//Shield
  clickList[7].x=291; clickList[7].y=274; clickList[7].w=32; clickList[7].h=32;
	//Armor
  clickList[8].x=74; clickList[8].y=182; clickList[8].w=32; clickList[8].h=32;
	//Helmet
  clickList[9].x=166; clickList[9].y=149; clickList[9].w=32; clickList[9].h=32;
	//Boots
  clickList[10].x=166; clickList[10].y=399; clickList[10].w=32; clickList[10].h=32;
	//Hands
  clickList[11].x=258; clickList[11].y=182; clickList[11].w=32; clickList[11].h=32;
	//Legs
  clickList[12].x=74; clickList[12].y=366; clickList[12].w=32; clickList[12].h=32;
	//Ring
  clickList[13].x=258; clickList[13].y=366; clickList[13].w=32; clickList[13].h=32;
	//Potion
  clickList[14].x=166; clickList[14].y=275; clickList[14].w=32; clickList[14].h=32;
	//Drop Item
  clickList[15].x=258; clickList[15].y=420; clickList[15].w=32; clickList[15].h=32;
	//Close button
	clickList[16].x=347; clickList[16].y=416; clickList[16].w=138; clickList[16].h=48;

	//The spell list
	clickSpell[0].x=186; clickSpell[0].w=252; clickSpell[0].y=120; clickSpell[0].h=16;
	clickSpell[1].x=186; clickSpell[1].w=252; clickSpell[1].y=136; clickSpell[1].h=16;
	clickSpell[2].x=186; clickSpell[2].w=252; clickSpell[2].y=152; clickSpell[2].h=16;
	clickSpell[3].x=186; clickSpell[3].w=252; clickSpell[3].y=168; clickSpell[3].h=16;
	clickSpell[4].x=186; clickSpell[4].w=252; clickSpell[4].y=184; clickSpell[4].h=16;
	clickSpell[5].x=186; clickSpell[5].w=252; clickSpell[5].y=200; clickSpell[5].h=16;
	clickSpell[6].x=186; clickSpell[6].w=252; clickSpell[6].y=216; clickSpell[6].h=16;
	clickSpell[7].x=186; clickSpell[7].w=252; clickSpell[7].y=232; clickSpell[7].h=16;
	clickSpell[8].x=186; clickSpell[8].w=252; clickSpell[8].y=248; clickSpell[8].h=16;
	clickSpell[9].x=186; clickSpell[9].w=252; clickSpell[9].y=264; clickSpell[9].h=16;
	clickSpell[10].x=186; clickSpell[10].w=252; clickSpell[10].y=280; clickSpell[10].h=16;
	clickSpell[11].x=186; clickSpell[11].w=252; clickSpell[11].y=296; clickSpell[11].h=16;
	clickSpell[12].x=186; clickSpell[12].w=252; clickSpell[12].y=312; clickSpell[12].h=16;
	clickSpell[13].x=186; clickSpell[13].w=252; clickSpell[13].y=328; clickSpell[13].h=16;
	clickSpell[14].x=186; clickSpell[14].w=252; clickSpell[14].y=344; clickSpell[14].h=16;

	//up/down buttons
  clickSpell[15].x=302; clickSpell[15].w=16; clickSpell[15].y=368; clickSpell[15].h=16;
  clickSpell[16].x=322; clickSpell[16].w=16; clickSpell[16].y=368; clickSpell[16].h=16;

	//done button
  clickSpell[17].x=393; clickSpell[17].w=61; clickSpell[17].y=364; clickSpell[17].h=24;

	//scroll buttons
  clickSpell[18].x=442; clickSpell[18].w=16; clickSpell[18].y=120; clickSpell[18].h=16;
  clickSpell[19].x=442; clickSpell[19].w=16; clickSpell[19].y=344; clickSpell[19].h=16;


	window.SetSize(170,88,300,316);
	bShowSpellList=false;
  
	return true;
}

bool cDA2Party::AddItem(int item){
	int i;
	for(i=0;i<40;i++){
		if(PartyItems[i]==0){
			PartyItems[i]=item;
			return true;
		}
	}
	return false;
}

bool cDA2Party::CheckItem(int item){
	int i;
	for(i=0;i<40;i++){
		if(PartyItems[i]==item) return true;
	}
	return false;
}

bool cDA2Party::RemoveItem(int item){
	int i;
	for(i=0;i<40;i++){
		if(PartyItems[i]==item){
			PartyItems[i]=0;
			return true;
		}
	}
	return false;
}

bool cDA2Party::SwapItem(int itemOld, int itemNew){
	int i;
	for(i=0;i<40;i++){
		if(PartyItems[i]==itemOld){
			PartyItems[i]=itemNew;
			return true;
		}
	}
	return false;
}

void cDA2Party::CalcAttributes(){
	//Mod Str
	Players[ActivePlayer].ModStr=Players[ActivePlayer].BaseStr;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Armor].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Boots].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Hands].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Legs].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Ring].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Shield].StrMod;
	Players[ActivePlayer].ModStr+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].StrMod;

	//ModAgl
	Players[ActivePlayer].ModAgl=Players[ActivePlayer].BaseAgl;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Armor].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Boots].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Hands].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Legs].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Ring].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Shield].AglMod;
	Players[ActivePlayer].ModAgl+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].AglMod;

	//ModInt
	Players[ActivePlayer].ModInt=Players[ActivePlayer].BaseInt;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Armor].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Boots].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Hands].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Legs].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Ring].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Shield].IntMod;
	Players[ActivePlayer].ModInt+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].IntMod;

	//ModSpeed
	Players[ActivePlayer].ModSpeed=Players[ActivePlayer].BaseSpeed;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Armor].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Boots].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Hands].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Legs].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Ring].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Shield].SpeedMod;
	Players[ActivePlayer].ModSpeed+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].SpeedMod;

	//MaxHP
	Players[ActivePlayer].MaxHP=Players[ActivePlayer].ModStr*2;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Armor].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Boots].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Hands].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Legs].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Ring].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Shield].HPMod;
	Players[ActivePlayer].MaxHP+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].HPMod;

	//MaxMP
	if(ActivePlayer==0){
		Players[ActivePlayer].MaxMP=Players[ActivePlayer].ModInt*2;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Armor].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Boots].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Hands].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Legs].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Ring].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Shield].MPMod;
		Players[ActivePlayer].MaxMP+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].MPMod;
	}

	//Armor
	Players[ActivePlayer].Armor=ItemList->Items[Players[ActivePlayer].Gear.Armor].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Boots].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Hands].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Legs].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Ring].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Shield].ArmorStr;
	Players[ActivePlayer].Armor+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].ArmorStr;

	//Encumbrance
	Players[ActivePlayer].Encumbrance=ItemList->Items[Players[ActivePlayer].Gear.Armor].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Boots].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Hands].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Helmet].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Legs].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Ring].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Shield].ArmorEncumb;
	Players[ActivePlayer].Encumbrance+=ItemList->Items[Players[ActivePlayer].Gear.Weapon].ArmorEncumb;

	if(Players[ActivePlayer].ModAgl<1) Players[ActivePlayer].ModAgl=1;
	if(Players[ActivePlayer].ModStr<1) Players[ActivePlayer].ModStr=1;
	if(Players[ActivePlayer].ModInt<1) Players[ActivePlayer].ModInt=1;
	if(Players[ActivePlayer].ModSpeed<1) Players[ActivePlayer].ModSpeed=1;
	if(Players[ActivePlayer].MaxHP<1) Players[ActivePlayer].MaxHP=1;
	if(Players[ActivePlayer].MaxMP<1 && ActivePlayer==0) Players[ActivePlayer].MaxMP=1;
	if(Players[ActivePlayer].Armor<0) Players[ActivePlayer].Armor=0;
	if(Players[ActivePlayer].Encumbrance<0) Players[ActivePlayer].Encumbrance=0;

};

bool cDA2Party::Render(){
	SDL_Rect r;
	int tile;
	int i,j;
	int x,y;
	bool bMatch=false;
	char str[128];
	double damMin,damMax;
	int WeapStr;

	//display the stat screen
  SDL_RenderClear(display->renderer);
  SDL_RenderCopy(display->renderer, ddGfx->Stats[ActivePlayer]->texture, NULL, NULL);
	//ddObj->ddsb->Blt(&r, ddGfx->Stats[ActivePlayer], &r, DDBLT_WAIT,NULL);

	//display character name
	text.drawText(display->renderer,70,36,0,Players[ActivePlayer].PlayerName);

	//display stats
	sprintf(str,"Level: %d\n\nHP:  %d/%d\nMP:  %d/%d\n\nEXP: %d\n\nSkl: %d\nTP:  %d",Players[ActivePlayer].Level,Players[ActivePlayer].HP,Players[ActivePlayer].MaxHP,
																															 Players[ActivePlayer].MP,Players[ActivePlayer].MaxMP,Players[ActivePlayer].Experience,
																															 Players[ActivePlayer].SAP+1, Players[ActivePlayer].TP);
  text.drawText(display->renderer, 376, 30, 0, str);

	sprintf(str,"Gold: %d\n\nStr: %d/%d\nAgl: %d/%d\nInt: %d/%d\nSpd: %d/%d\n\nAL:  %d\nEnc: %d/%d",Gold,Players[ActivePlayer].BaseStr,Players[ActivePlayer].ModStr,Players[ActivePlayer].BaseAgl,
																																			 Players[ActivePlayer].ModAgl,Players[ActivePlayer].BaseInt,Players[ActivePlayer].ModInt,
																																			 Players[ActivePlayer].BaseSpeed,Players[ActivePlayer].ModSpeed,
																																			 Players[ActivePlayer].Armor,Players[ActivePlayer].Encumbrance/Players[ActivePlayer].ModStr/4,
																																			 Players[ActivePlayer].Encumbrance);
  text.drawText(display->renderer, 500, 30, 0, str);

	WeapStr=ItemList->Items[Players[ActivePlayer].Gear.Weapon].WeaponStr;
	if(WeapStr<1) WeapStr=1;
	if(ActivePlayer==2)	damMax = (WeapStr * Players[ActivePlayer].ModStr)/20.0 + 1.0;
	else damMax = (WeapStr * Players[ActivePlayer].ModStr)/30.0 + 1.0;
	damMin = damMax*0.85;
	if(damMin<1.0) damMin=1.01;
	if(damMax<1.0) damMax=1.01;
	sprintf(str,"Dam: %d - %d",(int)damMin,(int)(damMax+0.5));
  text.drawText(display->renderer, 376, 175, 0, str);

	//True thief has multi-attack
	if(ActivePlayer==1){
		i = (int)(0.02 * (float)Players[ActivePlayer].ModAgl);
		if(i==0) {
      text.drawText(display->renderer, 500, 175, 0, "Attacks: 1");
		} else {
			sprintf(str,"Attacks: %d - %d",i,i+1);
      text.drawText(display->renderer, 500, 175, 0, str);
		}
	}
    

	//display player items
  r.w=32;
  r.h=32;
	//armor
	r.y=182;	r.x=74;	
	tile=Players[ActivePlayer].Gear.Armor;
  if(tile>0 && TakeOffItem != 3) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r);
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index],&ddGfx->vObj->at(tile).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	//weapon
	r.y=275;	r.x=41;	
	tile=Players[ActivePlayer].Gear.Weapon;
  if(tile>0 && TakeOffItem != 1) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//helmet
	r.y=149;	r.x=166;	
	tile=Players[ActivePlayer].Gear.Helmet;
  if(tile>0 && TakeOffItem != 4) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//hands
	r.y=182;	r.x=258;	
	tile=Players[ActivePlayer].Gear.Hands;
  if(tile>0 && TakeOffItem != 6) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//shield
	r.y=274;	r.x=291;	
	tile=Players[ActivePlayer].Gear.Shield;
  if(tile>0 && TakeOffItem != 2) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//ring
	r.y=366;	r.x=258;	
	tile=Players[ActivePlayer].Gear.Ring;
  if(tile>0 && TakeOffItem != 8) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//boots
	r.y=399;	r.x=166;	
	tile=Players[ActivePlayer].Gear.Boots;
  if(tile>0 && TakeOffItem != 5) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//legs
	r.y=366;	r.x=74;
	tile=Players[ActivePlayer].Gear.Legs;
  if(tile>0 && TakeOffItem != 7) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
  //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

	//Display the Party Items
	text.drawText(display->renderer,360,215,0,"Shared Inventory:");
	for(y=0;y<4;y++){
		for(x=0;x<6;x++){
			r.x=x*40+360+4;
			r.y=y*40+231+4;
			if(x+y*6+packOffset==40) break;
			tile=PartyItems[x+y*6+packOffset];
      if(tile>0 && x + y * 6 + packOffset != Selection) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(tile).index]->texture, &ddGfx->vObj->at(tile).r, &r); 
      //ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(tile).index], &ddGfx->vObj->at(tile).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
      else SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[16], &r);
      //ddObj->ddsb->Blt(&r, ddGfx->Windows,&ddGfx->aWindows[16],DDBLT_WAIT,NULL);
		}
	}

	if(!bShowSpellList){
	
		//Display item info if moused-over
		if(Selection<0 && TakeOffItem<0){

			bMatch=false;
			for(j=5;j<14;j++){
				if(diObj->CheckMousePos(clickList[j])){
					bMatch=true;
					break;
				}
			}
			if(bMatch){
				switch(j){
				case 5:
					i=(diObj->MouseY()-231)/40 * 6;
					i+=(diObj->MouseX()-360)/40;
					i+=packOffset;
					if(i<40){
						x=i%6*40+360;
						y=(i-packOffset)/6*40+231;
						if(TakeOffItem<0 && Selection<0) box.DrawBox(x,y,x+40,y+40,120,false,*colorCounter);
						if(PartyItems[i]>0)	ItemWinRender(363,40,PartyItems[i]);
					}
					break;
				case 6:
					if(Players[ActivePlayer].Gear.Weapon>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Weapon);
					break;
				case 7:
					if(Players[ActivePlayer].Gear.Shield>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Shield);
					break;
				case 8:
					if(Players[ActivePlayer].Gear.Armor>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Armor);
					break;
				case 9:
					if(Players[ActivePlayer].Gear.Helmet>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Helmet);
					break;
				case 10:
					if(Players[ActivePlayer].Gear.Boots>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Boots);
					break;
				case 11:
					if(Players[ActivePlayer].Gear.Hands>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Hands);
					break;
				case 12:
					if(Players[ActivePlayer].Gear.Legs>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Legs);
					break;
				case 13:
					if(Players[ActivePlayer].Gear.Ring>0) ItemWinRender(363,40,Players[ActivePlayer].Gear.Ring);
					break;
				default:
					break;
				}
			}
		}

		//Highlight selected item location
		if(Highlight>-1){
      r.x=clickList[Highlight].x - 1;
      r.y=clickList[Highlight].y - 1;
      r.w=clickList[Highlight].w + 1;
      r.h=clickList[Highlight].h + 1;
      box.DrawBox(r, 80, false, *colorCounter);
		}

	} else {

		//Display the spell list
		window.DrawWindow();
		if(Selection>-1) box.DrawBox(clickSpell[Selection],166,true);	
		for(j=listOffset;j<SpellCount && j<listOffset+15;j++){
			text.drawText(display->renderer,200,120+(j-listOffset)*16,0,Spells[SpellList[j]].SpellName);
		}
		if(Selection >-1){
      if(listOffset + Selection>0) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &clickSpell[15]);
      //ddObj->ddsb->Blt(&clickSpell[15], ddGfx->Windows, &ddGfx->aWindows[11], DDBLT_WAIT, NULL);
      if(listOffset + Selection<SpellCount - 1) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &clickSpell[16]);
      //ddObj->ddsb->Blt(&clickSpell[16], ddGfx->Windows, &ddGfx->aWindows[12], DDBLT_WAIT, NULL);
		}
    if(SpellCount - listOffset>15) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &clickSpell[19]);
    //ddObj->ddsb->Blt(&clickSpell[19],ddGfx->Windows,&ddGfx->aWindows[12],DDBLT_WAIT,NULL);
    if(listOffset>0) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &clickSpell[17]); 
    //ddObj->ddsb->Blt(&clickSpell[18], ddGfx->Windows, &ddGfx->aWindows[11], DDBLT_WAIT, NULL);
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[15], &clickSpell[17]); 
    //ddObj->ddsb->Blt(&clickSpell[17], ddGfx->Windows, &ddGfx->aWindows[15], DDBLT_WAIT, NULL);

	}

	//display the mouse cursor && selected item
	r.y=diObj->MouseY()-16;
	r.x=diObj->MouseX()-16;
	r.h=32;
	r.w=32;
	if(Selection>-1 && !bShowSpellList) {
    SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(PartyItems[Selection]).index]->texture, &ddGfx->vObj->at(PartyItems[Selection]).r, &r);
		//ddObj->ddsb->Blt(&r,ddGfx->Objects[ddGfx->vObj->at(PartyItems[Selection]).index],&ddGfx->vObj->at(PartyItems[Selection]).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
    r.x=clickList[15].x - 1;
    r.y=clickList[15].y - 1;
    r.w=clickList[15].w + 1;
    r.h=clickList[15].h + 1;
    box.DrawBox(r, 80, false, *colorCounter);
	}
	switch(TakeOffItem){
		case 1: i=Players[ActivePlayer].Gear.Weapon; break;
		case 2: i=Players[ActivePlayer].Gear.Shield; break;
		case 3: i=Players[ActivePlayer].Gear.Armor; break;
		case 4: i=Players[ActivePlayer].Gear.Helmet; break;
		case 5: i=Players[ActivePlayer].Gear.Boots; break;
		case 6: i=Players[ActivePlayer].Gear.Hands; break;
		case 7: i=Players[ActivePlayer].Gear.Legs; break;
		case 8: i=Players[ActivePlayer].Gear.Ring; break;
    default: i=-1; break;
	}
  r.y=diObj->MouseY() - 16;
  r.x=diObj->MouseX() - 16;
  r.h=32;
  r.w=32;
  if(i>-1) SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(i).index]->texture, &ddGfx->vObj->at(i).r, &r);
  SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	return true;
}

void cDA2Party::ItemWinRender(int x, int y, int item){
	SDL_Rect r;
	char str[64];
	char str2[64];
  Uint8 R, G, B, A;

	r.y=y;
	r.x=x;
	r.h=140;
	r.w=245;
  SDL_GetRenderDrawColor(display->renderer, &R, &G, &B, &A);
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(display->renderer, &r);
  SDL_SetRenderDrawColor(display->renderer, R, G, B, A);
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[1], &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Windows,&ddGfx->aWindows[1],DDBLT_WAIT,NULL);

	text.drawText(display->renderer,x+15,y+10,0,ItemList->Items[item].ItemName);

	switch(ItemList->Items[item].Item){
  case 1:	text.drawText(display->renderer, x + 15, y + 25, 0, "Weapon"); break;
  case 2:	text.drawText(display->renderer, x + 15, y + 25, 0, "Shield"); break;
  case 3:	text.drawText(display->renderer, x + 15, y + 25, 0, "Torso"); break;
  case 4:	text.drawText(display->renderer, x + 15, y + 25, 0, "Helmet"); break;
  case 5:	text.drawText(display->renderer, x + 15, y + 25, 0, "Boots"); break;
  case 6:	text.drawText(display->renderer, x + 15, y + 25, 0, "Forearms"); break;
  case 7:	text.drawText(display->renderer, x + 15, y + 25, 0, "Legs"); break;
  case 8:	text.drawText(display->renderer, x + 15, y + 25, 0, "Ring"); break;
	case 9:
  default: text.drawText(display->renderer, x + 15, y + 25, 0, "Potion"); break;
	}

	strcpy(str,"Damage:  ");
	sprintf(str2,"%d",ItemList->Items[item].WeaponStr);
	strcat(str,str2);
  text.drawText(display->renderer, x + 15, y + 40, 0, str);

	strcpy(str,"Balance: ");
	sprintf(str2,"%d",ItemList->Items[item].WeaponBal);
	strcat(str,str2);
  text.drawText(display->renderer, x + 15, y + 55, 0, str);

	strcpy(str,"Defense: ");
	sprintf(str2,"%d",ItemList->Items[item].ArmorStr);
	strcat(str,str2);
	text.drawText(display->renderer,x+15,y+70,0,str);

	strcpy(str,"Encumb:  ");
	sprintf(str2,"%d",ItemList->Items[item].ArmorEncumb);
	strcat(str,str2);
	text.drawText(display->renderer,x+15,y+85,0,str);

	strcpy(str,"HP Mod:  ");
	sprintf(str2,"%d",ItemList->Items[item].HPMod);
	strcat(str,str2);
	text.drawText(display->renderer,x+15,y+100,0,str);

	strcpy(str,"Str. Mod:  ");
	sprintf(str2,"%d",ItemList->Items[item].StrMod);
	strcat(str,str2);
	text.drawText(display->renderer,x+110,y+40,0,str);

	strcpy(str,"Agl. Mod:  ");
	sprintf(str2,"%d",ItemList->Items[item].AglMod);
	strcat(str,str2);
	text.drawText(display->renderer,x+110,y+55,0,str);

	strcpy(str,"Int. Mod:  ");
	sprintf(str2,"%d",ItemList->Items[item].IntMod);
	strcat(str,str2);
	text.drawText(display->renderer,x+110,y+70,0,str);

	strcpy(str,"Speed Mod: ");
	sprintf(str2,"%d",ItemList->Items[item].SpeedMod);
	strcat(str,str2);
	text.drawText(display->renderer,x+110,y+85,0,str);

	strcpy(str,"MP Mod:    ");
	sprintf(str2,"%d",ItemList->Items[item].MPMod);
	strcat(str,str2);
	text.drawText(display->renderer,x+110,y+100,0,str);
 
	strcpy(str,"Users: ");
	switch(ItemList->Items[item].user){
	case 0: strcat(str,"All"); break;
	case 1:	strcat(str,"Warrior"); break;
	case 2:	strcat(str,"Thief"); break;
	case 3:	strcat(str,"Mage"); break;
	case 4:	strcat(str,"Warrior, Thief"); break;
	case 5:	strcat(str,"Warrior, Mage"); break;
	case 6:	strcat(str,"Thief, Mage"); break;
	default: break;
	}
	text.drawText(display->renderer,x+15,y+115,0,str);

	/*
        Select Case Query
        Case 1
            MSurfBack.BltFast x, Y + 140, Windows, win(4).r, DDBLTFAST_WAIT
        Case 2
            MSurfBack.BltFast x, Y + 140, Windows, win(3).r, DDBLTFAST_WAIT
        Case 3
            If Items(Num).Item = 9 Then
                MSurfBack.BltFast x, Y + 140, Windows, win(4).r, DDBLTFAST_WAIT
            Else
                MSurfBack.BltFast x, Y + 140, Windows, win(2).r, DDBLTFAST_WAIT
            End If
        End Select

        Flip

  */
}

int cDA2Party::Logic(){
	int i;
	bool bClick;

	diObj->pollEvents();
	
	if(bShowSpellList) {
		SpellListLogic();
		return true;
	}

	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
		diObj->LockKey(KEY_ESCAPE);
		return 0;
	}

	

	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		diObj->LockMouse(0);
		bClick=false;
		for(i=0;i<17;i++){
			if(diObj->CheckMousePos(clickList[i])){
				bClick=true;
				break;
			}
		}

		if(bClick){
			switch(i){
			case 0:
				return false;
				break;
			case 1:
				ActivePlayer--;
				if(ActivePlayer<0) ActivePlayer=2;
				break;
			case 2:
				ActivePlayer++;
				if(ActivePlayer>2) ActivePlayer=0;
				break;
			case 3:
				if(packOffset>5) packOffset-=6;
				break;
			case 4:
				if(packOffset<18) packOffset+=6;
				break;
			case 5:
				i=(diObj->MouseY()-231)/40 * 6;
				i+=(diObj->MouseX()-360)/40;
				i+=packOffset;
				if(i<40 && PartyItems[i]>0)	{
					Selection=i;
					switch(ActivePlayer){
					case 0:
						if(ItemList->Items[PartyItems[i]].user==0 || ItemList->Items[PartyItems[i]].user==3 || 
							 ItemList->Items[PartyItems[i]].user==5 || ItemList->Items[PartyItems[i]].user==6){
							Highlight=ItemList->Items[PartyItems[i]].Item+5;
						}
						break;
					case 1:
						if(ItemList->Items[PartyItems[i]].user==0 || ItemList->Items[PartyItems[i]].user==2 || 
							 ItemList->Items[PartyItems[i]].user==4 || ItemList->Items[PartyItems[i]].user==6){
							Highlight=ItemList->Items[PartyItems[i]].Item+5;
						}
						break;
					case 2:
						if(ItemList->Items[PartyItems[i]].user==0 || ItemList->Items[PartyItems[i]].user==1 || 
							 ItemList->Items[PartyItems[i]].user==4 || ItemList->Items[PartyItems[i]].user==5){
							Highlight=ItemList->Items[PartyItems[i]].Item+5;
						}
						break;
					default:
						break;
					}
				}
				break;
			case 6:
				TakeOffItem=1;
				Highlight=5;
				break;
			case 7:
				TakeOffItem=2;
				Highlight=5;
				break;
			case 8:
				TakeOffItem=3;
				Highlight=5;
				break;
			case 9:
				TakeOffItem=4;
				Highlight=5;
				break;
			case 10:
				TakeOffItem=5;
				Highlight=5;
				break;
			case 11:
				TakeOffItem=6;
				Highlight=5;
				break;
			case 12:
				TakeOffItem=7;
				Highlight=5;
				break;
			case 13:
				TakeOffItem=8;
				Highlight=5;
				break;
			case 16:
				if(ActivePlayer>0)break;
				bShowSpellList=true;
				Selection=-1;
				listOffset=0;
				break;
			default:
				break;
			}
		}
	} 

	if(!diObj->MousePress(0) && (Selection>-1 || TakeOffItem>-1)) {
		if(Highlight>-1){
			if(diObj->CheckMousePos(clickList[Highlight])){
				switch(Highlight){
				case 5:
					switch(TakeOffItem){
					case 1: if(AddItem(Players[ActivePlayer].Gear.Weapon)) Players[ActivePlayer].Gear.Weapon=0; break;
					case 2: if(AddItem(Players[ActivePlayer].Gear.Shield)) Players[ActivePlayer].Gear.Shield=0; break;
					case 3: if(AddItem(Players[ActivePlayer].Gear.Armor)) Players[ActivePlayer].Gear.Armor=0; break;
					case 4: if(AddItem(Players[ActivePlayer].Gear.Helmet)) Players[ActivePlayer].Gear.Helmet=0; break;
					case 5: if(AddItem(Players[ActivePlayer].Gear.Boots)) Players[ActivePlayer].Gear.Boots=0; break;
					case 6: if(AddItem(Players[ActivePlayer].Gear.Hands)) Players[ActivePlayer].Gear.Hands=0; break;
					case 7: if(AddItem(Players[ActivePlayer].Gear.Legs)) Players[ActivePlayer].Gear.Legs=0; break;
					case 8: if(AddItem(Players[ActivePlayer].Gear.Ring)) Players[ActivePlayer].Gear.Ring=0; break;
					default: break;
					}
					CalcAttributes();
					break;
				case 6:
					i=Players[ActivePlayer].Gear.Weapon;
					Players[ActivePlayer].Gear.Weapon=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 7:
					i=Players[ActivePlayer].Gear.Shield;
					Players[ActivePlayer].Gear.Shield=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 8:
					i=Players[ActivePlayer].Gear.Armor;
					Players[ActivePlayer].Gear.Armor=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 9:
					i=Players[ActivePlayer].Gear.Helmet;
					Players[ActivePlayer].Gear.Helmet=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 10:
					i=Players[ActivePlayer].Gear.Boots;
					Players[ActivePlayer].Gear.Boots=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 11:
					i=Players[ActivePlayer].Gear.Hands;
					Players[ActivePlayer].Gear.Hands=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 12:
					i=Players[ActivePlayer].Gear.Legs;
					Players[ActivePlayer].Gear.Legs=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 13:
					i=Players[ActivePlayer].Gear.Ring;
					Players[ActivePlayer].Gear.Ring=PartyItems[Selection];
					PartyItems[Selection]=i;
					CalcAttributes();
					break;
				case 14:
					switch(PartyItems[Selection]){
					case 60: //Magic Potion
						if(ActivePlayer>0) break;
						if(Players[ActivePlayer].MP==Players[ActivePlayer].MaxMP) break;
						i=(int)(Players[ActivePlayer].MaxMP/4);
						if(i<50) i=50;
						Players[ActivePlayer].MP+=i;
						if(Players[ActivePlayer].MP>Players[ActivePlayer].MaxMP) Players[ActivePlayer].MP=Players[ActivePlayer].MaxMP;
						PartyItems[Selection]=0;
						break;

					case 61: //Life Potion
						if(Players[ActivePlayer].HP<1){
							Players[ActivePlayer].HP=1;
							PartyItems[Selection]=0;
						}
						break;

					case 62: //Health Potion
						if(Players[ActivePlayer].HP==Players[ActivePlayer].MaxHP) break;
						if(Players[ActivePlayer].HP>0){
							Players[ActivePlayer].HP+=(int)(Players[ActivePlayer].MaxHP/2);
							if(Players[ActivePlayer].HP>Players[ActivePlayer].MaxHP) Players[ActivePlayer].HP=Players[ActivePlayer].MaxHP;
							PartyItems[Selection]=0;
						}
						break;

					case 63: //Strength
						if(Players[ActivePlayer].HP>0){
							Players[ActivePlayer].BaseStr+=1;
							Players[ActivePlayer].ModStr+=1;
							Players[ActivePlayer].MaxHP+=2;
							Players[ActivePlayer].HP+=2;
							PartyItems[Selection]=0;
						}
						break;

					case 64: //Agility
						if(Players[ActivePlayer].HP>0){
							Players[ActivePlayer].BaseAgl+=1;
							Players[ActivePlayer].ModAgl+=1;
							PartyItems[Selection]=0;
						}
						break;

					case 65: //Intelligence
						if(Players[ActivePlayer].HP>0){
							Players[ActivePlayer].BaseInt+=1;
							Players[ActivePlayer].ModInt+=1;
							PartyItems[Selection]=0;
						}
						break;

					case 66: //Speed
						if(Players[ActivePlayer].HP>0){
							Players[ActivePlayer].BaseSpeed+=1;
							Players[ActivePlayer].ModSpeed+=1;
							PartyItems[Selection]=0;
						}
						break;

					default:
						break;
					}
					break;
				default:
					break;
				}

			}

		}

		//if dropping item
		if(diObj->CheckMousePos(clickList[15])){
			DropItem=PartyItems[Selection];
			Selection=-1;
			Highlight=-1;
			TakeOffItem=-1;
			return 2;
		}

		Selection=-1;
		Highlight=-1;
		TakeOffItem=-1;
	}
 
	return 1;

}

void cDA2Party::SpellListLogic(){
	int i,j;
	bool bClick;

	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
		diObj->LockKey(KEY_ESCAPE);
		bShowSpellList=false;
		Selection=-1;
		return;
	}

	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		diObj->LockMouse(0);
		bClick=false;
		for(i=0;i<20;i++){
			if(diObj->CheckMousePos(clickSpell[i])){
				bClick=true;
				break;
			}
		}

		if(bClick){
			switch(i){
			case 15: //move spell up
				if(Selection+listOffset>0 && Selection>-1){
					j=SpellList[Selection+listOffset-1];
					SpellList[Selection+listOffset-1]=SpellList[Selection+listOffset];
					SpellList[Selection+listOffset]=j;
					if(Selection>0) Selection--;
					else listOffset--;
				}
				break;
			case 16: //move spell down
				if(Selection+listOffset<SpellCount-1 && Selection>-1){
					j=SpellList[Selection+listOffset+1];
					SpellList[Selection+listOffset+1]=SpellList[Selection+listOffset];
					SpellList[Selection+listOffset]=j;
					if(Selection<14) Selection++;
					else listOffset++;
				}
				break;
			case 17:
				bShowSpellList=false;
				Selection=-1;
				return;
				break;
			case 18:
				if(listOffset>0) listOffset--;
				break;
			case 19:
				if(listOffset+15<SpellCount) listOffset++;
				break;
			default: //cases 0 through 14
				if(listOffset+i<SpellCount) Selection=i;
				break;
			}
		}

	}

}
