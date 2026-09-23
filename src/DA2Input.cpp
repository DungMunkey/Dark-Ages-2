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
    if(e.type == SDL_EVENT_KEY_DOWN) setKey(e.key.key, true);
    else if(e.type == SDL_EVENT_KEY_UP) setKey(e.key.key, false);
    else if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) setButton(e.button.button, true);
    else if(e.type == SDL_EVENT_MOUSE_BUTTON_UP) setButton(e.button.button, false);
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
      //printf("Button 1 release\n");
      mouseLocked[0]=false;
      lastButton=1;
    }
    break;
  case SDL_BUTTON_RIGHT:
    buttonState[1]=b;
    if(!b) {
      //printf("Button 2 release\n");
      mouseLocked[1]=false;
      lastButton=2;
    }
    break;
  case SDL_BUTTON_MIDDLE:
    buttonState[2]=b;
    if(!b) {
      //printf("Button 3 release\n");
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
  case SDLK_A:      x=KEY_A; break;
  case SDLK_B:      x=KEY_B; break;
  case SDLK_C:      x=KEY_C; break;
  case SDLK_D:      x=KEY_D; break;
  case SDLK_E:      x=KEY_E; break;
  case SDLK_F:      x=KEY_F; break;
  case SDLK_G:      x=KEY_G; break;
  case SDLK_H:      x=KEY_H; break;
  case SDLK_I:      x=KEY_I; break;
  case SDLK_J:      x=KEY_J; break;
  case SDLK_K:      x=KEY_K; break;
  case SDLK_L:      x=KEY_L; break;
  case SDLK_M:      x=KEY_M; break;
  case SDLK_N:      x=KEY_N; break;
  case SDLK_O:      x=KEY_O; break;
  case SDLK_P:      x=KEY_P; break;
  case SDLK_Q:      x=KEY_Q; break;
  case SDLK_R:      x=KEY_R; break;
  case SDLK_S:      x=KEY_S; break;
  case SDLK_T:      x=KEY_T; break;
  case SDLK_U:      x=KEY_U; break;
  case SDLK_V:      x=KEY_V; break;
  case SDLK_W:      x=KEY_W; break;
  case SDLK_X:      x=KEY_X; break;
  case SDLK_Y:      x=KEY_Y; break;
  case SDLK_Z:      x=KEY_Z; break;
  case SDLK_SPACE:  x=KEY_SPACE; break;
  case SDLK_BACKSPACE:      x=KEY_BACKSPACE; break;
  case SDLK_ESCAPE: x=KEY_ESCAPE; break;
  case SDLK_TAB:    x=KEY_TAB; break;
  case SDLK_COMMA:      x=KEY_COMMA; break;
  case SDLK_PERIOD:      x=KEY_PERIOD; break;
  case SDLK_UP:     x=KEY_UP; break;
  case SDLK_DOWN:   x=KEY_DOWN; break;
  case SDLK_LEFT:   x=KEY_LEFT; break;
  case SDLK_RIGHT:  x=KEY_RIGHT; break;
  case SDLK_LSHIFT:  x=KEY_LSHIFT; break;
  case SDLK_RSHIFT:  x=KEY_RSHIFT; break;
  case SDLK_KP_PLUS:  x=KEY_ADD; break;
  case SDLK_KP_MINUS:  x=KEY_SUBTRACT; break;
  case SDLK_0:      x=KEY_0; break;
  case SDLK_1:      x=KEY_1; break;
  case SDLK_2:      x=KEY_2; break;
  case SDLK_3:      x=KEY_3; break;
  case SDLK_4:      x=KEY_4; break;
  case SDLK_5:      x=KEY_5; break;
  case SDLK_6:      x=KEY_6; break;
  case SDLK_7:      x=KEY_7; break;
  case SDLK_8:      x=KEY_8; break;
  case SDLK_9:      x=KEY_9; break;
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
