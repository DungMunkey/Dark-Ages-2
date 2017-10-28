#include <iostream>
#include "DA2Gfx.h"


cDA2Gfx::cDA2Gfx(){
	int i;
	for(i=0;i<8;i++) Tiles[i]=NULL;
	for(i=0;i<4;i++) Chars[i]=NULL;
	for(i=0;i<4;i++) Monsters[i]=NULL;
	for(i=0;i<2;i++) Objects[i]=NULL;
	for(i=0;i<3;i++) Stats[i]=NULL;
	for(i=0;i<7;i++) Battle[i]=NULL;
	Font=NULL;
	Cursor=NULL;
	Frame=NULL;
	Windows=NULL;
	Shop=NULL;
	Border=NULL;
	WorldMap=NULL;
	JournalFace=NULL;
	TextSurface=NULL;

	vTile = new vector<TileType>;
	vNPC = new vector<TileType>;
	vObj = new vector<TileType>;
	vMonster = new vector<TileType>;
}

cDA2Gfx::~cDA2Gfx(){
	int i;
  for(i=0; i<8; i++) delete Tiles[i];
  for(i=0; i<4; i++) delete Chars[i];
  for(i=0; i<4; i++) delete Monsters[i];
  for(i=0; i<2; i++) delete Objects[i];
  for(i=0; i<3; i++) delete Stats[i];
  for(i=0; i<7; i++) delete Battle[i];
  delete Font;
  delete Cursor;
  delete Frame;
  delete Windows;
  delete Shop;
  delete Border;
  delete WorldMap;
  delete JournalFace;
  delete TextSurface;

	delete vTile;
	delete vNPC;
	delete vObj;
	delete vMonster;
}

/*
bool cDA2Gfx::Init(cDX *dd7){
	if((ddObj=dd7)==NULL) return false;
	return true;
}
*/


