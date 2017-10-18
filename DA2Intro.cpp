#include "DA2Intro.h"

cDA2Intro::cDA2Intro(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	party=NULL;
	animCounter=NULL;
	mode=0;
	currentPlayer=0;
}

cDA2Intro::~cDA2Intro(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
	party=NULL;
	animCounter=NULL;
}

bool cDA2Intro::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cDA2Party *p, int *anim){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if((party=p)==NULL) return false;
	if((animCounter=anim)==NULL) return false;
	if(!text.Init(gfx)) return false;
	mode=0;
	currentPlayer=0;
	Cursor=-1;

	//Cancel button
	button.x=355; button.w=61; button.y=220; button.h=24;
	return true;
}

void cDA2Intro::SetDefault(){
	mode=0;
	currentPlayer=0;
	Cursor=strlen(party->Players[currentPlayer].PlayerName);
}

bool cDA2Intro::Logic(){
	int i;
	bool bClick=false;
	string str,str2;
	char c;

	diObj->pollEvents();

	if(Cursor>-1){
		str=party->Players[currentPlayer].PlayerName;
		for(i=0;i<=100;i++){
      if(diObj->KeyPress((DA2KEYS)i) && !diObj->CheckLock((DA2KEYS)i)){
        diObj->LockKey((DA2KEYS)i);
        c=DIKtoASCII((DA2KEYS)i);
				if(c>=97 && c<=122)	{
					if(diObj->KeyPress(KEY_LSHIFT) || diObj->KeyPress(KEY_RSHIFT)) c-=32;
					if(strlen(party->Players[currentPlayer].PlayerName)==19) continue;
					if(Cursor==strlen(party->Players[currentPlayer].PlayerName)){
						str+=c;
						strcpy(party->Players[currentPlayer].PlayerName,&str[0]);
					} else {
						str2=str.substr(Cursor,40);
						str=str.substr(0,Cursor);
						str+=c;
						str+=str2;
						strcpy(party->Players[currentPlayer].PlayerName,&str[0]);
					}
					Cursor++;
				} else if(c>=32 && c<=57) { //numbers
					if(strlen(party->Players[currentPlayer].PlayerName)==19) continue;
					if(Cursor==strlen(party->Players[currentPlayer].PlayerName)){
						str+=c;
						strcpy(party->Players[currentPlayer].PlayerName,&str[0]);
					} else {
						str2=str.substr(Cursor,40);
						str=str.substr(0,Cursor);
						str+=c;
						str+=str2;
						strcpy(party->Players[currentPlayer].PlayerName,&str[0]);
					}
					Cursor++;
				} else if (c==8){ //backspace
					if(Cursor==strlen(party->Players[currentPlayer].PlayerName)){
						party->Players[currentPlayer].PlayerName[strlen(party->Players[currentPlayer].PlayerName)-1]='\0';
					  str=party->Players[currentPlayer].PlayerName;
					} else {
						str2=str.substr(Cursor,40);
						str=str.substr(0,Cursor-1);
						str+=str2;
						strcpy(party->Players[currentPlayer].PlayerName,&str[0]);
					}
					Cursor--;
					if(Cursor<0) Cursor=0;
				}
			}
		}
		if(diObj->KeyPress(KEY_DELETE) && !diObj->CheckLock(KEY_DELETE) && Cursor<strlen(party->Players[currentPlayer].PlayerName)){
			diObj->LockKey(KEY_DELETE);
			str2=str.substr(Cursor+1,40);
			str=str.substr(0,Cursor);
			str+=str2;
			strcpy(party->Players[currentPlayer].PlayerName,&str[0]);
		}
		if(diObj->KeyPress(KEY_LEFT) && !diObj->CheckLock(KEY_LEFT) && Cursor>0) {
			Cursor--;
			diObj->LockKey(KEY_LEFT);
		}
		if(diObj->KeyPress(KEY_RIGHT) && !diObj->CheckLock(KEY_RIGHT) && Cursor<strlen(party->Players[currentPlayer].PlayerName))	{
			Cursor++;
			diObj->LockKey(KEY_RIGHT);
		}
	}

	if(diObj->MousePress(0) && !diObj->CheckLockMouse(0)){
		if(mode==0){
      if(diObj->MouseX() >= button.x && diObj->MouseX() <= button.x+button.w &&
         diObj->MouseY() >= button.y && diObj->MouseY() <= button.y+button.h){
				bClick=true;
				diObj->LockMouse(0);
			}
		} else {
			bClick=true;
			diObj->LockMouse(0);
		}

		if(bClick){
			switch(mode){
			case 0:
				currentPlayer++;
				if(currentPlayer==3) mode++;
        else Cursor=strlen(party->Players[currentPlayer].PlayerName);
				break;
			case 1:
				return false;
			default:
				break;
			}
		}
	}

	return true;
}

