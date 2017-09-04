#include "DA2TextParser.h"

void cTextParser::parseText(char *in, char *out){
	int i;
	string tmp;

	tmp="";
	for(i=0;i<strlen(in);i++){
		switch(in[i]){
		case '@':
			//strcat(out,Names[0]);
			tmp+=Names[0];
			break;
		case '$':
			//strcat(out,Names[1]);
			tmp+=Names[1];
			break;
		case '^':
			//strcat(out,Names[2]);
			tmp+=Names[2];
			break;
		default:
			//strcat(out,in[
			tmp+=in[i];
			break;
		}
	}
	strcpy(out,&tmp[0]);
}

void cTextParser::setNames(char *n1, char *n2, char *n3){
	Names[0]=n1;
	Names[1]=n2;
	Names[2]=n3;
}