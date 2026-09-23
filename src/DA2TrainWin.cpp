#include "DA2TrainWin.h"

cTrainWin::cTrainWin(){
	party=NULL;
}

cTrainWin::~cTrainWin(){
	party=NULL;
}

bool cTrainWin::SetWindow(int x, int y, int cst, int stt, cDA2Party *par){
  /*1
	if((party=par)==NULL) return false;
	xPos=x;
	yPos=y;
	cost=cst;
	stat=stt;

	buttons[0].left=x+107; buttons[0].top=y+153; buttons[0].right=buttons[0].left+61; buttons[0].bottom=buttons[0].top+24;
	buttons[1].left=x+171; buttons[1].top=y+153; buttons[1].right=buttons[1].left+61; buttons[1].bottom=buttons[1].top+24;

	sliderButtons[0].left=x+170; sliderButtons[0].right=x+186; sliderButtons[0].top=y+21; sliderButtons[0].bottom=y+37;
	sliderButtons[1].left=x+170; sliderButtons[1].right=x+186; sliderButtons[1].top=y+51; sliderButtons[1].bottom=y+67;
	sliderButtons[2].left=x+170; sliderButtons[2].right=x+186; sliderButtons[2].top=y+81; sliderButtons[2].bottom=y+97;
	sliderButtons[3].left=x+188; sliderButtons[3].right=x+204; sliderButtons[3].top=y+21; sliderButtons[3].bottom=y+37;
	sliderButtons[4].left=x+188; sliderButtons[4].right=x+204; sliderButtons[4].top=y+51; sliderButtons[4].bottom=y+67;
	sliderButtons[5].left=x+188; sliderButtons[5].right=x+204; sliderButtons[5].top=y+81; sliderButtons[5].bottom=y+97;

	price=0;
	bAccept=false;
	text.Init(ddGfx);
  */
	return true;
	
}

bool cTrainWin::Render(bool mouse){
  /*1
	RECT r;
	int i;
	char str[32];

	//Draw the dialog window
	r.left=xPos;
	r.top=yPos;
	r.right=r.left+245;
	r.bottom=r.top+140;
	ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[1], DDBLT_WAIT,NULL);

	//Draw the main buttons
	r.top=yPos+140;
	r.bottom=r.top+50;
	ddObj->ddsb->Blt(&r, ddGfx->Windows, &ddGfx->aWindows[3], DDBLT_WAIT,NULL);
	
	//Draw current player stats
	for(i=0;i<3;i++){
		text.DrawText(ddObj->ddsb,xPos+15,yPos+10+i*30,25,party->Players[i].PlayerName );
		switch(stat){
		case 1:
			sprintf(str,"Str:%d/%d",party->Players[i].ModStr,party->Players[i].BaseStr);
			break;
		case 2:
			sprintf(str,"Agl:%d/%d",party->Players[i].ModAgl,party->Players[i].BaseAgl);
			break;
		case 3:
			sprintf(str,"Int:%d/%d",party->Players[i].ModInt,party->Players[i].BaseInt);
			break;
		case 4:
			sprintf(str,"Spd:%d/%d",party->Players[i].ModSpeed,party->Players[i].BaseSpeed);
			break;
		default:
			break;
		}
		text.DrawText(ddObj->ddsb,xPos+15,yPos+25+i*30,25,str);
	}

	//Draw the slider buttons
	for(i=0;i<6;i++){
		if(i<3) ddObj->ddsb->Blt(&sliderButtons[i], ddGfx->Windows, &ddGfx->aWindows[21], DDBLT_WAIT,NULL);
		else ddObj->ddsb->Blt(&sliderButtons[i], ddGfx->Windows, &ddGfx->aWindows[22], DDBLT_WAIT,NULL);
	}

	//Draw training points
	for(i=0;i<3;i++){
		sprintf(str,"TP: %d",party->Players[i].TP-train[i]);
		text.DrawText(ddObj->ddsb,xPos+110,yPos+22+i*30,10,str);
		sprintf(str,"%d",train[i]);
		text.DrawText(ddObj->ddsb,xPos+210,yPos+22+i*30,10,str);
	}
		
	//Draw additional text
	sprintf(str,"Gold: %d",party->Gold);
	text.DrawText(ddObj->ddsb,xPos+15,yPos+110,20,str);
	sprintf(str,"Cost: %d",price);
	text.DrawText(ddObj->ddsb,xPos+120,yPos+110,20,str);

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
bool cTrainWin::Logic(){
  /*1
	int i;

	diObj->Read();

	//Make sure keys and buttons aren't locked
	if( diObj->CheckLockMouse(0) ) return true;

	if( diObj->MousePress(0) ){
		diObj->LockMouse(0);

		//Check the slider buttons
		for(i=0;i<6;i++){
			if(diObj->MouseX()>=sliderButtons[i].left && diObj->MouseX()<sliderButtons[i].right && 
				 diObj->MouseY()>=sliderButtons[i].top && diObj->MouseY()<sliderButtons[i].bottom){
				break;
			}
		}
		switch(i){
		case 0:
			if(train[0]>0) {
				train[0]--;
				price-=cost;
			}
			break;
		case 3:
			if(party->Players[0].TP>train[0] && (price+cost)<=party->Gold){
				train[0]++;
				price+=cost;
			}
			break;
		case 1:
			if(train[1]>0) {
				train[1]--;
				price-=cost;
			}
			break;
		case 4:
			if(party->Players[1].TP>train[1] && (price+cost)<=party->Gold){
				train[1]++;
				price+=cost;
			}
			break;
		case 2:
			if(train[2]>0) {
				train[2]--;
				price-=cost;
			}
			break;
		case 5:
			if(party->Players[2].TP>train[2] && (price+cost)<=party->Gold){
				train[2]++;
				price+=cost;
			}
			break;
		default:
			break;
		}
			
		//Check the dialog buttons
		if(diObj->MouseX()>=buttons[0].left && diObj->MouseX()<buttons[0].right && 
			 diObj->MouseY()>=buttons[0].top && diObj->MouseY()<buttons[0].bottom){
			bAccept=true;
			switch(stat){
			case 1:
				for(i=0;i<3;i++){
					party->Players[i].BaseStr+=train[i]*2;
					party->Players[i].ModStr+=train[i]*2;
					party->Players[i].MaxHP+=train[i]*4;
					party->Players[i].HP+=train[i]*4;
				}
				break;
			case 2:
				for(i=0;i<3;i++){
					party->Players[i].BaseAgl+=train[i]*2;
					party->Players[i].ModAgl+=train[i]*2;
				}
				break;
			case 3:
				for(i=0;i<3;i++){
					party->Players[i].BaseInt+=train[i]*2;
					party->Players[i].ModInt+=train[i]*2;
					if(i==0){
						party->Players[i].MaxMP+=train[i]*4;
						party->Players[i].MP+=train[i]*4;
					}
				}
				break;
			case 4:
				for(i=0;i<3;i++){
					party->Players[i].BaseSpeed+=train[i]*2;
					party->Players[i].ModSpeed+=train[i]*2;
				}
				break;
			default:
				break;
			}
			for(i=0;i<3;i++) {
				party->Players[i].TP-=train[i];
				train[i]=0;
			}
			party->Gold-=price;
			return false;
		}

		if(diObj->MouseX()>=buttons[1].left && diObj->MouseX()<buttons[1].right && 
			 diObj->MouseY()>=buttons[1].top && diObj->MouseY()<buttons[1].bottom){
			bAccept=false;
			for(i=0;i<3;i++) train[i]=0;
			return false;
		}
		
	}
  */
	return true;
}