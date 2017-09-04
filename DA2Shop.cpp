#include "DA2Shop.h"

cDA2Shop::cDA2Shop(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	Party=NULL;
	ItemList=NULL;
	Selection=0;
	ShopType=0;
	BuyOffset=0;
	SellOffset=0;
	BuyCount=0;
	SellCount=0;
	PartyItemCount=0;
	BuyOrSell=true;

	clickTime=SDL_GetTicks();

	int i;
	for(i=0;i<10;i++) Buy[i]=0;

	//Buy scroll up
	clickList[0].x=287; clickList[0].y=180; clickList[0].w=16; clickList[0].h=16;
	//Buy scroll down
  clickList[1].x=287; clickList[1].y=281; clickList[1].w=16; clickList[1].h=16;
	//Sell scroll up
  clickList[2].x=595; clickList[2].y=180; clickList[2].w=16; clickList[2].h=16;
	//Sell scroll down
  clickList[3].x=595; clickList[3].y=281; clickList[3].w=16; clickList[3].h=16;
  //Buy selection 1
	clickList[4].x=36; clickList[4].y=181; clickList[4].w=243; clickList[4].h=38;
	//Buy selection 2
	clickList[5].x=36; clickList[5].y=220; clickList[5].w=243; clickList[5].h=39;
	//Buy selection 3
	clickList[6].x=36; clickList[6].y=260; clickList[6].w=243; clickList[6].h=39;
  //Sell selection 1
	clickList[7].x=344; clickList[7].y=181; clickList[7].w=243; clickList[7].h=38;
	//Sell selection 2
	clickList[8].x=344; clickList[8].y=220; clickList[8].w=243; clickList[8].h=39;
	//Sell selection 3
	clickList[9].x=344; clickList[9].y=260; clickList[9].w=243; clickList[9].h=39;
	//Buy button
	clickList[10].x=412; clickList[10].y=338; clickList[10].w=61; clickList[10].h=24;
	//Sell button
  clickList[11].x=412; clickList[11].y=367; clickList[11].w=61; clickList[11].h=24;
	//Done button
  clickList[12].x=412; clickList[12].y=425; clickList[12].w=61; clickList[12].h=24;
	//Party button - compares to party
  clickList[13].x=412; clickList[13].y=396; clickList[13].w=61; clickList[13].h=24;
	//Stats window button
  clickList[14].x=373; clickList[14].y=334; clickList[14].w=61; clickList[14].h=24;
}

cDA2Shop::~cDA2Shop(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	Party=NULL;
	ItemList=NULL;
}

bool cDA2Shop::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cDA2Party *par, cItemController *itm){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((Party=par)==NULL) return false;
	if((ItemList=itm)==NULL) return false;
	if(!text.Init(ddGfx)) return false;
	if(!box.Init(display,ddGfx)) return false;
	if(!statsWin.Init(display,ddGfx)) return false;
	Selection=-1;
	statsWin.SetSize(190,106,260,268);
	return true;
}

void cDA2Shop::Clear(){
	int i;
	for(i=0;i<10;i++) Buy[i]=0;
	Sell.clear();
	Selection=-1;
	ShopType=0;
	BuyOffset=0;
	SellOffset=0;
	BuyCount=0;
	SellCount=0;
}

void cDA2Shop::SetType(int type){
	int i;

	Sell.clear();
	ShopType=type;
	PartyItemCount=0;
	switch(type){
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		for(i=0;i<40;i++){
			if(Party->PartyItems[i]==0) continue;
			if(ItemList->Items[Party->PartyItems[i]].Item==type) Sell.push_back(i);
			PartyItemCount++;
		}
		break;
	case 10: //all armor
		for(i=0;i<40;i++){
			if(Party->PartyItems[i]==0) continue;
			if(ItemList->Items[Party->PartyItems[i]].Item>1 && ItemList->Items[Party->PartyItems[i]].Item<9) Sell.push_back(i);
			PartyItemCount++;
		}
		break;
	case 11: //weapons and armor
		for(i=0;i<40;i++){
			if(Party->PartyItems[i]==0) continue;
			if(ItemList->Items[Party->PartyItems[i]].Item>0 && ItemList->Items[Party->PartyItems[i]].Item<9) Sell.push_back(i);
			PartyItemCount++;
		}
		break;
	case 12:
		for(i=0;i<40;i++){
			if(Party->PartyItems[i]==0) continue;
			if(ItemList->Items[Party->PartyItems[i]].Item>0) Sell.push_back(i);
			PartyItemCount++;
		}
		break;
	default:
		for(i=0;i<40;i++){
			if(Party->PartyItems[i]==0) continue;
			PartyItemCount++;
		}
		break;
	}
}

