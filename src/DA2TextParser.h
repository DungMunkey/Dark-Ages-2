#ifndef _DA2TEXTPARSER_H
#define _DA2TEXTPARSER_H

#include <cstdio>
#include <cstring>
#include <string>

using namespace std;

class cTextParser {
public:
	void parseText(char *in, char *out);
	void setNames(char *n1, char *n2, char *n3);
protected:
private:
	string Names[3];

};

#endif