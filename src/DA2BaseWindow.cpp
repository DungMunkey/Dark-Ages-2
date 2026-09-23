#include "DA2BaseWindow.h"

cBaseWindow::cBaseWindow(){}

cBaseWindow::~cBaseWindow(){
	display=NULL;
	ddGfx=NULL;
	diObj=NULL;
}

bool cBaseWindow::Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp){
	if((display=d)==NULL) return false;
	if((ddGfx=gfx)==NULL) return false;
	if((diObj=inp)==NULL) return false;
	if(!text.Init(gfx)) return false;
	xPos=0;
	yPos=0;
	return true;
}

void cBaseWindow::SetPos(int x, int y){
	xPos=x;
	yPos=y;
}

//bool cBaseWindow::Render(){return true;}
//bool cBaseWindow::Logic(){return true;}