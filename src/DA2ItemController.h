#ifndef _DA2ITEMCONTROLLER_H
#define _DA2ITEMCONTROLLER_H

#include <iostream>

typedef struct sItem{
	short Item;
  short WeaponStr;
  short WeaponBal;
  short ArmorStr;
  short ArmorEncumb;
  short StrMod;
  short AglMod;
  short IntMod;
  short SpeedMod;
  short HPMod;
  short MPMod;
  short user;
  short ItemPic;
  char ItemName[25];
  int value;
  short Ranged;
} sItem;

class cItemController{
public:
	sItem Items[400];

	bool LoadItems();
protected:
private:
};

#endif