#include "DA2Map.h"

cMap::cMap(){
	NPCCount=0;
	ObjArray = new vector<ObjectType>;
}

cMap::~cMap(){
	if(NPCCount>0) delete NPCArray;
	delete ObjArray;
}

void cMap::Init(int npc){
	NPCCount=npc;
	if(NPCCount) NPCArray = new NPCType[NPCCount];
}

void cMap::ResetNPC(){
	int i;
	for(i=0;i<NPCCount;i++){
		//reset coordinates to min?
	}
}
