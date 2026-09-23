#ifndef _DA2_BATTLEENGINE_H
#define _DA2_BATTLEENGINE_H

#include "DA2Box.h"
#include "DA2Input.h"
#include "DA2ItemController.h"
#include "DA2Party.h"
#include "DA2Text.h"
#include "DA2TextWin.h"
#include "DA2DialogWin.h"
#include "StackManager.h"
#include <cmath>
#include <ctime>

typedef struct BattleObject{
	//holds battle stats
	bool Alive;
	float ActionCounter;
	float Awareness;

	int Agility;
	int Intelligence;
	int Speed;
	int Strength;

	int ModAgl;
	int ModInt;
	int ModSpd;
	int ModStr;

	int HP;
	int MaxHP;
	int MP;
	int MaxMP;
	int Encumbrance;
	int Armor;
	int WeaponBal;
	int WeaponStr;
	bool Ranged;
	int ModArmor;
	int SAP;

	bool BackRow;
	int Level;
	int Experience;
	int Gold;
	int Picture;
	int XPos;
	int YPos;

	char Name[24];
} BattleObject;

typedef struct MonsterStat{
	int Str;
	int Agl;
	int Spd;
	int HP;
	int Armor;
	int Experience;
	int Gold;
	int Level;
	char Name[32];
} MonsterStat;

typedef struct BattleScript{
	int ArenaPrice;
	int MonsterID[7];
	int BGID;
	int SongID;
	int Flee;
	int IsRandom;
	int Fixed;
	int Variable;
} BattleScript;

typedef struct BattleTarget{
	int graphic;
	int pawn;
	int effect;
} BattleTarget;

typedef struct BattleAnimStruct{
	BattleTarget damage[6];
	int targetCount;
	char Text[8];
} BattleAnimStruct;

enum BattleMode{
	EnemySelect=0,
	PlayerSelect,
	EnemyRowSelect,
	PlayerRowSelect,
	AllSelect
};

class cDA2BattleEngine{
public:
	int pawnCount;
	BattleObject pawns[9];  //0,1,2 are players, all else are monsters
	vector<BattleAnimStruct> *action;
	
	cDA2BattleEngine();
	~cDA2BattleEngine();

	int GoBattle();
	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cDA2Party *par, cItemController *itm);
	bool Load(int BattleNum);
	int Logic();
	bool Render(bool mouse);
	
protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cDA2Party *party;
	cItemController *ItemList;
	cStackManager battleStack;
	cText text;
	cTextWin textWin;
	cBox box;
	cDialogWin dialog;

	MonsterStat Monsters[128];
	BattleScript Scripts[200];

  SDL_Texture* canvas;

	SDL_Rect SingleMonster[6];
	SDL_Rect RowMonster[2];
	SDL_Rect SinglePlayer[3];
	SDL_Rect RowPlayer;
	SDL_Rect AllMonster;
	SDL_Rect ActionButton[6];

	char skills[3][6][20];
	int skillTargets[3][6];
	int skillPawn;

  int colorCounter;
  Uint32 battleTicks;
  Uint32 lastTicks;

	int BGNum;
	int BattleActor;
	Uint32 BattleAnimTimer;

	void BattleOver(int won);	
	bool BattleAnimation();
	void BumpAwareness(int pawnID);
	void CalcRewards(bool fled=false);
	void ClearStack();
	void CheckDeath();
	bool GetNextCharacter(int &pawnID);
  void GShot();
	bool MonsterAttack(int monster, int &target, int &damage);
	bool MonsterLoad();
	bool PlayerAttack(int player, int monster, int &damage);
	bool PlayerSpell(int spell, int &effect, int &graphic, int target, int& stat);

	bool LevelUp[3];

  bool checkMouse(SDL_Rect& r);

	int HPotion;
	int MPotion;
	int LastAlive;
	int pXP[3];
	int goldWon;
	int ListOffset;
	int ListSelection;
	bool CanRun;
	bool SpellRender();
	int SpellLogic();
	bool SkillRender();
	int SkillLogic();

};


#endif