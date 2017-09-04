#ifndef _DA2LOADSAVE_H
#define _DA2LOADSAVE_H

#include "DA2Global.h"
#include "DA2DX.h"
#include "DA2Input.h"
#include "DA2Gfx.h"
#include "DA2Text.h"
#include "DA2Party.h"
#include "DA2Map.h"
#include "DA2Journal.h"
#include "DA2Options.h"
#include <io.h>
#include <vector>

using namespace std;

typedef struct SaveInfo{
	char descript[41];
	char timeStr[9];
	char dateStr[9];
	int ID;
	int playTime;
	int HP[3][2];
	int MP[2];
} SaveInfo;

class cDA2LoadSave{
public:
	cDA2LoadSave();
	~cDA2LoadSave();

	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, sPlayer *play, cDA2Party *par, cMap *map, short *flag, int *anim, cDA2Journal *jour, int *speed, cDA2Options* opt);
	void ReadDir();

	int Logic(bool bLoad);
	bool Render(bool bLoad);
protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cDA2Party *party;
	cDA2Options *options;
	sPlayer *player;
	cMap *maps;
	short *flags;
	int *animCounter;
	cDA2Journal *journal;
	int *gameSpeed;
	cText text;
	cBox box;
	
	//LPDIRECTDRAWSURFACE7 Frame;

	time_t loadTime,saveTime;
	int playTime;

	vector<SaveInfo> vFiles;
	bool bQS;
	bool bQL;
	SaveInfo QuickSave;
	SaveInfo QuickLoad;
	SaveInfo CurrentInfo;
	int Selection;
	int Offset;
	int Cursor;

	char DIKtoASCII(int dik);
	bool ReadHeader(char *file, SaveInfo &sav);
	bool SaveGame(int index);
	bool LoadGame(int index);


	SDL_Rect clickList[12];
};

#endif