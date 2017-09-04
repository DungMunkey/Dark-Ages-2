#ifndef _DA2PARTY_H
#define _DA2PARTY_H

#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Input.h"
#include "DA2PlayerCharacter.h"
#include "DA2ItemController.h"
#include "DA2Text.h"
#include "DA2Box.h"
#include "DA2Spell.h"
#include "DA2Window.h"

class cDA2Party{
public:

	cDA2Party();
	~cDA2Party();

	bool							BackRow[3];
	cItemController	 *ItemList;
	cPlayerCharacter	Players[3];
	int								PartyItems[40];
	Spell							Spells[30];
	int								SpellList[30];
	int								SpellCount;
	int								Gold;
	int								ActivePlayer;
	int								DropItem;

	SDL_Rect clickList[17];
	SDL_Rect clickSpell[20];

	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cItemController *itm, int *cc);

	bool Render();
	void ItemWinRender(int x, int y, int item);
	int Logic();

	bool AddItem(int item);
	bool CheckItem(int item);
	bool RemoveItem(int item);
	bool SwapItem(int itemOld, int itemNew);

	void CalcAttributes();

protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
  int *colorCounter;
	cText text;
	cBox box;
	cDA2Window window;

	int packOffset;
	int Selection;
	int TakeOffItem;
	int Highlight;
	int listOffset;
	bool bShowSpellList;

	void SpellListLogic();

};

#endif