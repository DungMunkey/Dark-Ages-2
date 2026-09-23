#ifndef _DA2SHOP_H
#define _DA2SHOP_H

#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Input.h"
#include "DA2Party.h"
#include "DA2Box.h"
#include "DA2ItemController.h"
#include "DA2Window.h"
#include <vector>

using namespace std;

class cDA2Shop{
public:

	cDA2Shop();
	~cDA2Shop();

	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, cDA2Party *par, cItemController *itm, int *col);
	void Clear();
	void SetType(int type);
	void SetItem(int item);

	void RenderItemInfo(int item);
	void RenderStats(int item);
	bool Render();
	bool Logic();

protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cDA2Party *Party;
	cItemController *ItemList;
	cText text;
	cBox box;
	cDA2Window statsWin;

	SDL_Rect clickList[15];
	Uint32 clickTime;

	int Selection;
	bool BuyOrSell;
	bool bStatsWin;
	int Buy[10];
	int ShopType;
	int BuyOffset;
	int SellOffset;
	vector<int> Sell;

  int* colorCounter;

	int SellCount;
	int BuyCount;
	int PartyItemCount;
};

#endif