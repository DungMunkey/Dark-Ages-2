#include "DA2Input.h"

cDA2Input::cDA2Input(){
  mousePosX=0;
  mousePosY=0;
  offsetPosX=1;
  offsetPosY=1;
  int i;
  for(i=0; i < 128; i++) {
    keyState[i]=false;
    keyLocked[i]=false;
  }
  for(i=0; i<3; i++) buttonState[i]=false;
  mouseLocked[0]=mouseLocked[1]=false;
  lastButton=0;
}

cDA2Input::~cDA2Input(){
}

bool cDA2Input::CheckMousePos(SDL_Rect& r){
  if(mouseX >= r.x && mouseX <= r.x + r.w &&
     mouseY >= r.y && mouseY <= r.y + r.h){
    return true;
  }
  return false;
}

bool cDA2Input::CheckLock(DA2KEYS key){
	return keyLocked[key];
}

bool cDA2Input::KeyPress(DA2KEYS key){
	return keyState[key];
}

void cDA2Input::LockKey(DA2KEYS key){
	//key will not function until released;
	keyLocked[key]=1;
}

void cDA2Input::pollEvents(){
  //printf("Polling\n");
  while(SDL_PollEvent(&e) != 0)	{
    if(e.type == SDL_KEYDOWN) setKey(e.key.keysym.sym, true);
    else if(e.type == SDL_KEYUP) setKey(e.key.keysym.sym, false);
    else if(e.type == SDL_MOUSEBUTTONDOWN) setButton(e.button.button, true);
    else if(e.type == SDL_MOUSEBUTTONUP) setButton(e.button.button, false);
  }
  SDL_GetMouseState(&mousePosX, &mousePosY);
  mouseX=(int)(mousePosX / multPosX) - offsetPosX;
  mouseY=(int)(mousePosY / multPosY) - offsetPosY;
}

bool cDA2Input::getButtonState(int k){
  return buttonState[k];
}

bool cDA2Input::getKeyState(int k){
  return keyState[k];
}

bool cDA2Input::isPressed(int k){
  if(keyState[k]) return true;
  else return false;
}

bool cDA2Input::isReleased(int k){
  if(keyState[k]) return false;
  else return true;
}

bool cDA2Input::CheckLockMouse(int button){
	return mouseLocked[button];
}

void cDA2Input::LockMouse(int button){
	//button will not function until released;
	mouseLocked[button]=true;
}

bool cDA2Input::MousePress(int button){
	return buttonState[button];
}

int cDA2Input::MouseX(){
  return mouseX;
}

int cDA2Input::MouseY(){
  return mouseY;
}

void cDA2Input::setButton(Uint8 k, bool b){
  switch(k){
  case SDL_BUTTON_LEFT:
    buttonState[0]=b;
    if(!b) {
      printf("Button 1 release\n");
      mouseLocked[0]=false;
      lastButton=1;
    }
    break;
  case SDL_BUTTON_RIGHT:
    buttonState[1]=b;
    if(!b) {
      printf("Button 2 release\n");
      mouseLocked[1]=false;
      lastButton=2;
    }
    break;
  case SDL_BUTTON_MIDDLE:
    buttonState[2]=b;
    if(!b) {
      printf("Button 3 release\n");
      lastButton=3;
    }
    break;
  default:
    break;
  }
}

void cDA2Input::setKey(SDL_Keycode k, bool b){
  DA2KEYS x;
  switch(k){
  case SDLK_a:      x=KEY_A; break;
  case SDLK_b:      x=KEY_B; break;
  case SDLK_c:      x=KEY_C; break;
  case SDLK_g:      x=KEY_G; break;
  case SDLK_j:      x=KEY_J; break;
  case SDLK_l:      x=KEY_L; break;
  case SDLK_q:      x=KEY_Q; break;
  case SDLK_s:      x=KEY_S; break;
  case SDLK_z:      x=KEY_Z; break;
  case SDLK_SPACE:  x=KEY_SPACE; break;
  case SDLK_ESCAPE: x=KEY_ESCAPE; break;
  case SDLK_TAB:    x=KEY_TAB; break;
  case SDLK_UP:     x=KEY_UP; break;
  case SDLK_DOWN:   x=KEY_DOWN; break;
  case SDLK_LEFT:   x=KEY_LEFT; break;
  case SDLK_RIGHT:  x=KEY_RIGHT; break;
  default: return;
  }
  keyState[x]=b;
  if(!b)  keyLocked[x]=false;
}

void cDA2Input::setMouseOffsets(float mx, int ox, float my, int oy){
  multPosX=mx;
  multPosY=my;
  offsetPosX=ox;
  offsetPosY=oy;
}