/*
void cDA2Gfx::CreateSurfaces(){
	int i;

	//Set up generic surface description
	DDSURFACEDESC2 ddsdGeneric;
	ZeroMemory(&ddsdGeneric, sizeof(ddsdGeneric));
	ddsdGeneric.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsdGeneric.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsdGeneric.dwHeight = 480;
	ddsdGeneric.dwWidth = 640;
	ddsdGeneric.dwSize = sizeof(ddsdGeneric);

	//Load Palette
	ddPal = ddObj->DA2LoadPalette(ddObj->dd, "Gfx/da2pal.txt");

	for(i=0;i<8;i++) ddObj->dd->CreateSurface(&ddsdGeneric, &Tiles[i], NULL);
	for(i=0;i<4;i++) ddObj->dd->CreateSurface(&ddsdGeneric, &Chars[i], NULL);
	for(i=0;i<4;i++) ddObj->dd->CreateSurface(&ddsdGeneric, &Monsters[i], NULL);
	for(i=0;i<2;i++) ddObj->dd->CreateSurface(&ddsdGeneric, &Objects[i], NULL);
	for(i=0;i<3;i++) ddObj->dd->CreateSurface(&ddsdGeneric, &Stats[i], NULL);
	for(i=0;i<7;i++) ddObj->dd->CreateSurface(&ddsdGeneric, &Battle[i], NULL);

	ddObj->dd->CreateSurface(&ddsdGeneric, &Shop, NULL);
	ddObj->dd->CreateSurface(&ddsdGeneric, &Border, NULL);
	ddObj->dd->CreateSurface(&ddsdGeneric, &JournalFace, NULL);

	ddsdGeneric.dwHeight = 504;
	ddsdGeneric.dwWidth = 824;
	ddObj->dd->CreateSurface(&ddsdGeneric, &Windows, NULL);

	ddsdGeneric.dwHeight = 384;
	ddsdGeneric.dwWidth = 256;
	ddObj->dd->CreateSurface(&ddsdGeneric, &Font, NULL);

	ddsdGeneric.dwHeight = 220;
	ddsdGeneric.dwWidth = 188;
	ddObj->dd->CreateSurface(&ddsdGeneric, &TextSurface, NULL);

	ddsdGeneric.dwHeight = 1024;
	ddsdGeneric.dwWidth = 1024;
	ddObj->dd->CreateSurface(&ddsdGeneric, &WorldMap, NULL);

	ddsdGeneric.dwHeight = 256;
	ddsdGeneric.dwWidth = 352;
	ddObj->dd->CreateSurface(&ddsdGeneric, &Frame, NULL);

	ddsdGeneric.dwHeight = 216;
	ddsdGeneric.dwWidth = 16;
	ddObj->dd->CreateSurface(&ddsdGeneric, &Cursor, NULL);


	//Set Palletes
	for(i=0;i<8;i++) Tiles[i]->SetPalette(ddPal);
	for(i=0;i<4;i++) Chars[i]->SetPalette(ddPal);
	for(i=0;i<4;i++) Monsters[i]->SetPalette(ddPal);
	for(i=0;i<2;i++) Objects[i]->SetPalette(ddPal);
	for(i=0;i<3;i++) Stats[i]->SetPalette(ddPal);
	for(i=0;i<7;i++) Battle[i]->SetPalette(ddPal);
	Cursor->SetPalette(ddPal);
	TextSurface->SetPalette(ddPal);
	Frame->SetPalette(ddPal);
	WorldMap->SetPalette(ddPal);
	Shop->SetPalette(ddPal);
	Border->SetPalette(ddPal);
	Font->SetPalette(ddPal);
	Windows->SetPalette(ddPal);
	JournalFace->SetPalette(ddPal);

	//Make a small clipper
	ddObj->dd->CreateClipper(0,&TextClip,NULL);

	LPRGNDATA lpClipList = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
	RECT rcClipRect = {0, 0, 188, 220};

	memcpy(lpClipList->Buffer, &rcClipRect, sizeof(RECT));  // copy the actual clip region
	lpClipList->rdh.dwSize = sizeof(RGNDATAHEADER);         // size of header structure
	lpClipList->rdh.iType = RDH_RECTANGLES;                 // type of clip region
	lpClipList->rdh.nCount = 1;                             // number of clip regions
	lpClipList->rdh.nRgnSize = sizeof(RECT);                // size of lpClipList->Buffer
	lpClipList->rdh.rcBound = rcClipRect;                   // the bounding RECT
	TextClip->SetClipList(lpClipList,0);
	TextSurface->SetClipper(TextClip);

}
*/

void cDA2Gfx::CutTiles(){
	TileType t;
	int i;
	int x,y,z;
	SDL_Rect r;
	FILE *f;

	//Cut bitmaps into tiles
	i = 0;
	t.solid=0;
  t.r.w=32;
  t.r.h=32;
	for(z=0;z<8;z++){
		for(y=0;y<15;y++){
			for(x=0;x<20;x++){
    
        t.r.x = x * 32;
        t.r.y = y * 32;
				t.index = z;
				vTile->push_back(t);
				if(i<1184) vNPC->push_back(t);
        if(i<600)	vObj->push_back(t);
    
        i++;
			}
		}
	}

	//Cut monster bitmaps into tiles
	i = 0;
  t.r.w=96;
  t.r.h=96;
	for(z=0;z<4;z++){
		for(y=0;y<5;y++){
			for(x=0;x<6;x++){
    
        t.r.x = x * 96;
        t.r.y = y * 96;
				t.index = z;
				vMonster->push_back(t);   
        i++;
			}
		}
	}

	//Make font rects
	for(z=0;z<8;z++){
		i=32;
		for(y=0;y<3;y++){
			for(x=0;x<32;x++){
				r.x = x * 8;
				r.w = 8;
				r.y = z * 48 + y * 16;
				r.h = 16;
				aFont[z][i]=r;
				i++;
			}
		}
	}

	//Make cursor rects;
	r.x=0;
	r.w=16;
  r.h=24;
	for(y=0;y<9;y++){
		r.y=y*24;
		aCursor[y]=r;
	}

	//Battle Status rects
  r.w=16;
  r.h=16;
	for(y=0;y<2;y++){
		for(x=0;x<4;x++){
			r.x=224+x*16;
			r.y=0+y*16;
			aBattleStatus[y*4+x]=r;
		}
	}
	r.x=288;
	r.w=16;
	r.y=0;
	r.h=16;
	aBattleStatus[8]=r;

	//Load cursor hotspots
	f=fopen("Gfx/cursor.dat","rt");
	for(x=0;x<9;x++) fscanf(f,"%d,%d\n",&CursorX[x], &CursorY[x]);
  fclose(f);

	//Load window rects
	f=fopen("Gfx/win.dat","rt");
	for(x=0;x<33;x++){
		fscanf(f,"%d,%d,%d,%d\n",&r.x, &r.y, &r.w, &r.h);
    r.w-=r.x;
    r.h-=r.y;
    aWindows[x]=r;
  }
  fclose(f);

	//Set solidity level
  f=fopen("Data/KABOOM.OOF","rt");
	for(x=0;x<3000;x++){
		fscanf(f,"%d\n",&y);
		if(y>0) vTile->at(x).solid=1;
	}
  fclose(f);
}

