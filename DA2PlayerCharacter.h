#ifndef _DA2PLAYERCHARACTER_H
#define _DA2PLAYERCHARACTER_H

#include "DA2Character.h"

typedef struct sPlayerItems{
	int Weapon;
  int Shield;
  int Armor;
  int Helmet;
  int Boots;
  int Hands;
  int Ring;
  int Legs;
} sPlayerItems;

class cPlayerCharacter:public cCharacter{
public:
	sPlayerItems Gear;
protected:
private:
};

#endif