#ifndef _DA2CUTSCENE_H
#define _DA2CUTSCENE_H

#include "DA2DX.h"
#include "DA2Input.h"
#include "DA2Gfx.h"
#include "DA2Map.h"
#include "DA2Music.h"
#include "DA2TextWin.h"
#include "DA2TextParser.h"
#include "Camera.h"
#include <vector>

typedef struct Act {
	int type;
	int target;
	int posX;
	int posY;
	int pic;
	int speed;
	char text[512];
} Act;

typedef struct Actor {
	int character;
	int direction;
	int frame;
	int posX;
	int posY;
	int destX;
	int destY;
	int speed;
	bool bMoving;
} Actor;

class DA2Cutscene {
public:

	DA2Cutscene();
	~DA2Cutscene();

	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cMap *maps, cDA2Music *mus, int* anim);
	bool LoadScene(int index, char *p1, char *p2, char *p3);

	bool Render();
	void ObjRender();
	void NPCRender();
	bool Logic();

protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
  cDA2Music *music;

	bool bAction;
	bool bBorder;
	bool bPause;
	bool bRoof;
	bool bText;

	int* animCounter;

	int CamX;
	int CamY;
	int CamSpeed;
	cCameraController SceneCam;

	cMap *Maps;
	int MapWin[3][3];

	cTextParser parser;
	cTextWin text;

	int sceneIndex;
	vector<Act> scene;
	Actor npc[20];

  unsigned int lastTicks;

};


#endif