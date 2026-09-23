#ifndef _STACKMANAGER_H
#define _STACKMANAGER_H

#include <cstdlib>
//#include <windows.h>

enum GameStack{
	NoGameStack,
	GameRender,
	TextWindow,
	PreTextWindow,
	StatScreen,
	ShopScreen,
	JournalScreen,
	SaveDialog,
	LoadDialog,
	OptionsScreen,
	InnDialog,
	BasicDialog,
	SpellDialog,
	QuitDialog,
	TrainDialog,
	CastDialog,
	BattleChoiceDialog,
	SurpriseAttackDialog,
	ScriptBattle,
	ChangeTheMap,
	GameMenu,
	CutSceneA,
	CutSceneB,
	IntroScreen,
	BattleMain,
	BattleAnim,
	BattleSpell,
	BattleSkill,
	BattleWon,
	BattleLost,
	BattleDialog,
	WaitOnMouse,
	WaitOnKeyboard
};

class cStackManager {
	typedef struct sState{
		GameStack gProcess;
		sState *Next;
		sState *Prev;
		
		sState(){
      Next = NULL;
			Prev = NULL;
    }

    ~sState() {
      Next = NULL;
			Prev = NULL;
    }

	} sState;

protected:
	sState *StateParent;
	sState *First;
	sState *Last;

public:
	cStackManager();
	~cStackManager();

	GameStack PeekNext();
	void Push(GameStack g);
	bool Pop();
	GameStack Process();
	bool SetStateFirst();
	bool SetStateLast();
	bool SetStateNext();
	bool SetStatePrev();
};

#endif