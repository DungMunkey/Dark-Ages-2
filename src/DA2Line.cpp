#include "DA2Line.h"
#include <iostream>

cLine::cLine(){
	display=NULL;
}

cLine::~cLine(){
	display=NULL;
}

bool cLine::Init(CDisplay *d){
	if((display=d)==NULL) return false;
	return true;
}

//Adapted from Greg Damon @ thepixels.net
void cLine::DrawLine (int x1, int y1, int x2, int y2, char color) {
  /*1
	int dx;		//deltas
  int dy;
  int dx2;	//scaled deltas
  int dy2;
  int ix;		//increase rate on the x axis
  int iy;		//increase rate on the y axis
  int err;	//the error term
  int i;		//looping variable
	int yPos=y1;
	int xPos=x1;

	//Lock the backbuffer
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddObj->ddsb->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_READONLY, NULL);

	// difference between starting and ending points
	dx = x2 - x1;
	dy = y2 - y1;

	// calculate direction of the vector and store in ix and iy
	if (dx >= 0)	ix = 1;

	if (dx < 0)	{
		ix = -1;
		dx = abs(dx);
	}

	if (dy >= 0) iy = 1;

	if (dy < 0)	{
		iy = -1;
		dy = abs(dy);
	}

	// scale deltas and store in dx2 and dy2
	dx2 = dx * 2;
	dy2 = dy * 2;

	if (dx > dy) {	// dx is the major axis
		// initialize the error term
		err = dy2 - dx;

		for (i = 0; i <= dx; i++)		{
			if(xPos>0 && xPos<640 && yPos>0 && yPos<480) ((BYTE *) ddsd.lpSurface)[yPos * ddsd.lPitch + xPos]=color;
			if (err >= 0)			{
				err -= dx2;
				yPos+=iy;
			}
			err += dy2;
			xPos+=ix;
		}
	}	else {		// dy is the major axis
		// initialize the error term
		err = dx2 - dy;

		for (i = 0; i <= dy; i++)		{
			if(xPos>0 && xPos<640 && yPos>0 && yPos<480) ((BYTE *) ddsd.lpSurface)[yPos * ddsd.lPitch + xPos]=color;
			if (err >= 0)			{
				err -= dy2;
				xPos+=ix;
			}
			err += dx2;
			yPos+=iy;
		}
	}

	ddObj->ddsb->Unlock(NULL);
  */

}