/*
void cDA2Gfx::LoadGFX(){
	int i;
	
	//Set up generic surface description
	DDSURFACEDESC2 ddsdGeneric;
	ZeroMemory(&ddsdGeneric, sizeof(ddsdGeneric));
	ddsdGeneric.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsdGeneric.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsdGeneric.dwHeight = 480;
	ddsdGeneric.dwWidth = 640;
	ddsdGeneric.dwSize = sizeof(ddsdGeneric);

	//Load graphics onto surfaces
  LoadBitmapResource(Tiles[0],0,0,"Gfx/1.bmp");
	LoadBitmapResource(Tiles[1],0,0,"Gfx/2.bmp");
  LoadBitmapResource(Tiles[2],0,0,"Gfx/3.bmp");
	LoadBitmapResource(Tiles[3],0,0,"Gfx/4.bmp");
  LoadBitmapResource(Tiles[4],0,0,"Gfx/5.bmp");
	LoadBitmapResource(Tiles[5],0,0,"Gfx/6.bmp");
  LoadBitmapResource(Tiles[6],0,0,"Gfx/7.bmp");
	LoadBitmapResource(Tiles[7],0,0,"Gfx/8.bmp");

  LoadBitmapResource(Chars[0],0,0,"Gfx/Char1.bmp");
	LoadBitmapResource(Chars[1],0,0,"Gfx/Char2.bmp");
	LoadBitmapResource(Chars[2],0,0,"Gfx/Char3.bmp");
	LoadBitmapResource(Chars[3],0,0,"Gfx/Char4.bmp");

  LoadBitmapResource(Monsters[0],0,0,"Gfx/mon1.bmp");
	LoadBitmapResource(Monsters[1],0,0,"Gfx/mon2.bmp");
	LoadBitmapResource(Monsters[2],0,0,"Gfx/mon3.bmp");
	LoadBitmapResource(Monsters[3],0,0,"Gfx/mon4.bmp");

  LoadBitmapResource(Objects[0],0,0,"Gfx/Obj1.bmp");
	LoadBitmapResource(Objects[1],0,0,"Gfx/Obj2.bmp");

  LoadBitmapResource(Stats[0],0,0,"Gfx/stat1.bmp");
	LoadBitmapResource(Stats[1],0,0,"Gfx/stat2.bmp");
	LoadBitmapResource(Stats[2],0,0,"Gfx/stat3.bmp");

	LoadBitmapResource(Battle[0],0,0,"Gfx/bg1.bmp");
	LoadBitmapResource(Battle[1],0,0,"Gfx/bg2.bmp");
	LoadBitmapResource(Battle[2],0,0,"Gfx/bg3.bmp");
	LoadBitmapResource(Battle[3],0,0,"Gfx/bg4.bmp");
	LoadBitmapResource(Battle[4],0,0,"Gfx/bg5.bmp");
	LoadBitmapResource(Battle[5],0,0,"Gfx/bg6.bmp");
	LoadBitmapResource(Battle[6],0,0,"Gfx/bg7.bmp");

	LoadBitmapResource(Shop,0,0,"Gfx/barter2.bmp");
	LoadBitmapResource(JournalFace,0,0,"Gfx/journal.bmp");
	LoadBitmapResource(Border,0,0,"Gfx/border.bmp");

	ddsdGeneric.dwHeight = 504;
	ddsdGeneric.dwWidth = 824;
	LoadBitmapResource(Windows,0,0,"Gfx/windows.bmp");

	ddsdGeneric.dwHeight = 384;
	ddsdGeneric.dwWidth = 256;
	LoadBitmapResource(Font,0,0,"Gfx/font.bmp");

	ddsdGeneric.dwHeight = 1024;
	ddsdGeneric.dwWidth = 1024;
	LoadBitmapResource(WorldMap,0,0,"Gfx/map.bmp");

	ddsdGeneric.dwHeight = 256;
	ddsdGeneric.dwWidth = 352;
	LoadBitmapResource(Frame,0,0,"Gfx/loadsave.bmp");

	ddsdGeneric.dwHeight = 216;
	ddsdGeneric.dwWidth = 16;
	LoadBitmapResource(Cursor,0,0,"Gfx/cursors.bmp");


	DDCOLORKEY	ddck;
	ddck.dwColorSpaceLowValue = 0;
  ddck.dwColorSpaceHighValue = 0;
  for(i=0;i<8;i++) Tiles[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	for(i=0;i<4;i++) Chars[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	for(i=0;i<4;i++) Monsters[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	for(i=0;i<2;i++) Objects[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	for(i=0;i<3;i++) Stats[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	for(i=0;i<7;i++) Battle[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	Windows->SetColorKey(DDCKEY_SRCBLT, &ddck);
	Shop->SetColorKey(DDCKEY_SRCBLT, &ddck);
	Border->SetColorKey(DDCKEY_SRCBLT, &ddck);
	Font->SetColorKey(DDCKEY_SRCBLT, &ddck);
	WorldMap->SetColorKey(DDCKEY_SRCBLT, &ddck);
	JournalFace->SetColorKey(DDCKEY_SRCBLT, &ddck);
	TextSurface->SetColorKey(DDCKEY_SRCBLT, &ddck);
	Frame->SetColorKey(DDCKEY_SRCBLT, &ddck);
	Cursor->SetColorKey(DDCKEY_SRCBLT, &ddck);

}
*/

