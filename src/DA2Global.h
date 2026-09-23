#ifndef _DA2GLOBAL_H
#define _DA2GLOBAL_H

typedef struct sPlayer{
	int imageOffset;
	int dir;  //0=Down, 1=Right, 2=Up, 3=Left
	int frame;
	int X;
	int Y;
	int Map;
	bool Moving;
} sPlayer;

#endif