#include "DA2SpellWin.h"

cSpellWin::cSpellWin(){
	party=NULL;
	activeSpell=0;
	isSelecting=false;
}

cSpellWin::~cSpellWin(){
	party=NULL;
}

bool cSpellWin::SetWindow(int x, int y, cDA2Party *par){
  /*1
	if((party=par)==NULL) return false;
	xPos=x;
	yPos=y;

	buttons[0].left=x+213; buttons[0].top=y+32; buttons[0].right=x+229; buttons[0].bottom=y+48;
	buttons[1].left=x+171; buttons[1].top=y+193; buttons[1].right=buttons[1].left+61; buttons[1].bottom=buttons[1].top+24;

	popUpList[0].left=x+256; popUpList[0].right=x+354; popUpList[0].top=y+13; popUpList[0].bottom=y+29;
	popUpList[1].left=x+256; popUpList[1].right=x+354; popUpList[1].top=y+29; popUpList[1].bottom=y+45;
	popUpList[2].left=x+256; popUpList[2].right=x+354; popUpList[2].top=y+45; popUpList[2].bottom=y+61;
	popUpList[3].left=x+256; popUpList[3].right=x+354; popUpList[3].top=y+61; popUpList[3].bottom=y+77;
	popUpList[4].left=x+256; popUpList[4].right=x+354; popUpList[4].top=y+77; popUpList[4].bottom=y+93;
	popUpList[5].left=x+256; popUpList[5].right=x+354; popUpList[5].top=y+93; popUpList[5].bottom=y+109;

	targets[0].left=x+14; targets[0].right=x+231; targets[0].top=y+68; targets[0].bottom=y+100;
	targets[1].left=x+14; targets[1].right=x+231; targets[1].top=y+100; targets[1].bottom=y+132;
	targets[2].left=x+14; targets[2].right=x+231; targets[2].top=y+132; targets[2].bottom=y+164;

	//text.Init(ddGfx);
	box.Init(ddObj,ddGfx);
  */
	return true;
}

void cSpellWin::Update(){
	int i=0;
	for(i=0;i<6;i++){
		if(party->Spells[21+i].Known) spellsKnown[i]=true;
	}
}

bool cSpellWin::Render(bool mouse){
  /*1
	RECT r;
	int i;
	char str[32];

	//Draw the dialog window
	r.left=xPos; r.right=r.left+245;
	r.top=yPos;r.bottom=r.top+180;
	ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[9], DDBLT_WAIT,NULL);

	//Draw the buttons
	r.top=yPos+180;
	r.bottom=r.top+50;
	ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[2], DDBLT_WAIT,NULL);
	if(isSelecting) ddObj->ddsb->Blt(&buttons[0], ddGfx->Windows, &ddGfx->aWindows[21], DDBLT_WAIT,NULL);
	else ddObj->ddsb->Blt(&buttons[0], ddGfx->Windows, &ddGfx->aWindows[22], DDBLT_WAIT,NULL);

	//No spells if mage is dead
	if(party->Players[0].HP<1){

		sprintf(str,"Alas! %s has perished.",party->Players[0].PlayerName);
		text.DrawText(ddObj->ddsb,xPos+32,yPos+100,20,str);

	} else {

		//Draw the MP
		sprintf(str,"MP:\n%d/%d",party->Players[0].MP,party->Players[0].MaxMP);
		text.DrawText(ddObj->ddsb,xPos+16,yPos+16,0,str);

		//Draw the ActiveSpell
		sprintf(str,"Spell: %s\nCost: %d MP",party->Spells[21+activeSpell].SpellName,party->Spells[21+activeSpell].MPCost);
		text.DrawText(ddObj->ddsb,xPos+96,yPos+16,0,str);

		//Draw the SpellSelector (if active)
		if(isSelecting){
			r.left=xPos+245; r.right=xPos+367;
			r.top=yPos;r.bottom=yPos+122;
			ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[23], DDBLT_WAIT,NULL);
			for(i=0;i<6;i++){
				if(spellsKnown[i]){
					if(party->Players[0].MP>=party->Spells[i+21].MPCost){
						text.DrawText(ddObj->ddsb,xPos+258,yPos+13+i*16,0,party->Spells[i+21].SpellName);
					} else {
						text.DrawText(ddObj->ddsb,xPos+258,yPos+13+i*16,0,party->Spells[i+21].SpellName,1);
					}
				}
			}
		}

		//Highlight selectable areas
		if(isSelecting){
			for(i=0;i<6;i++){
				if(diObj->MouseX()>=popUpList[i].left && diObj->MouseX()<popUpList[i].right && 
					 diObj->MouseY()>=popUpList[i].top && diObj->MouseY()<popUpList[i].bottom && spellsKnown[i]) {
					box.DrawBox(popUpList[i].left,popUpList[i].top,popUpList[i].right,popUpList[i].bottom,15);
				}
			}
		} else {
			for(i=0;i<3;i++){
				if(diObj->MouseX()>=targets[i].left && diObj->MouseX()<targets[i].right && 
					 diObj->MouseY()>=targets[i].top && diObj->MouseY()<targets[i].bottom &&
					 party->Players[0].MP>=party->Spells[activeSpell+21].MPCost) {
					if( (activeSpell<4 && party->Players[i].HP<party->Players[i].MaxHP && party->Players[i].HP>0) ||
							(activeSpell>3 && party->Players[i].HP<1) ){
						box.DrawBox(targets[i].left,targets[i].top,targets[i].right,targets[i].bottom,15);
					}
				}
			}
		}

		//Draw the character stats
		r.left=xPos+32; r.right=xPos+64;
		for(i=0;i<3;i++){
			r.top=i*32+68+yPos; r.bottom=i*32+100+yPos;
			if(party->Players[i].HP>0) ddObj->ddsb->Blt(&r, ddGfx->Chars[0],&ddGfx->vNPC->at(i*16).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			else ddObj->ddsb->Blt(&r,  ddGfx->Tiles[ddGfx->vTile->at(1560+i).index],&ddGfx->vTile->at(1560+i).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			sprintf(str,"%s\nHP: %d/%d",party->Players[i].PlayerName,party->Players[i].HP,party->Players[i].MaxHP);
			if( (activeSpell<4 && party->Players[i].HP<party->Players[i].MaxHP && party->Players[i].HP>0) ||
				  (activeSpell>3 && party->Players[i].HP<1) ){
				text.DrawText(ddObj->ddsb,xPos+68,i*32+68+yPos,0,str);
			} else {
				text.DrawText(ddObj->ddsb,xPos+68,i*32+68+yPos,0,str,1);
			}
		}

	}

	//Draw mouse cursor
	if(mouse){
		r.top=diObj->MouseY()-16;
		r.left=diObj->MouseX()-16;
		r.bottom=r.top+32;
		r.right=r.left+32;
		ddObj->ddsb->Blt(&r, ddGfx->Tiles[0],&ddGfx->vTile->at(20).r,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
	}
	*/
	return true;
}