bool cDA2Intro::Render(){
	
	int i;
	SDL_Rect r;
	char str[8];

	//Draw a blank screen
  SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
  SDL_RenderClear(display->renderer);

	//Show players when naming them
	if(mode==0){
    SDL_Rect rx=ddGfx->vNPC->at(*animCounter + currentPlayer * 16).r;
		//r.left=308; r.right=340; r.top=128; r.bottom=160;
		//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(*animCounter).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		r.x=322; r.w=32; r.y=148; r.h=32;
    SDL_RenderCopy(display->renderer, ddGfx->Chars[0]->texture, &ddGfx->vNPC->at(*animCounter + currentPlayer * 16).r, &r);
		//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(*animCounter+currentPlayer*16).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
		//r.left=436; r.right=468; r.top=128; r.bottom=160;
		//ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(*animCounter+32).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	}

	//Show text
	if(mode==0){
		switch(currentPlayer){
			case 0:
				text.drawText(display->renderer,56,80 ,0,"The mage uses magic to best his foes. He has limited physical");
				text.drawText(display->renderer,56,100,0,"strength and achieves his peak performance with high intelligence.");
				text.drawText(display->renderer,56,120,0,"He has limited knowledge of conventional weapons and armor.");
				text.drawText(display->renderer,56,200,0,"Name your mage: ");
				break;
			case 1:
				text.drawText(display->renderer,56,80 ,0,"The thief is capable of making multiple attacks agains foes.");
				text.drawText(display->renderer,56,100,0,"Her skill in battle is greatest with high agility.");
				text.drawText(display->renderer,56,120,0,"She has decent knowledge of conventional weapons and armor.");
				text.drawText(display->renderer,56,200,0,"Name your thief: ");
				break;
			case 2:
				text.drawText(display->renderer,56,80 ,0,"The warrior excells in hand-to-hand combat against foes. He");
				text.drawText(display->renderer,56,100,0,"relies on strength to absorb most attacks and counter effectively.");
				text.drawText(display->renderer,56,120,0,"He has superb knowledge of conventional weapons and armor.");
				text.drawText(display->renderer,56,200,0,"Name your warrior: ");
				break;
			default:
				break;
		}
	} else {
		text.drawText(display->renderer,48,45,0,"The story so far:");
    text.drawText(display->renderer,48,60,0,"    With a silent shrug the guard turns from the shadow, giving up.");
    text.drawText(display->renderer,48,72,0,"his search. Releasing a quiet sigh of relief, the young thief");
    text.drawText(display->renderer,48,84,0,"motions to her two companions. One is a mage of slight talent, the");
    text.drawText(display->renderer,48,96,0,"other a hulking oaf of an axe-swinger. The Thieves' Guild accepts");
    text.drawText(display->renderer,48,108,0,"apprentices of varied talents. After all, a little brains and brawn");
    text.drawText(display->renderer,48,120,0,"could be necessary in any assignment.");
    text.drawText(display->renderer,48,135,0,"    Night in Wisp is very still, a stark contrast to the nonstop");
    text.drawText(display->renderer,48,147,0,"bustle in town during the day. Wisp is centuries old and home to");
    text.drawText(display->renderer,48,159,0,"some of the richest men on the island - the newly formed island.");
    text.drawText(display->renderer,48,171,0,"Once Wisp was part of the main continent. But there was an accident");
    text.drawText(display->renderer,48,183,0,"caused by the Mage Guild. Too much life energy from the planet was");
    text.drawText(display->renderer,48,195,0,"released at once, rending the land apart. This island broke off and");
    text.drawText(display->renderer,48,207,0,"many perished. Wisp was fortunate and survived.");
    text.drawText(display->renderer,48,222,0,"    Magic was banned from the land and the Mage Guild was disbanded.");
    text.drawText(display->renderer,48,234,0,"Many of its members were captured and executed. A radical group");
    text.drawText(display->renderer,48,246,0,"known as the Rein emerged to hunt down those mages that escaped");
    text.drawText(display->renderer,48,258,0,"punishment. But the Rein went corrupt. Its bloodthirsty dark priests");
    text.drawText(display->renderer,48,270,0,"kill more innocent people than mages.");
    text.drawText(display->renderer,48,285,0,"    The Thieves' Guild was lucky. A few mages sought sanctuary and");
    text.drawText(display->renderer,48,297,0,"have proven useful members. Strange, though, that one would be put");
    text.drawText(display->renderer,48,309,0,"on this assignment, a simple robbery. Harold, a rich collector of");
    text.drawText(display->renderer,48,321,0,"jewels, has in his possession a rare gem. But what is it about this");
    text.drawText(display->renderer,48,333,0,"gem that the client requested a mage to be present? And who is this");
    text.drawText(display->renderer,48,345,0,"mysterious client? His identity is unknown to all but the highest");
	  text.drawText(display->renderer,48,357,0,"guild members.");
    text.drawText(display->renderer,48,372,0,"    The guards are picking up their watch. Perhaps they are");
    text.drawText(display->renderer,48,384,0,"suspicious? The three adventurers scale the gate around the mansion");
    text.drawText(display->renderer,48,396,0,"and head inside...                   <click to continue>");
	}

	//Show current name and button
	if(mode==0){
		text.drawText(display->renderer,222,200,0,party->Players[currentPlayer].PlayerName);
		if(Cursor>-1 && (*animCounter==1 || *animCounter==2))	text.drawText(display->renderer,222+8*Cursor,200,0,"|");
    SDL_RenderCopy(display->renderer, ddGfx->Windows->texture, &ddGfx->aWindows[18], &button);
		//ddObj->ddsb->Blt(&button, ddGfx->Windows,&ddGfx->aWindows[18],DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}

	//Diagnostics
	//sprintf(str,"%d",Cursor);
	//text.drawText(display->renderer,80,350,0,str);

	//Display border
	r.y=0;
	r.x=0;
	r.h=480;
	r.w=640;
  SDL_RenderCopy(display->renderer, ddGfx->Border->texture, &r, &r);
	//ddObj->ddsb->Blt(&r,ddGfx->Border,&r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	//Mouse
	r.y=diObj->MouseY()-16;
	r.x=diObj->MouseX()-16;
	r.h=32;
	r.w=32;
  SDL_RenderCopy(display->renderer, ddGfx->Tiles[0]->texture, &ddGfx->vTile->at(20).r, &r);
	//ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);

	return true;

}

char cDA2Intro::DIKtoASCII(DA2KEYS dik){
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