#ifndef _DA2GFX_H
#define _DA2GFX_H

#include <vector>
#include "DA2Graphic.h"

#if _MSC_VER > 1020	      // if VC++ version is > 4.2					 
	using namespace std;    // std c++ libs implemented in std	 
#endif;

typedef struct TileType {
	SDL_Rect r;
  int index;
  char solid;
} TileType;

class cDA2Gfx{
public:
  cDA2Gfx();
  ~cDA2Gfx();

  CGraphic* Tiles[8];
  CGraphic* Chars[4];
  CGraphic* Monsters[4];
  CGraphic* Objects[2];
  CGraphic* Stats[3];
  CGraphic* Battle[7];
  CGraphic* Windows;
  CGraphic* Shop;
  CGraphic* Border;
  CGraphic* Font;
  CGraphic* Cursor;

	//For load/save
  CGraphic* Frame;

	//For journal stuff:
  CGraphic* WorldMap;
  CGraphic* JournalFace;
  CGraphic* TextSurface;
	//LPDIRECTDRAWCLIPPER TextClip;
	//LPDIRECTDRAWPALETTE ddPal;

	vector<TileType> *vTile;
	vector<TileType> *vNPC;
	vector<TileType> *vObj;
	vector<TileType> *vMonster;
	SDL_Rect aWindows[33];
	SDL_Rect aFont[8][128];
	SDL_Rect aCursor[9];
	SDL_Rect aBattleStatus[9];
	int CursorX[9];  //offset for cursor hotspot
	int CursorY[9];  //offset for cursor hotspot
	
	//bool Init(cDX *dd7);

	//void CreateSurfaces();
	void CutTiles();
	//int LoadBitmapResource(LPDIRECTDRAWSURFACE7 lpdds, int xDest, int yDest, char *fn);
	//void LoadGFX();
  bool LoadGfx(CDisplay* display);
	//bool RestoreAll();

private:
	//cDX *ddObj;

};

#endif