bool cDA2Gfx::LoadGfx(CDisplay* display){
  Tiles[0] = new CGraphic("Gfx/1.bmp", display, true, 0, 0, 0);
  Tiles[1]= new CGraphic("Gfx/2.bmp", display, true, 0, 0, 0);
  Tiles[2]= new CGraphic("Gfx/3.bmp", display, true, 0, 0, 0);
  Tiles[3]= new CGraphic("Gfx/4.bmp", display, true, 0, 0, 0);
  Tiles[4]= new CGraphic("Gfx/5.bmp", display, true, 0, 0, 0);
  Tiles[5]= new CGraphic("Gfx/6.bmp", display, true, 0, 0, 0);
  Tiles[6]= new CGraphic("Gfx/7.bmp", display, true, 0, 0, 0);
  Tiles[7]= new CGraphic("Gfx/8.bmp", display, true, 0, 0, 0);

  Chars[0]= new CGraphic("Gfx/Char1.bmp", display, true, 0, 0, 0);
  Chars[1]= new CGraphic("Gfx/Char2.bmp", display, true, 0, 0, 0);
  Chars[2]= new CGraphic("Gfx/Char3.bmp", display, true, 0, 0, 0);
  Chars[3]= new CGraphic("Gfx/Char4.bmp", display, true, 0, 0, 0);

  Monsters[0]= new CGraphic("Gfx/mon1.bmp", display, true, 0, 0, 0);
  Monsters[1]= new CGraphic("Gfx/mon2.bmp", display, true, 0, 0, 0);
  Monsters[2]= new CGraphic("Gfx/mon3.bmp", display, true, 0, 0, 0);
  Monsters[3]= new CGraphic("Gfx/mon4.bmp", display, true, 0, 0, 0);

  Objects[0]= new CGraphic("Gfx/Obj1.bmp", display, true, 0, 0, 0);
  Objects[1]= new CGraphic("Gfx/Obj2.bmp", display, true, 0, 0, 0);

  Stats[0]= new CGraphic("Gfx/stat1.bmp", display, true, 0, 0, 0);
  Stats[1]= new CGraphic("Gfx/stat2.bmp", display, true, 0, 0, 0);
  Stats[2]= new CGraphic("Gfx/stat3.bmp", display, true, 0, 0, 0);

  Battle[0]= new CGraphic("Gfx/bg1.bmp", display, true, 0, 0, 0);
  Battle[1]= new CGraphic("Gfx/bg2.bmp", display, true, 0, 0, 0);
  Battle[2]= new CGraphic("Gfx/bg3.bmp", display, true, 0, 0, 0);
  Battle[3]= new CGraphic("Gfx/bg4.bmp", display, true, 0, 0, 0);
  Battle[4]= new CGraphic("Gfx/bg5.bmp", display, true, 0, 0, 0);
  Battle[5]= new CGraphic("Gfx/bg6.bmp", display, true, 0, 0, 0);
  Battle[6]= new CGraphic("Gfx/bg7.bmp", display, true, 0, 0, 0);

  Shop= new CGraphic("Gfx/barter2.bmp", display, true, 0, 0, 0);
  JournalFace= new CGraphic("Gfx/journal.bmp", display, true, 0, 0, 0);
  Border= new CGraphic("Gfx/border.bmp", display, true, 0, 0, 0);

  Windows= new CGraphic("Gfx/windows.bmp", display, true, 0, 0, 0);
  Font = new CGraphic("Gfx/font.bmp", display, true, 0, 0, 0);
  WorldMap= new CGraphic("Gfx/map.bmp", display, true, 0, 0, 0);
  Frame= new CGraphic("Gfx/loadsave.bmp", display, true, 0, 0, 0);
  Cursor= new CGraphic("Gfx/cursors.bmp", display, true, 0, 0, 0);

  CutTiles();
  return true;
}

