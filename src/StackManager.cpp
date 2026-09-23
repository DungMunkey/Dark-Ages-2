#include "StackManager.h"

cStackManager::cStackManager(){
	StateParent=NULL;
	First=NULL;
	Last=NULL;
}

cStackManager::~cStackManager(){
	sState *StatePtr;
	while((StatePtr=Last)!=NULL){
		Last=Last->Prev;
		delete StatePtr;
	}
}

void cStackManager::Push(GameStack g) {
	sState *StatePtr = new sState;
	StatePtr->gProcess=g;

	if(First==NULL){
		StatePtr->Prev=NULL;
		StatePtr->Next=NULL;
		First=StatePtr;
	} else {
		Last->Next = StatePtr;
		StatePtr->Prev = Last;
	}
	Last=StatePtr;
	StateParent = Last;
}

bool cStackManager::Pop(){
	sState *StatePtr = Last;

	if(StatePtr!=NULL){
		Last=Last->Prev;
		if(Last!=NULL) Last->Next=NULL;
		delete StatePtr;
	}

	StateParent=Last;

	if(Last==NULL){
		First=NULL;
		return false;
	} else {
		return true;
	}
}

GameStack cStackManager::PeekNext(){
	if(StateParent==NULL) return NoGameStack;
	if(StateParent->Next==NULL) return NoGameStack;
	return StateParent->Next->gProcess;
}

GameStack cStackManager::Process(){
	if(StateParent==NULL) return NoGameStack;
	else return StateParent->gProcess;
}

bool cStackManager::SetStateFirst(){
	StateParent=First;
	if(StateParent==NULL) return false;
	return true;
}

bool cStackManager::SetStateLast(){
	StateParent=Last;
	if(Last==NULL) return false;
	return true;
}

bool cStackManager::SetStateNext(){
	StateParent=StateParent->Next;
	if(StateParent==NULL) return false;
	return true;
}

bool cStackManager::SetStatePrev(){
	StateParent=StateParent->Prev;
	if(StateParent==NULL) return false;
	return true;
}