void cDA2Shop::SetItem(int item){
	int i;
	for(i=0;i<10;i++){
		if(Buy[i]==0) {
			Buy[i]=item;
			BuyCount++;
			break;
		}
	}
}

bool cDA2Shop::Render(){
	SDL_Rect r;
	int i;
	int x,y;
	char str[24],str2[24];

	//display the stat screen
  SDL_RenderClear(display->renderer);
  SDL_RenderCopy(display->renderer, ddGfx->Shop->texture, NULL, NULL);

	//display party cash
	strcpy(str,"(Gold: ");
	sprintf(str2,"%d)",Party->Gold);
	strcat(str,str2);
	text.drawText(display->renderer,125,158,0,str);

	//Display party item count
	sprintf(str,"(%d/40 items in pack)\0",PartyItemCount);
	if(PartyItemCount==40) strcat(str," Full!");
	text.drawText(display->renderer,390,158,0,str);

	//Render items from shop
	x=35;
	y=180;
  r.w=32;
	r.h=32;
	for(i=BuyOffset;i<BuyOffset+3 && i<BuyCount;i++){
		if(BuyOrSell && Selection==i) box.DrawBox(33,y-2,280,y+36,165,true);
		
		r.x=x;
		r.y=y;
    SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(Buy[i]).index]->texture, &ddGfx->vObj->at(Buy[i]).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(Buy[i]).index], &ddGfx->vObj->at(Buy[i]).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	
		text.drawText(display->renderer,x+35,y,0,ItemList->Items[Buy[i]].ItemName);
		strcpy(str,"Price: ");
		sprintf(str2,"%d",ItemList->Items[Buy[i]].value);
		strcat(str,str2);
		text.drawText(display->renderer,x+35,y+16,0,str);
		
		y+=40;
	}

	//Render player items to sell
	x=343;
	y=180;
  r.w=32;
  r.h=32;
	for(i=SellOffset;i<SellOffset+3 && i<Sell.size();i++){
		if(!BuyOrSell && Selection==i) box.DrawBox(343,y-2,588,y+36,165,true);
		
		r.x=x;
		r.y=y;
    SDL_RenderCopy(display->renderer, ddGfx->Objects[ddGfx->vObj->at(Party->PartyItems[Sell[i]]).index]->texture, &ddGfx->vObj->at(Party->PartyItems[Sell[i]]).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Objects[ddGfx->vObj->at(Party->PartyItems[Sell[i]]).index], &ddGfx->vObj->at(Party->PartyItems[Sell[i]]).r, DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	
		text.drawText(display->renderer,x+35,y,0,ItemList->Items[Party->PartyItems[Sell[i]]].ItemName);
		strcpy(str,"Price: ");
		sprintf(str2,"%d",ItemList->Items[Party->PartyItems[Sell[i]]].value/4);
		strcat(str,str2);
		text.drawText(display->renderer,x+35,y+16,0,str);
	
		y+=40;
	}

	//Draw scroll buttons if needed
  if(BuyOffset + 3<BuyCount) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &clickList[1]);
  if(BuyOffset>0)	SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &clickList[0]);
  if(SellOffset + 3<Sell.size()) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[12], &clickList[3]);
  if(SellOffset>0) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[11], &clickList[2]);
	//if(BuyOffset+3<BuyCount) ddObj->ddsb->Blt(&clickList[1], ddGfx->Windows,&ddGfx->aWindows[12],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	//if(BuyOffset>0)	ddObj->ddsb->Blt(&clickList[0], ddGfx->Windows,&ddGfx->aWindows[11],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	//if(SellOffset+3<Sell.size()) ddObj->ddsb->Blt(&clickList[3], ddGfx->Windows,&ddGfx->aWindows[12],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	//if(SellOffset>0) ddObj->ddsb->Blt(&clickList[2], ddGfx->Windows,&ddGfx->aWindows[11],DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	//highlight item under mouse
	if(!bStatsWin){
		if(diObj->MouseX()>35 && diObj->MouseX()<280 && diObj->MouseY()>180 && diObj->MouseY()<300){
			y = (diObj->MouseY()-180)/40;
			box.DrawBox(33,178+y*40,280,178+y*40+38,15);
		}
		if(diObj->MouseX()>343 && diObj->MouseX()<588 && diObj->MouseY()>180 && diObj->MouseY()<300){
			y = (diObj->MouseY()-180)/40;
			box.DrawBox(343,178+y*40,588,178+y*40+38,15);
		}
	}

	//Show highlighted item stats
	if(Selection>-1){
		if(BuyOrSell) i=Buy[Selection];
		else i=Party->PartyItems[Sell[Selection]];

		text.drawText(display->renderer,168,334,0,ItemList->Items[i].ItemName);

		switch(ItemList->Items[i].Item){
    case 1:	text.drawText(display->renderer, 168, 349, 0, "Weapon"); break;
    case 2:	text.drawText(display->renderer, 168, 349, 0, "Shield"); break;
    case 3:	text.drawText(display->renderer, 168, 349, 0, "Torso"); break;
    case 4:	text.drawText(display->renderer, 168, 349, 0, "Helmet"); break;
    case 5:	text.drawText(display->renderer, 168, 349, 0, "Boots"); break;
    case 6:	text.drawText(display->renderer, 168, 349, 0, "Forearms"); break;
    case 7:	text.drawText(display->renderer, 168, 349, 0, "Legs"); break;
    case 8:	text.drawText(display->renderer, 168, 349, 0, "Ring"); break;
		case 9:
    default: text.drawText(display->renderer, 168, 349, 0, "Potion"); break;
		}

		sprintf(str2,"%d",ItemList->Items[i].WeaponStr);
    text.drawText(display->renderer, 168, 364, 0, "Damage:\0");
    text.drawText(display->renderer, 232, 364, 0, str2);

		sprintf(str2,"%d",ItemList->Items[i].WeaponBal);
		text.drawText(display->renderer,168,379,0,"Balance:\0");
		text.drawText(display->renderer,232,379,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].ArmorStr);
		text.drawText(display->renderer,168,394,0,"Defense:\0");
		text.drawText(display->renderer,232,394,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].ArmorEncumb);
		text.drawText(display->renderer,168,409,0,"Encumb:\0");
		text.drawText(display->renderer,232,409,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].HPMod);
		text.drawText(display->renderer,168,424,0,"HP Mod:\0");
		text.drawText(display->renderer,232,424,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].StrMod);
		text.drawText(display->renderer,263,364,0,"Str. Mod:\0");
		text.drawText(display->renderer,343,364,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].AglMod);
		text.drawText(display->renderer,263,379,0,"Agl. Mod:\0");
		text.drawText(display->renderer,343,379,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].IntMod);
		text.drawText(display->renderer,263,394,0,"Int. Mod:\0");
		text.drawText(display->renderer,343,394,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].SpeedMod);
		text.drawText(display->renderer,263,409,0,"Speed Mod:\0");
		text.drawText(display->renderer,343,409,0,str2);

		sprintf(str2,"%d",ItemList->Items[i].MPMod);
		text.drawText(display->renderer,263,424,0,"MP Mod:\0");
		text.drawText(display->renderer,343,424,0,str2);
 
		strcpy(str,"Users: ");
		switch(ItemList->Items[i].user){
		case 0: strcat(str,"All"); break;
		case 1:	strcat(str,"Warrior"); break;
		case 2:	strcat(str,"Thief"); break;
		case 3:	strcat(str,"Mage"); break;
		case 4:	strcat(str,"Warrior, Thief"); break;
		case 5:	strcat(str,"Warrior, Mage"); break;
		case 6:	strcat(str,"Thief, Mage"); break;
		default: break;
		}
		text.drawText(display->renderer,168,439,0,str);
	}

	//draw buttons
  if(BuyOrSell && Selection>-1 && PartyItemCount<40 && Party->Gold >= ItemList->Items[Buy[Selection]].value) 	SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[13], &clickList[10]); 
  //ddObj->ddsb->Blt(&clickList[10], ddGfx->Windows, &ddGfx->aWindows[13], DDBLT_WAIT | DDBLT_KEYSRC, NULL);
  if(BuyOrSell && Selection>-1 && ItemList->Items[Buy[Selection]].Item<9) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[32], &clickList[13]); 
  //ddObj->ddsb->Blt(&clickList[13], ddGfx->Windows, &ddGfx->aWindows[32], DDBLT_WAIT | DDBLT_KEYSRC, NULL); //compare
	
  if(!BuyOrSell && Selection>-1) SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[14], &clickList[11]); 
  //ddObj->ddsb->Blt(&clickList[11], ddGfx->Windows, &ddGfx->aWindows[14], DDBLT_WAIT | DDBLT_KEYSRC, NULL);
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[15], &clickList[12]);
	//ddObj->ddsb->Blt(&clickList[12], ddGfx->Windows,&ddGfx->aWindows[15],DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	if(bStatsWin)	RenderStats(Buy[Selection]);

	//display the mouse cursor
	r.y=diObj->MouseY()-16;
	r.x=diObj->MouseX()-16;
	r.h=32;
	r.w=32;
  SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	return true;
}

