#ifndef _DA2SPELL_H
#define _DA2SPELL_H

typedef struct Spell{
	bool Known;
	int Type;
	int MPCost;
	char SpellName[20];
} Spell;

#endif