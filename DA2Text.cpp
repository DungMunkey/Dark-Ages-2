#include "DA2Text.h"

cText::cText(){
	ddGfx=NULL;
}

cText::~cText(){
	ddGfx=NULL;
}

bool cText::Init(cDA2Gfx *gfx){

	if((ddGfx=gfx)==NULL) return false;

	return true;
}

/*
void cText::DrawText(int x, int y, int wrap, char *txt){
	int xPos=x;
	int yPos=y;
	int count=0;
	int i;

	char *tok;
	char str[512];

	strcpy(str,txt);

	RECT r;

	tok=strtok(str," ");
	while(tok!=NULL){
		if(wrap>0 && count+strlen(tok)>wrap){
			xPos=x;
			yPos+=16;
			count=0;
		}

		for(i=0;i<strlen(tok);i++){
			if(tok[i]=='\n'){
				xPos=x;
				yPos+=16;
				count=0;
				continue;
			}
			r.left=xPos;
			r.top=yPos;
			r.right=r.left+8;
			r.bottom=r.top+16;
			ddObj->ddsb->Blt(&r, Font, &aFont[tok[i]], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			xPos+=8;
			count++;
		}

		tok=strtok(NULL," ");
		if(tok!=NULL){
			r.left=xPos;
			r.top=yPos;
			r.right=r.left+8;
			r.bottom=r.top+16;
			ddObj->ddsb->Blt(&r, Font, &aFont[32], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			xPos+=8;
			count++;
		}
	}

}
*/

int cText::drawText(SDL_Renderer *surf, int x, int y, int wrap, char *txt, int color){
	int xPos=x;
	int yPos=y;
	int count=0;
	int lineCount=0;
	int i;

	char *tok;
	char str[2048];

	strcpy(str,txt);

	SDL_Rect r;

	if(wrap==0){
		xPos=x;
		yPos=y;
		for(i=0;i<strlen(txt);i++) {
			if(txt[i]=='\n'){
				xPos=x;
				yPos+=16;
				lineCount++;
				continue;
			}
			r.x=xPos;
			r.y=yPos;
			r.w=8;
			r.h=16;
      SDL_RenderCopy(surf, ddGfx->Font->texture, &ddGfx->aFont[color][txt[i]], &r);
			//surf->Blt(&r, ddGfx->Font, &ddGfx->aFont[color][txt[i]], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			xPos+=8;
		}
		return lineCount+1;
	}

	tok=strtok(str," ");
	while(tok!=NULL){
		if(wrap>0 && count+strlen(tok)>wrap){
			xPos=x;
			yPos+=16;
			count=0;
			lineCount++;
		}

		for(i=0;i<strlen(tok);i++){
			if(tok[i]=='\n'){
				xPos=x;
				yPos+=16;
				count=0;
				lineCount++;
				continue;
			}
			r.x=xPos;
			r.y=yPos;
			r.w=8;
			r.h=16;
      SDL_RenderCopy(surf, ddGfx->Font->texture, &ddGfx->aFont[color][tok[i]], &r);
			//surf->Blt(&r, ddGfx->Font, &ddGfx->aFont[color][tok[i]], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			xPos+=8;
			count++;
		}

		tok=strtok(NULL," ");
		if(tok!=NULL){
			r.x=xPos;
			r.y=yPos;
			r.w=8;
			r.h=16;
      SDL_RenderCopy(surf, ddGfx->Font->texture, &ddGfx->aFont[color][32], &r);
			//surf->Blt(&r, ddGfx->Font, &ddGfx->aFont[color][32], DDBLT_WAIT|DDBLT_KEYSRC,NULL);
			xPos+=8;
			count++;
		}
	}

	return lineCount;

}