void cDA2Shop::RenderStats(int item){
	char str[128];
	int i,j;
	int compItem[3];
	SDL_Rect r;

	for(i=0;i<3;i++){
		switch(ItemList->Items[item].Item){
		case 1: compItem[i]=Party->Players[i].Gear.Weapon; break;
		case 2: compItem[i]=Party->Players[i].Gear.Shield; break;
		case 3: compItem[i]=Party->Players[i].Gear.Armor; break;
		case 4: compItem[i]=Party->Players[i].Gear.Helmet; break;
		case 5: compItem[i]=Party->Players[i].Gear.Boots; break;
		case 6: compItem[i]=Party->Players[i].Gear.Hands; break;
		case 7: compItem[i]=Party->Players[i].Gear.Legs; break;
		case 8: compItem[i]=Party->Players[i].Gear.Ring; break;
		default: break;
		}
	}

	statsWin.DrawWindow();
  SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[15], &clickList[14]);
	//ddObj->ddsb->Blt(&clickList[14], ddGfx->Windows,&ddGfx->aWindows[15],DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	//sprintf(str,"Compare %s",ItemList->Items[item].ItemName);
	text.drawText(display->renderer,206,122,0,ItemList->Items[item].ItemName);

	/*
	switch(ItemList->Items[item].Item){
	case 1: text.drawText(display->renderer,40,122,0,"Compare Weapons:"); break;
	case 2: text.drawText(display->renderer,40,122,0,"Compare Shields:"); break;
	case 3: text.drawText(display->renderer,40,122,0,"Compare Torsos:"); break;
	case 4: text.drawText(display->renderer,40,122,0,"Compare Helmets:"); break;
	case 5: text.drawText(display->renderer,40,122,0,"Compare Boots:"); break;
	case 6: text.drawText(display->renderer,40,122,0,"Compare Forearms:"); break;
	case 7: text.drawText(display->renderer,40,122,0,"Compare Legs:"); break;
	case 8: text.drawText(display->renderer,40,122,0,"Compare Rings:"); break;
	default: break;
	}
	*/

	sprintf(str,"Dam: %d\nBal: %d\nDef: %d\nEnc: %d\nStr: %d\nAgl: %d\nInt: %d\nSpd: %d\nHP:  %d\nMP:  %d",ItemList->Items[item].WeaponStr,ItemList->Items[item].WeaponBal,ItemList->Items[item].ArmorStr,
				ItemList->Items[item].ArmorEncumb,ItemList->Items[item].StrMod,ItemList->Items[item].AglMod,ItemList->Items[item].IntMod,ItemList->Items[item].SpeedMod,
				ItemList->Items[item].HPMod,ItemList->Items[item].MPMod);
  text.drawText(display->renderer,206,170,0,str);
	//text.drawText(display->renderer,40,170,0,"Dam:\nBal:\nDef:\nEnc:\nStr:\nAgl:\nInt:\nSpd:\nHP:\nMP:");

	for(i=0;i<3;i++){
		r.x=302+i*48; r.w=32; r.y=138; r.h=32;
    if(Party->Players[i].HP>0) SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(i * 16).r, &r);
    //ddObj->ddsb->Blt(&r, ddGfx->Chars[0], &ddGfx->vNPC->at(i * 16).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
    else SDL_RenderCopy(display->renderer, ddGfx->Tiles[ddGfx->vTile->at(1560 + i).index]->texture, &ddGfx->vTile->at(1560 + i).r, &r);
    //ddObj->ddsb->Blt(&r, ddGfx->Tiles[ddGfx->vTile->at(1560 + i).index], &ddGfx->vTile->at(1560 + i).r, DDBLT_WAIT | DDBLT_KEYSRC, NULL);

		//text.drawText(display->renderer,96+i*176,138,0,Party->Players[i].PlayerName);
		//text.drawText(display->renderer,96+i*176,154,0,ItemList->Items[compItem[i]].ItemName);
		//if(compItem[i]>0){
		//	sprintf(str,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d",ItemList->Items[compItem[i]].WeaponStr,ItemList->Items[compItem[i]].WeaponBal,ItemList->Items[compItem[i]].ArmorStr,
		//		ItemList->Items[compItem[i]].ArmorEncumb,ItemList->Items[compItem[i]].StrMod,ItemList->Items[compItem[i]].AglMod,ItemList->Items[compItem[i]].IntMod,ItemList->Items[compItem[i]].SpeedMod,
		//		ItemList->Items[compItem[i]].HPMod,ItemList->Items[compItem[i]].MPMod);
		//} else {
		if(i==0 && (ItemList->Items[item].user==1 || ItemList->Items[item].user==2 || ItemList->Items[item].user==4)) {
			sprintf(str," -\n -\n -\n -\n -\n -\n -\n -\n -\n -");
			text.drawText(display->renderer,302+i*48,170,0,str,1);
		} else if (i==1 && (ItemList->Items[item].user==1 || ItemList->Items[item].user==3 || ItemList->Items[item].user==5)) {
			sprintf(str," -\n -\n -\n -\n -\n -\n -\n -\n -\n -");
			text.drawText(display->renderer,302+i*48,170,0,str,1);
		} else if (i==2 && (ItemList->Items[item].user==2 || ItemList->Items[item].user==3 || ItemList->Items[item].user==6)) {
			sprintf(str," -\n -\n -\n -\n -\n -\n -\n -\n -\n -");
			text.drawText(display->renderer,302+i*48,170,0,str,1);
		} else {

			j=ItemList->Items[item].WeaponStr - ItemList->Items[compItem[i]].WeaponStr;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,170,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,170,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,170,0," 0");
			}

			j=ItemList->Items[item].WeaponBal - ItemList->Items[compItem[i]].WeaponBal;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,186,0,str,3);
			}	else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,186,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,186,0," 0");
			}


			j=ItemList->Items[item].ArmorStr - ItemList->Items[compItem[i]].ArmorStr;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,202,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,202,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,202,0," 0");
			}

			j=ItemList->Items[item].ArmorEncumb - ItemList->Items[compItem[i]].ArmorEncumb;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,218,0,str,4);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,218,0,str,3);
			} else {
				text.drawText(display->renderer,302+i*48,218,0," 0");
			}

			j=ItemList->Items[item].StrMod - ItemList->Items[compItem[i]].StrMod;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,234,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,234,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,234,0," 0");
			}

			j=ItemList->Items[item].AglMod - ItemList->Items[compItem[i]].AglMod;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,250,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,250,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,250,0," 0");
			}

			j=ItemList->Items[item].IntMod - ItemList->Items[compItem[i]].IntMod;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,266,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,266,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,266,0," 0");
			}

			j=ItemList->Items[item].SpeedMod - ItemList->Items[compItem[i]].SpeedMod;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,282,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,282,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,282,0," 0");
			}

			j=ItemList->Items[item].HPMod - ItemList->Items[compItem[i]].HPMod;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,298,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,298,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,298,0," 0");
			}

			j=ItemList->Items[item].MPMod - ItemList->Items[compItem[i]].MPMod;
			if(j<0) {
				sprintf(str,"%d",j);
				text.drawText(display->renderer,302+i*48,314,0,str,3);
			} else if(j>0) {
				sprintf(str,"+%d",j);
				text.drawText(display->renderer,302+i*48,314,0,str,4);
			} else {
				text.drawText(display->renderer,302+i*48,314,0," 0");
			}

		}

	}
}