/*
bool cDA2Gfx::RestoreAll(){
	if(ddObj->dd->RestoreAllSurfaces()!=DD_OK) return false;
	else LoadGFX();
	return true;
}

int cDA2Gfx::LoadBitmapResource(LPDIRECTDRAWSURFACE7 lpdds, int xDest, int yDest, char *fn) {
  HDC hSrcDC;           // source DC - memory device context
  HDC hDestDC;          // destination DC - surface device context
  HBITMAP hbitmap;      // handle to the bitmap resource
  BITMAP bmp;           // structure for bitmap info
  int nHeight, nWidth;  // bitmap dimensions

  // first load the bitmap resource
  if ((hbitmap = (HBITMAP)LoadImage(NULL, fn,IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE)) == NULL) return(FALSE);

  // create a DC for the bitmap to use
  if ((hSrcDC = CreateCompatibleDC(NULL)) == NULL) return(FALSE);

  // select the bitmap into the DC
  if (SelectObject(hSrcDC, hbitmap) == NULL){
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  // get image dimensions
  if (GetObject(hbitmap, sizeof(BITMAP), &bmp) == 0) {
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  nWidth = bmp.bmWidth;
  nHeight = bmp.bmHeight;

  // retrieve surface DC
  if (FAILED(lpdds->GetDC(&hDestDC))) {
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  // copy image from one DC to the other
  if (BitBlt(hDestDC, xDest, yDest, nWidth, nHeight, hSrcDC, 0, 0, SRCCOPY) == NULL) {
    lpdds->ReleaseDC(hDestDC);
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  // kill the device contexts
  lpdds->ReleaseDC(hDestDC);
  DeleteDC(hSrcDC);

  // return success
  return(TRUE);
}
*/