#ifndef _DA2GAME_H
#define _DA2GAME_H

#include "DA2Global.h"
#include "DA2Map.h"
#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Input.h"
#include "DA2Party.h"
#include "DA2Shop.h"
#include "DA2Text.h"
#include "DA2TextWin.h"
#include "DA2TextParser.h"
#include "DA2Box.h"
#include "DA2DialogWin.h"
#include "DA2ListWin.h"
#include "DA2TrainWin.h"
#include "DA2SpellWin.h"
#include "DA2Journal.h"
#include "DA2LoadSave.h"
#include "DA2BattleEngine.h"
#include "DA2Cutscene.h"
#include "DA2Music.h"
#include "DA2Options.h"
#include "DA2Intro.h"
#include "Camera.h"
#include "StackManager.h"
//#include <windows.h>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

enum EventTag{
	Failure=0,
	Normal,
	Barter,
	BattleTag,
	BattleNow,
	Inn,
	Train,
	Spell
};

typedef struct DeadScript{
	int map;
	int num;
	bool npc;
} DeadScript;

typedef struct MapChanger{
	int map;
	int x;
	int y;
} MapChanger;

typedef struct BitMapHeader{
   WORD    bfType;        // must be 'BM' 
   DWORD   bfSize;        // size of the whole .bmp file
   WORD    bfReserved1;   // must be 0
   WORD    bfReserved2;   // must be 0
   DWORD   bfOffBits;     
} BitMapHeader; 

typedef struct BitMapInfoHeader {
   DWORD  biSize;            // size of the structure
   LONG   biWidth;           // image width
   LONG   biHeight;          // image height
   WORD   biPlanes;          // bitplanes
   WORD   biBitCount;        // resolution 
   DWORD  biCompression;     // compression
   DWORD  biSizeImage;       // size of the image
   LONG   biXPelsPerMeter;   // pixels per meter X
   LONG   biYPelsPerMeter;   // pixels per meter Y
   DWORD  biClrUsed;         // colors used
   DWORD  biClrImportant;    // important colors
} BitMapInfoHeader;
                

class cDA2Game {
public:
  CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cDA2Music *MusicObj;
	cItemController *ItemList;
	cText text;
	cTextWin textWindow;
	cTextParser textParser;
	cBox box;
	cDialogWin dialog;
	cListWin listDialog;
	cTrainWin trainDialog;
	cSpellWin castDialog;
	cStackManager mainStack;
	cDA2LoadSave loadSave;
	cDA2BattleEngine battleEngine;
	DA2Cutscene cutScene;

	cDA2Options options;
	cDA2Intro intro;

	cDA2Shop shop;
	cDA2Journal journal;

	sPlayer Player;
	cDA2Party Party;

  sConf* conf;

	short Flags[300];
	vector<string> vText;

	cCameraController PlayerCam;
	cMap Maps[500];
	int MapWin[3][3];
	int Cursor[3][3];
	char RoofMask[96][96];
	int RoofX,RoofY;
	int RoofTileX,RoofTileY;
	bool RoofOff;

	int GoGame();
	bool CheckCollide(int dir);
	bool CheckRender();
  void GShot();
	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cItemController *itm, cDA2Music *mus, sConf *con);
	bool NPCClick(int atX, int atY, int &MapNum, int &NPCNum);
	void NPCLogic();
	void NPCRender();
	int ObjClick(int atX, int atY, int &MapNum, int &ObjNum);
  void ObjRender(SDL_Renderer *surf, bool bSolid=false);
  void ObjRender(SDL_Surface *surf, bool bSolid=false);
	bool Render();
	bool Logic();
	void LoadMaps();
	void NewGame();

	void ChangeMapWalking(int xdir, int ydir);
	void ChangeMap(int map, int xpos, int ypos);
	void CheckBattle(double chance);

	void SetRoofMask();
	void FlipRoof(int X, int Y);

  int colorCounter;
	int animCounter;
	int animator;
	bool bAnimateNow;
	bool bHighlightObject;
	bool bBorder;

	int FPS;
	int FPSCounter;

	//DWORD AnimTimer;
	//DWORD NPCTimer;
	//DWORD FPSTimer;
	//DWORD RenderTimer;

  unsigned int ticks;
  unsigned int lastTicks;

  unsigned int tickAnim;
  unsigned int tickFPS;
  unsigned int tickNPC;
  unsigned int tickX;
  unsigned int tickY;

	__int64 fps64;
	__int64 freq;

	//For script handling
	long NPCFileIndex[500];
	long ObjFileIndex[500];
	EventTag NPCScript(int map, int npc);
	EventTag ObjScript(int map, int obj);

	bool bDebug;

private:
	bool DropItem();

	bool bTrainDialog;

  int postCutScene;
	int preText;
	int SpellCost;
	int SpellNum;
	int InnCost;
	int renderSpeed;
	int renderCounter;
	int playerRender;
	int randBattleCounter;
	
	//int PlayerCounter;
	DWORD PlayerTimer;

  SDL_Texture* canvas;
  SDL_Surface* collide;

	double xProgress;
	double yProgress;
	int xSpeed;
	int ySpeed;

	DWORD msTime;

	DeadScript killMe;
	MapChanger newMap;

};

#endif