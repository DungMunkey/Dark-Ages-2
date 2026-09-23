#include "DA2DX.h"
#include <iostream>

CDisplay::CDisplay(){

  screenHeight = 480; // 1050;
  screenWidth =  640; // 1680;
  currentScreenMode=0;
  renderer=NULL;
  window = NULL;

  txtColors[0].r=255;
  txtColors[0].g=255;
  txtColors[0].b=255;
  txtColors[0].a=255;

  FILE* f=fopen("Gfx/da2pal.txt", "rt");
  for(int i=0; i<256; i++){
    fscanf(f, "%d %d %d\n", &palette[i].r, &palette[i].g, &palette[i].b);
    palette[i].a=255;
  }
  fclose(f);

}

CDisplay::~CDisplay(){
  if(renderer != NULL) SDL_DestroyRenderer(renderer);
  renderer=NULL;
  if(window != NULL) SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();
}

bool CDisplay::init(sConf& conf) {
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)){
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else	{

    int displayCount = 0;
    SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);
    SDL_DisplayID primaryDisplay = (displays != NULL && displayCount > 0) ? displays[0] : 0;

    const SDL_DisplayMode* current = SDL_GetDesktopDisplayMode(primaryDisplay);
    SDL_PixelFormat pFormat = current ? current->format : SDL_PIXELFORMAT_UNKNOWN;
    float rRate = current ? current->refresh_rate : 0.0f;
    sDAVidMode vm;
    char str[32];

    int modeCount = 0;
    SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(primaryDisplay, &modeCount);
    for(int i=modeCount - 1; i >= 0; i--){
      SDL_DisplayMode* mode = modes[i];
      if(mode->format != pFormat || mode->refresh_rate != rRate) continue;
      vm.h=mode->h;
      vm.w=mode->w;
      sprintf(str, "%dx%d", mode->w, mode->h);
      vm.name=str;
      if(conf.w == vm.w && conf.h == vm.h){
        currentScreenMode=screenModes.size();
      }
      screenModes.push_back(vm);
      //printf("%d\t%i x %i, %f hz\n", i, mode->w, mode->h, mode->refresh_rate);
    }
    SDL_free(modes);

    //Some displays (e.g. a single windowed-only mode) report no fullscreen modes at all; fall back to the desktop's own resolution.
    if(screenModes.empty() && current){
      vm.w=current->w;
      vm.h=current->h;
      sprintf(str, "%dx%d", vm.w, vm.h);
      vm.name=str;
      screenModes.push_back(vm);
      currentScreenMode=0;
    }
    SDL_free(displays);

    conf.w=screenModes[currentScreenMode].w;
    conf.h=screenModes[currentScreenMode].h;

    //for(int i=0; i < screenModes.size();i++){
    //  printf("%s\n", &screenModes[i].name[0]);
    //}
    screenWidth=screenModes[currentScreenMode].w;
    screenHeight=screenModes[currentScreenMode].h;
    SDL_WindowFlags wf = conf.fullScreen ? SDL_WINDOW_FULLSCREEN : 0;

    //Nearest-neighbor texture filtering (SDL3 dropped the global render-scale-quality hint in
    //favor of a per-texture scale mode - see CGraphic::loadTexture). Pixels rule!!!

    //Create window
    window = SDL_CreateWindow("Dark Ages II: Engel", screenWidth, screenHeight, wf);
    if(window == NULL)	{
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else	{
      //SDL3 doesn't allow a window to have both a surface (SDL_GetWindowSurface) and a renderer at the
      //same time, so get the pixel format straight from the window instead of via its surface.
      pixelFormat = SDL_GetWindowPixelFormat(window);

      renderer = SDL_CreateRenderer(window, NULL);
      if(renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      } else {
        //Initialize renderer color
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      }

    }
  }

  return success;
}

void CDisplay::resize(size_t screenMode){
  if(screenMode >= screenModes.size()) return;
  currentScreenMode=screenMode;
  screenWidth=screenModes[currentScreenMode].w;
  screenHeight=screenModes[currentScreenMode].h;
  SDL_SetWindowSize(window, screenWidth, screenHeight);
}