//This function checks to see if a button was pushed
bool cSpellWin::Logic(){
  /*1
	int i;
	int effect;

	diObj->pollEvents();

	//Make sure keys and buttons aren't locked
	if( diObj->CheckLockMouse(0) ) return true;

	if( diObj->MousePress(0) ){
		diObj->LockMouse(0);

		//Only a live mage can cast.
		if(party->Players[0].HP>0){
		
			//Check spell selection
			if(isSelecting){
				for(i=0;i<6;i++){
					if(diObj->MouseX()>=popUpList[i].left && diObj->MouseX()<popUpList[i].right && 
						 diObj->MouseY()>=popUpList[i].top && diObj->MouseY()<popUpList[i].bottom && spellsKnown[i]) {
						activeSpell=i;
						isSelecting=false;
					}
				}
			}

			//Check spell casting
			for(i=0;i<3;i++){
				if(diObj->MouseX()>=targets[i].left && diObj->MouseX()<targets[i].right && 
					 diObj->MouseY()>=targets[i].top && diObj->MouseY()<targets[i].bottom &&
					 party->Players[0].MP>=party->Spells[activeSpell+21].MPCost) {
					if( (activeSpell<4 && party->Players[i].HP<party->Players[i].MaxHP && party->Players[i].HP>0) ||
							(activeSpell>3 && party->Players[i].HP<1) ){
						switch(activeSpell){
						case 0:
							effect = (int)(10 * (party->Players[0].ModInt) / 25);
							if(effect > 50) effect = 50;
							break;	
						case 1:
							effect = (int)(25 * (party->Players[0].ModInt) / 25);
							if(effect > 150) effect = 150;
							break;	
						case 2:
							effect = (int)(50 * (party->Players[0].ModInt) / 25);
							if(effect > 350) effect = 350;
							break;	
						case 3:
						case 5:
							effect = party->Players[i].MaxHP;
							break;	
						case 4:
							effect = 1;
							break;
						default:
							break;
						}
						party->Players[i].HP+=effect;
						if(party->Players[i].HP>party->Players[i].MaxHP) party->Players[i].HP=party->Players[i].MaxHP;
						party->Players[0].MP-=party->Spells[activeSpell+21].MPCost;
					}
				}
			}

			//Check the spell selection button
			if(diObj->MouseX()>=buttons[0].left && diObj->MouseX()<buttons[0].right && 
				 diObj->MouseY()>=buttons[0].top && diObj->MouseY()<buttons[0].bottom){
				if(isSelecting) isSelecting=false;
				else isSelecting=true;
			}

		}

		//close the spell window
		if(diObj->MouseX()>=buttons[1].left && diObj->MouseX()<buttons[1].right && 
			 diObj->MouseY()>=buttons[1].top && diObj->MouseY()<buttons[1].bottom){
			return false;
		}
		
	}
  */
	return true;
}