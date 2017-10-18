//#include <windows.h>
#include <time.h>
#include "DA2DX.h"
#include "DA2Gfx.h"
#include "DA2Game.h"
#include "DA2Input.h"
#include "DA2ItemController.h"
#include "DA2Music.h"
//#include "DA2Party.h"
//#include "DA2ScriptHandler.h"
#include "DA2Title.h"

enum GameState{
	Title,
	MainGame,
	Party,
	Cutscene,
	Battle,
	Exit
};

//HWND g_hWnd;
//HINSTANCE g_hInstance;

//LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//BOOL InitWindows(HINSTANCE hInstance);
//BOOL Process(void);
//BOOL Render(void);
//BOOL GamePrep();

//CDisplay MyDX;
cDA2Gfx MyGfx;
cDA2Game MyGame;
//cDA2Input MyInput;
//cDA2Party MyParty;
cDA2Title MyTitle;
cDA2Music MyMusic;
cItemController ItemList;

GameState gstate;

int main(int argc, char* args[]) {

  srand(time(NULL));

  //read any configurations
  sConf conf;
  FILE* f;
  f=fopen("da2.cfg", "rb");
  if(f != NULL){
    fread(&conf, sizeof(sConf), 1, f);
    fclose(f);
  }

  //Start up SDL and create window
  CDisplay display;
  if(!display.init(conf))	{
    printf("Failed to initialize!\n");
    return -1;
  }
  SDL_RenderSetLogicalSize(display.renderer, 640, 480);
  MyGfx.LoadGfx(&display);
  SDL_ShowCursor(SDL_DISABLE);

  cDA2Input input;
  float sx, sy;
  SDL_RenderGetScale(display.renderer, &sx, &sy);
  printf("w:%d h:%d\t%f - %f\n", display.screenWidth,display.screenHeight,sx,sy);
  input.setMouseOffsets(sx, (display.screenWidth/sx-640)/2, sy, (display.screenHeight/sy-480)/2);

  //Process title screen until player chooses to quit
  MyMusic.PlayTheme();
  if(MyTitle.Init(&display,&input) == false) return 0;

  if(!ItemList.LoadItems()) return false;
  if(!MyGame.Init(&display, &MyGfx, &input, &ItemList, &MyMusic, &conf)) return false;

  bool bQuit=false;

  while(!bQuit){
    input.pollEvents();
    switch(gstate){
    case Title:
      MyTitle.Render();
      switch(MyTitle.Logic()){
      case 1:
        //if(!GamePrep()) return false;
        MyMusic.KillTheme();
        MyGame.LoadMaps();
        MyGame.NewGame();
        gstate=MainGame;
        break;
      case 2:
        //if(!GamePrep()) return false;
        MyMusic.KillTheme();
        MyGame.LoadMaps();
        //MyGame.NewGame();
        MyGame.mainStack.Push(LoadDialog);
        gstate=MainGame;
        break;
      case 3:
        MyTitle.SetCredits(true);
        break;
      case 4:
        //SendMessage(g_hWnd, WM_CLOSE, 0, 0);
        //gstate=Exit;
        bQuit=true;
        break;
      case 5:
        MyTitle.SetCredits(false);
        break;
      default:
        break;
      }
      break;
    case MainGame:
      //MyDX.RotatePalette();
      switch(MyGame.GoGame()){
      case 0:
        //SendMessage(g_hWnd, WM_CLOSE, 0, 0);
        //gstate=Exit;
        bQuit=true;
        break;
      case 2:
        //reset title screen here
        //if(!MyDX.ChangeDisplayMode(32)) return false;
        //if(MyTitle.Init(&MyDX, &MyInput) == false) return 0;
        MyMusic.PlayTheme();
        gstate=Title;
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }

  //CInput inp;

  //CDarkages game(&display);
  //game.title();
  //game.run();

  f=fopen("da2.cfg", "wb");
  if(f != NULL){
    fwrite(&conf, sizeof(sConf), 1, f);
    fclose(f);
  }

  return 0;
}

/*
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
  MSG msg;
	bool bNoProcess=false;

	srand(time(NULL));

  if (!InitWindows(hInstance)) return 0;

	if(MyDX.Init(g_hWnd,32)==false) return 0;
	if(MyInput.Init(hInstance,g_hWnd)==false) return 0;

	if(MyTitle.Init(&MyDX,&MyInput)==false) return 0;

	if(MyMusic.Init()==false) return 0;

	//if(MyGfx.Init(&MyDX)==false) return 0;
	//MyGfx.LoadGFX();

	//if(ItemList.LoadItems()==false) return 0;

	//if(MyGame.Init(&MyDX,&MyGfx,&MyInput,&ItemList)==false) return 0;
	//MyGame.LoadMaps();

	//gstate=MainGame;
	gstate=Title;

  while (TRUE) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			if (msg.message == WM_ACTIVATEAPP) {
				if(bNoProcess){
					bNoProcess=false;
					MyDX.ReInit(g_hWnd);
					MyGfx.CreateSurfaces();
					MyGfx.LoadGFX();
					MyInput.dIKeyboard->Acquire();
					MyInput.dIMouse->Acquire();
					//Make this the restore point
				} else {
					bNoProcess=true;
				}
			}
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

		if(bNoProcess) {
			WaitMessage();
			continue;
		}
    if (!Process()) SendMessage(g_hWnd, WM_CLOSE, 0, 0);
				
        //if (!Render())
        //    SendMessage(g_hWnd, WM_CLOSE, 0, 0);
				
  }
  return msg.message;
}
*/
/*
BOOL Process(void)
{
	
  //if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000))
  //return FALSE;

  bool bCredits=false;

	switch(gstate){
		case Title:
			MyTitle.Render();
			switch(MyTitle.Logic()){
			case 1:
				if(!GamePrep()) return false;
				MyMusic.KillTheme();
				MyGame.LoadMaps();
				MyGame.NewGame();
				gstate=MainGame;
				break;
			case 2:
				if(!GamePrep()) return false;
				MyMusic.KillTheme();
				MyGame.LoadMaps();
				//MyGame.NewGame();
				MyGame.mainStack.Push(LoadDialog);
				gstate=MainGame;
				break;
			case 3:
        MyTitle.SetCredits(true);
        break;
			case 4:
				SendMessage(g_hWnd, WM_CLOSE, 0, 0);
				gstate=Exit;
				break;
      case 5:
        MyTitle.SetCredits(false);
        break;
			default:
				break;
			}
			break;
		case MainGame:
			MyDX.RotatePalette();
			switch(MyGame.GoGame()){
			case 0:
				SendMessage(g_hWnd, WM_CLOSE, 0, 0);
				gstate=Exit;
				break;
			case 2:
				//reset title screen here
				if(!MyDX.ChangeDisplayMode(32)) return false;
				if(MyTitle.Init(&MyDX,&MyInput)==false) return 0;
				MyMusic.PlayTheme();
				gstate=Title;
				break;
			default:
				break;
			}
			break;
		default:
			break;
	}
		
  return TRUE;

}

BOOL GamePrep(){
	if(!MyDX.ChangeDisplayMode()) return false;
	if(!MyGfx.Init(&MyDX)) return false;
	MyGfx.CreateSurfaces();
	MyGfx.LoadGFX();
	MyGfx.CutTiles();
	if(!ItemList.LoadItems()) return false;
	if(!MyGame.Init(&MyDX,&MyGfx,&MyInput,&ItemList,&MyMusic)) return false;
	return true;
}

BOOL Render(void)
{
  return TRUE;
}


BOOL InitWindows(HINSTANCE hInstance)
{
	//g_hInstance = hInstance;

 //   WNDCLASSEX wcex;
 //
 //   wcex.cbSize = sizeof(WNDCLASSEX);
 //   wcex.hInstance = g_hInstance;
 //   wcex.lpszClassName = "MYCLASS";
 //   wcex.lpfnWndProc = WinProc;
 //   wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
 //   wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
 //   wcex.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
 //   wcex.hCursor = LoadCursor (NULL, IDC_ARROW);
 //   wcex.lpszMenuName = NULL;
 //   wcex.cbClsExtra = 0;
 //   wcex.cbWndExtra = 0;
 //   wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
 //
 //   RegisterClassEx(&wcex);
 //
 //   g_hWnd = CreateWindowEx(WS_EX_TOPMOST, "MYCLASS", "Dark Ages II: Engel", WS_POPUP | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, g_hInstance, NULL);
 //
 //   if(!g_hWnd)
 //       return FALSE;

	//return TRUE;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
*/