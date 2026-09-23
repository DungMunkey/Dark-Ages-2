#ifndef _DA2JOURNAL_H
#define _DA2JOURNAL_H

#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Input.h"
#include "DA2Box.h"
#include "DA2Line.h"
#include "DA2Text.h"
#include <vector>

/*
Private ddsdMap As DDSURFACEDESC2
Private ddsdJournal As DDSURFACEDESC2
Private MSurfMap As DirectDrawSurface7
Private MSurfJournal As DirectDrawSurface7
Private MRectMap As RECT
Private MRectJournal As RECT
Private JFontInfo As StdFont

Private JXOffset As Integer     'Offset of where map is scrolled to relative to player
Private JYOffset As Integer

Private TYOffset As Integer     'Offset text of journal - just on the Y-axis
Private TMaxY As Integer

Private JPX As Integer  'PlayerX, adjusted for journal map
Private JPY As Integer  'PlayerY, adjusted for journal map

Private NoPlayerLocation As Boolean     'Hide the player location circle if not on the map.
Private DoneWithJournal As Boolean      'Flags when player clicks exit button

Private JBlitPortal As Boolean

Private Type DifficultyType
    Diff As String
    Color As Byte
End Type
*/

typedef struct PointType {
    short X;
    short Y;
} PointType;

typedef struct JournalType {
    short ID;
    short Difficulty;
    short MainQuest;
    PointType EndPoint;
    short Active;
    string Title;
		short TextCount;
		vector<string>* Text;
		JournalType(){
			Text=new vector<string>;
		}
		JournalType(const JournalType& j){
			ID=j.ID;
			Difficulty=j.Difficulty;
			MainQuest=j.MainQuest;
			EndPoint=j.EndPoint;
			Active=j.Active;
			Title=j.Title;
			TextCount=j.TextCount;
			Text = new vector<string>;
			for(unsigned int i=0;i<j.Text->size();i++) Text->push_back(j.Text->at(i));
		}
		~JournalType(){
			if(Text!=NULL) delete Text;
		}
		JournalType& operator=(const JournalType& j){
			if(this!=&j){
				ID=j.ID;
				Difficulty=j.Difficulty;
				MainQuest=j.MainQuest;
				EndPoint=j.EndPoint;
				Active=j.Active;
				Title=j.Title;
				TextCount=j.TextCount;
				delete Text;
				Text = new vector<string>;
				for(unsigned int i=0;i<j.Text->size();i++) Text->push_back(j.Text->at(i));
			}
			return *this;
		}

} JournalType;

class cDA2Journal{
public:
	cDA2Journal();
	~cDA2Journal();

	bool Init(CDisplay *d, cDA2Gfx *gfx, cDA2Input *inp, short *flg);
	int Logic();
	bool Render();

	bool LoadEntries();
	void Reset(); //sets quest to main and active
	void SetPlayerPos(int map, int x, int y);

	int GetID(int ID);

	//Modifiers
	void SetID(int index, short value);
	void SetDifficulty(int index, short value);
	void SetMainQuest(int index, short value);
	void SetEndPoint(int index, short x, short y);
	void SetActive(int index, short value);
	void SetTitle(int index, char *str);
	void AddText(int index, char *str);
	
	JournalType JournalText[100];

protected:
private:
	CDisplay *display;
	cDA2Gfx *ddGfx;
	cDA2Input *diObj;
	cBox box;
	cText text;
	cLine line;

	short *flags;

	int PlayerX;
	int PlayerY;
	int JournalX;
	int JournalY;
	int TextOffset;
	int MaxTextOffset;
	int curEntry;
	bool bMain;
	bool bComplete;
	bool bPortals;
	bool bQuests;
	bool bTowns;
	bool bKeeps;

	SDL_Rect clickList[21];
	
	bool LoadJournal();
	vector<int> journalIndex;

	void SortJournal();
	void DrawPortals();
	
};

#endif