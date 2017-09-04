#ifndef _DA2MAP_H
#define _DA2MAP_H

#include <vector>
using namespace std;

typedef struct NPCType {
	short InMotion;
	short MapXLoc;
  short MapYLoc;
  short Movable;
  short PicNum;
	short PicFrame;
	short PicType;
	short XMax;
	short XMin;
	short xmove;
	short xoffset;
	short YMax;
	short YMin;
	short ymove;
	short yoffset;
	short NStatus;
} NPCType;

typedef struct ObjectType {
	short Xco;
	short Yco;
	short Status;
	short ObjectPic;
	short Solid;
	short HideTab;
} ObjectType;

class cMap{
public:
	short MapArray[96][96][4];
	short MapKey[256];
	int BattleNum;
	NPCType *NPCArray;
	vector<ObjectType> *ObjArray;
	//NPCType NPCArray[75];
	//ObjectType ObjArray[100];

	int NPCCount;

	cMap();
	~cMap();
	void Init(int npc);
	void ResetNPC();
};

#endif 
