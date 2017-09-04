#ifndef _DA2LINE_H
#define _DA2LINE_H

#include "DA2DX.h"

class cLine{
public:
	cLine();
	~cLine();

	bool Init(CDisplay *d);

	void DrawLine(int x1, int y1, int x2, int y2, char color);

protected:
private:
	CDisplay *display;
};

#endif
