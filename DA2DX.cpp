#include "DA2DX.h"

CDisplay::CDisplay(){

  screenHeight = 480; // 1050;
  screenWidth =  640; // 1680;
  currentScreenMode=0;
  renderer=NULL;
  screenSurface = NULL;
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
  if(screenSurface != NULL) SDL_FreeSurface(screenSurface);
  screenSurface = NULL;
  if(window != NULL) SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();
}

bool CDisplay::init(sConf& conf) {
  //Initialization flag
  bool success = true;

  //Initialize SDL
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0){
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else	{

    int display_count = 0, display_index = 0, mode_index = 0;
    SDL_DisplayMode mode ={SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0};

    SDL_GetCurrentDisplayMode(0, &mode);
    Uint32 pFormat=mode.format;
    int rRate=mode.refresh_rate;
    sDAVidMode vm;
    char str[32];

    for(int i=SDL_GetNumDisplayModes(0) - 1; i >= 0; i--){
      if(SDL_GetDisplayMode(0, i, &mode) != 0) {
        SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
      } else {
        if(mode.format != pFormat || mode.refresh_rate != rRate) continue;
        vm.h=mode.h;
        vm.w=mode.w;
        sprintf(str, "%dx%d", mode.w, mode.h);
        vm.name=str;
        if(conf.w == vm.w && conf.h == vm.h){
          currentScreenMode=screenModes.size();
        }
        screenModes.push_back(vm);
        //printf("%d\tSDL_GetDisplayMode(0, 0, &mode):\t\t%i bpp\t%i x %i, %ihz\n", i,SDL_BITSPERPIXEL(mode.format), mode.w, mode.h,mode.refresh_rate);
      }
    }
    conf.w=screenModes[currentScreenMode].w;
    conf.h=screenModes[currentScreenMode].h;

    //for(int i=0; i < screenModes.size();i++){
    //  printf("%s\n", &screenModes[i].name[0]);
    //}
    screenWidth=screenModes[currentScreenMode].w;
    screenHeight=screenModes[currentScreenMode].h;
    int wf;
    if(conf.fullScreen) {
      wf=SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
    } else {
      wf=SDL_WINDOW_SHOWN;
    }

    //Set texture filtering to nearest neighbor. Pixels rule!!!
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
      printf("Warning: nearest neighbor texture filtering not enabled!");
    }

    //Create window
    window = SDL_CreateWindow("Dark Ages II: Engel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, wf);
    if(window == NULL)	{
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else	{
      //Get window surface
      screenSurface = SDL_GetWindowSurface(window);

      renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
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

/*
#include <iostream>

cDX::cDX(){
	dd=NULL;
	ddsp=NULL;
	ddsb=NULL;
	ddsc=NULL;
	ddPal=NULL;
	ddClip=NULL;
	ddClipCollide=NULL;
	rotation=0;
}

cDX::~cDX(){
	if (ddPal != NULL) {
		ddPal->Release();
		ddPal = NULL;
  }
	if(ddClip){
		ddClip->Release();
		ddClip = NULL;
	}
	if(ddClipCollide){
		ddClipCollide->Release();
		ddClipCollide = NULL;
	}
	if(ddsb) {
		ddsb->Release();
		ddsb = NULL;
	}
	if(ddsp) {
		ddsp->Release();
		ddsp = NULL;
	}	
	if(ddsc) {
		ddsc->Release();
		ddsc = NULL;
	}
	if (dd) {
		dd->Release();
		dd = NULL;
	}
}

bool cDX::ChangeDisplayMode(int colorDepth){
	if (ddPal != NULL) {
		ddPal->Release();
		ddPal = NULL;
  }
	if(ddClip){
		ddClip->Release();
		ddClip = NULL;
	}
	if(ddClipCollide){
		ddClipCollide->Release();
		ddClipCollide = NULL;
	}
	if(ddsb) {
		ddsb->Release();
		ddsb = NULL;
	}
	if(ddsp) {
		ddsp->Release();
		ddsp = NULL;
	}
	if(ddsc) {
		ddsc->Release();
		ddsc = NULL;
	}

	//Set the new display mode
	if (dd->SetDisplayMode(640, 480, colorDepth, 0, 0) != DD_OK) {
		//print error message
		return false;
	}

	//rebuild all surfaces and clipping regions
  return SetSurfaces(colorDepth);
}

bool cDX::Init(HWND wnd, int colorDepth){

	if (DirectDrawCreateEx(NULL, (void **)&dd, IID_IDirectDraw7, NULL) != DD_OK) {
		//print error message
		return false;
	}

  if (dd->SetCooperativeLevel(wnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK) {
		return false;
  }

	if (dd->SetDisplayMode(640, 480, colorDepth, 0, 0) != DD_OK) {
		//print error message
		return false;
	}

	return SetSurfaces(colorDepth);
}

bool cDX::ReInit(HWND wnd, int colorDepth){

	dd->Release();

	if (DirectDrawCreateEx(NULL, (void **)&dd, IID_IDirectDraw7, NULL) != DD_OK) {
		//print error message
		return false;
	}

  if (dd->SetCooperativeLevel(wnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK) {
		return false;
  }

	if (dd->SetDisplayMode(640, 480, colorDepth, 0, 0) != DD_OK) {
		//print error message
		return false;
	}

	return SetSurfaces(colorDepth);
}

bool cDX::SetSurfaces(int colorDepth){
		DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	//INIT_DXSTRUCT(ddsd); //alternate initialization?

	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;  // valid flags
	ddsd.dwBackBufferCount = 1;                       // one back buffer
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |    // primary surface
                        DDSCAPS_COMPLEX |           // back buffer is chained
                        DDSCAPS_FLIP |              // allow page flipping
                        DDSCAPS_VIDEOMEMORY;        // create in video memory


	ddsd.dwSize = sizeof(DDSURFACEDESC2);

	//Get the primary surface
	if (dd->CreateSurface(&ddsd, &ddsp, NULL) != DD_OK) {
		//print error message;
		return false;
	}

	//Get the backbuffer
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(ddsp->GetAttachedSurface(&ddsd.ddsCaps, &ddsb))){
		// error-handling code here
		return false;
	}

	//For collision detection surface
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwHeight = 480;
	ddsd.dwWidth = 640;
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	if (dd->CreateSurface(&ddsd, &ddsc, NULL) != DD_OK) {
		return false;
	}

	if(colorDepth==8){
		ddPal = DA2LoadPalette(dd, "Gfx/da2pal.txt");
		//ddPal = DDLoadPalette(dd, "Gfx/1.bmp");
		ddsp->SetPalette(ddPal);
		ddsc->SetPalette(ddPal);
	}

	DDCOLORKEY	ddck;
	ddck.dwColorSpaceLowValue = 0;
  ddck.dwColorSpaceHighValue = 0;
  ddsc->SetColorKey(DDCKEY_SRCBLT, &ddck);

	//Make a full-screen clipper
	if(colorDepth==8){
		
	if(dd->CreateClipper(0,&ddClip,NULL) != DD_OK){
		return false;
	}

	LPRGNDATA lpClipList = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
	RECT rcClipRect = {0, 0, 640, 480};

	memcpy(lpClipList->Buffer, &rcClipRect, sizeof(RECT));  // copy the actual clip region
	lpClipList->rdh.dwSize = sizeof(RGNDATAHEADER);         // size of header structure
	lpClipList->rdh.iType = RDH_RECTANGLES;                 // type of clip region
	lpClipList->rdh.nCount = 1;                             // number of clip regions
	lpClipList->rdh.nRgnSize = sizeof(RECT);                // size of lpClipList->Buffer
	lpClipList->rdh.rcBound = rcClipRect;                   // the bounding RECT
	if(ddClip->SetClipList(lpClipList,0) != DD_OK) return false;
	if(ddsb->SetClipper(ddClip) != DD_OK) return false;

	
	//Make another clipper for collision window
	//if(dd->CreateClipper(0,&ddClipCollide,NULL) != DD_OK){
	//	return false;
	//}

	//LPRGNDATA lpClipList2 = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));

	//memcpy(lpClipList2->Buffer, &rcClipRect, sizeof(RECT));  // copy the actual clip region
	//lpClipList2->rdh.dwSize = sizeof(RGNDATAHEADER);         // size of header structure
	//lpClipList2->rdh.iType = RDH_RECTANGLES;                 // type of clip region
	//lpClipList2->rdh.nCount = 1;                             // number of clip regions
	//lpClipList2->rdh.nRgnSize = sizeof(RECT);                // size of lpClipList->Buffer
	//lpClipList2->rdh.rcBound = rcClipRect;                   // the bounding RECT
	//if(ddClipCollide->SetClipList(lpClipList2,0) != DD_OK) return false;
	//if(ddsc->SetClipper(ddClipCollide) != DD_OK) return false;

	}
	//print success message
	return true;
}

LPDIRECTDRAWPALETTE cDX::DDLoadPalette(LPDIRECTDRAW7 pdd, LPCSTR szBitmap){
	LPDIRECTDRAWPALETTE			ddpal;
  int                     i;
  int                     n;
  int                     fh;
  PALETTEENTRY            ape[256];

	char str[64];
	char* tok;
	FILE* f=fopen("da2pal.txt","rt");
	for(i=0;i<256;i++){
		fgets(str,64,f);
		tok=strtok(str," \t\n");
		ape[i].peRed=atoi(tok);
		tok=strtok(NULL," \t\n");
		ape[i].peGreen=atoi(tok);
		tok=strtok(NULL," \t\n");
		ape[i].peBlue=atoi(tok);
	}
	fclose(f);
	pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);
  return ddpal;


    //
    // Build a 332 palette as the default.
    //
    for (i = 0; i < 256; i++)
    {
        ape[i].peRed = (BYTE) (((i >> 5) & 0x07) * 255 / 7);
        ape[i].peGreen = (BYTE) (((i >> 2) & 0x07) * 255 / 7);
        ape[i].peBlue = (BYTE) (((i >> 0) & 0x03) * 255 / 3);
        ape[i].peFlags = (BYTE) 0;
    }
    if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER        bf;
        BITMAPINFOHEADER        bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);
        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;
        //
        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for (i = 0; i < n; i++)
        {
            BYTE        r = ape[i].peRed;

            ape[i].peRed = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }
    pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);
    return ddpal;
}

LPDIRECTDRAWPALETTE cDX::DA2LoadPalette(LPDIRECTDRAW7 pdd, char* file){
	LPDIRECTDRAWPALETTE			ddpal;
  int                     i,j;
	PALETTEENTRY						p;
  
	char str[64];
	char* tok;
	FILE* f=fopen(file,"rt");
	for(i=0;i<256;i++){
		fgets(str,64,f);
		tok=strtok(str," \t\n");
		pal[i].peRed=atoi(tok);
		tok=strtok(NULL," \t\n");
		pal[i].peGreen=atoi(tok);
		tok=strtok(NULL," \t\n");
		pal[i].peBlue=atoi(tok);
		pal[i].peFlags=0;
	}
	fclose(f);
	pdd->CreatePalette(DDPCAPS_8BIT, pal, &ddpal, NULL);

	for(i=0;i<7;i++){
		p=pal[80];
		for(j=0;j<7;j++) pal[80+j]=pal[81+j];
		pal[87]=p;

		p=pal[88];
		for(j=0;j<7;j++) pal[88+j]=pal[89+j];
		pal[95]=p;

		p=pal[128];
		for(j=0;j<7;j++) pal[128+j]=pal[129+j];
		pal[135]=p;

		p=pal[136];
		for(j=0;j<7;j++) pal[136+j]=pal[137+j];
		pal[143]=p;

		pdd->CreatePalette(DDPCAPS_8BIT, pal, &ddPalRot[i], NULL);
	}
  return ddpal;

}


void cDX::RotatePalette(){

	DWORD TimeNow=timeGetTime()-ElapsedTime;
	if(TimeNow < 125 && TimeNow>0) return;
	
	ElapsedTime=timeGetTime();

	//LPDIRECTDRAWPALETTE Pal1;
	//ddsp->GetPalette(&Pal1);

  //PALETTEENTRY work_pal[256];
	rotation++;
	if(rotation==8) {
		rotation=0;
		ddsp->SetPalette(ddPal);
	} else {
		ddsp->SetPalette(ddPalRot[rotation-1]);
	}

	// Rotate palettes instead of entries (as below) because it is faster that way.
  //ddPal->GetEntries(0,80,16,work_pal);
  //ddPal->SetEntries(0,80,7,&work_pal[1]);
  //ddPal->SetEntries(0,87,1,&work_pal[0]);
  //ddPal->SetEntries(0,88,7,&work_pal[9]);
  //ddPal->SetEntries(0,95,1,&work_pal[8]);

  //ddPal->GetEntries(0,128,16,work_pal);
  //ddPal->SetEntries(0,128,7,&work_pal[1]);
  //ddPal->SetEntries(0,135,1,&work_pal[0]);
  //ddPal->SetEntries(0,136,7,&work_pal[9]);
  //ddPal->SetEntries(0,143,1,&work_pal[8]);

}

int cDX::LoadBitmapResource(LPDIRECTDRAWSURFACE7 lpdds, int xDest, int yDest, char *fn) {
  HDC hSrcDC;           // source DC - memory device context
  HDC hDestDC;          // destination DC - surface device context
  HBITMAP hbitmap;      // handle to the bitmap resource
  BITMAP bmp;           // structure for bitmap info
  int nHeight, nWidth;  // bitmap dimensions

  // first load the bitmap resource
  if ((hbitmap = (HBITMAP)LoadImage(NULL, fn,IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE)) == NULL) return(FALSE);

  // create a DC for the bitmap to use
  if ((hSrcDC = CreateCompatibleDC(NULL)) == NULL) return(FALSE);

  // select the bitmap into the DC
  if (SelectObject(hSrcDC, hbitmap) == NULL){
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  // get image dimensions
  if (GetObject(hbitmap, sizeof(BITMAP), &bmp) == 0) {
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  nWidth = bmp.bmWidth;
  nHeight = bmp.bmHeight;

  // retrieve surface DC
  if (FAILED(lpdds->GetDC(&hDestDC))) {
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  // copy image from one DC to the other
  if (BitBlt(hDestDC, xDest, yDest, nWidth, nHeight, hSrcDC, 0, 0, SRCCOPY) == NULL) {
    lpdds->ReleaseDC(hDestDC);
    DeleteDC(hSrcDC);
    return(FALSE);
  }

  // kill the device contexts
  lpdds->ReleaseDC(hDestDC);
  DeleteDC(hSrcDC);

  // return success
  return(TRUE);
}

*/