bool cDA2Shop::Logic(){
	int i;
	bool bClick;
	Uint32 ElapsedTime=SDL_GetTicks();

	diObj->pollEvents();

	if(diObj->KeyPress(KEY_ESCAPE) && !diObj->CheckLock(KEY_ESCAPE)){
		diObj->LockKey(KEY_ESCAPE);
		return false;
	}

	if(diObj->MousePress(0)){

		//Handle the stats window if it is open
		if(bStatsWin && !diObj->CheckLockMouse(0)){
      if(diObj->CheckMousePos(clickList[14])){
				diObj->LockMouse(0);
				bStatsWin=false;
			}
			return true;
		}

		bClick=false;
		for(i=0;i<14;i++){
      if(diObj->CheckMousePos(clickList[i])){
				bClick=true;
				break;
			}
		}

		if(bClick){
			switch(i){
			case 0:
				if(!diObj->CheckLockMouse(0)) clickTime-=250;
				if(ElapsedTime-clickTime > 250 || ElapsedTime-clickTime<0){
          clickTime=SDL_GetTicks();
					BuyOffset--;
					if(BuyOffset<0) BuyOffset++;
				}
				break;
			case 1:
				if(!diObj->CheckLockMouse(0)) clickTime-=250;
				if(ElapsedTime-clickTime > 250 || ElapsedTime-clickTime<0){
          clickTime=SDL_GetTicks();
					BuyOffset++;
					if(BuyOffset>BuyCount-3) BuyOffset--;
				}
				break;
			case 2:
				if(!diObj->CheckLockMouse(0)) clickTime-=250;
				if(ElapsedTime-clickTime > 250 || ElapsedTime-clickTime<0){
          clickTime=SDL_GetTicks();
					SellOffset--;
					if(SellOffset<0) SellOffset++;
				}
				break;
			case 3:
				if(!diObj->CheckLockMouse(0)) clickTime-=250;
				if(ElapsedTime-clickTime > 250 || ElapsedTime-clickTime<0){
          clickTime=SDL_GetTicks();
					SellOffset++;
					if(SellOffset>Sell.size()-3) SellOffset--;
				}
				break;
			case 4:
				if(diObj->CheckLockMouse(0)) break;
				if(BuyOffset<BuyCount){
					Selection=BuyOffset;
					BuyOrSell=true;
				}
				break;
			case 5:
				if(diObj->CheckLockMouse(0)) break;
				if(BuyOffset+1<BuyCount){
					Selection=BuyOffset+1;
					BuyOrSell=true;
				}
				break;
			case 6:
				if(diObj->CheckLockMouse(0)) break;
				if(BuyOffset+2<BuyCount){
					Selection=BuyOffset+2;
					BuyOrSell=true;
				}
				break;
			case 7:
				if(diObj->CheckLockMouse(0)) break;
				if(SellOffset<Sell.size()){
					Selection=SellOffset;
					BuyOrSell=false;
				}
				break;
			case 8:
				if(diObj->CheckLockMouse(0)) break;
				if(SellOffset+1<Sell.size()){
					Selection=SellOffset+1;
					BuyOrSell=false;
				}
				break;
			case 9:
				if(diObj->CheckLockMouse(0)) break;
				if(SellOffset+2<Sell.size()){
					Selection=SellOffset+2;
					BuyOrSell=false;
				}
				break;
			case 10:
				if(!BuyOrSell) break;
				if(Selection<0) break;
				if(diObj->CheckLockMouse(0)) break;
				if(Party->Gold<ItemList->Items[Buy[Selection]].value) break;
				if(PartyItemCount==40) break;
				Party->Gold -= ItemList->Items[Buy[Selection]].value;
				Party->AddItem(Buy[Selection]);
				SetType(ShopType);
				Selection=-1;
				break;
			case 11:
				if(BuyOrSell) break;
				if(Selection<0) break;
				if(diObj->CheckLockMouse(0)) break;
				Party->Gold += (ItemList->Items[Party->PartyItems[Sell[Selection]]].value/4);
				Party->PartyItems[Sell[Selection]]=0;
				Sell.erase(Sell.begin()+Selection);
				PartyItemCount--;
				Selection=-1;
				break;
			case 12:
				if(diObj->CheckLockMouse(0)) break;
				diObj->LockMouse(0);
				return false;
				break;
			case 13:
				if(diObj->CheckLockMouse(0)) break;
				diObj->LockMouse(0);
				bStatsWin=true;
				break;
			default:
				break;
			}
			diObj->LockMouse(0);
		}
	}


	return true;
}