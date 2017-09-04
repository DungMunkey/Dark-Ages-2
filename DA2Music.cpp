#include "DA2Music.h"

using namespace std;

cDA2Music::cDA2Music(){
	int i;
	FILE* f;

  song[1].MaxTrack = 3;
  song[2].MaxTrack = 2;
  song[3].MaxTrack = 3;
  song[4].MaxTrack = 2;
  song[5].MaxTrack = 1;
  
  strcpy(song[0].FileName,"Battle");
  strcpy(song[1].FileName,"TownA");
  strcpy(song[2].FileName,"TownB");
  strcpy(song[3].FileName,"Travel");
  strcpy(song[4].FileName,"Dungeon");
  strcpy(song[5].FileName,"CutScene");
  
  for(i=0;i<6;i++){
    song[i].Track = 1;
    song[i].position = sf::Time::Zero;
  }

	f=fopen("music/SCORE.DAT","rt");
	for(i=0;i<500;i++) fscanf(f,"%d\n",&Music[i]);
	fclose(f);
  
  LastPlayed = -1;
  NowPlaying = -1;
	Volume = 5;
	bChangeSong=false;
}

cDA2Music::~cDA2Music(){
  /*1
	for(int i=0;i<6;i++){
    BASS_MusicFree(song[i].handle);
  }
  */
}

void cDA2Music::ChangeSong(int map){
  char strSong[256];
	int i;
	
	if(map<0) i=snBattle;
  else if (map==99999) i=snCutScene;
	else i=Music[map];

  if(i == NowPlaying) return;
  
  //If song is not loaded, load it at last position
  if(song[i].song.getStatus() == sf::Music::Stopped) {
		sprintf(strSong,"music/%s%d.ogg",song[i].FileName,song[i].Track);
    LoadSong(i, strSong, song[i].position, true);
  } else if(song[i].song.getStatus() == sf::Music::Paused){
    song[i].song.play();
	}
  
  LastPlayed = NowPlaying;
  NowPlaying = i;
  bChangeSong = true;
}

void cDA2Music::CheckState() {
	if(NowPlaying==-1) return;

	bool bFade=false;
	char strSong[256];
	//DWORD freq;
	float vol;
	int pan;
  int i;
	
  //Handle songs changing due to map change
  if(bChangeSong){
  
    //Check if songs are playing, if not load the new one
    if(LastPlayed > -1) {
			if(song[LastPlayed].song.getStatus()==sf::Music::Stopped) {
				song[LastPlayed].Track++;
        if(song[LastPlayed].Track > song[LastPlayed].MaxTrack) song[LastPlayed].Track = 1;
        song[LastPlayed].position = sf::Time::Zero;
				sprintf(strSong,"music/%s%d.ogg",song[LastPlayed].FileName,song[LastPlayed].Track);
        LoadSong(LastPlayed, strSong, sf::Time::Zero, false);
			}
		}
    
    if(song[NowPlaying].song.getStatus() == sf::Music::Stopped){
      song[NowPlaying].Track++;
      if(song[NowPlaying].Track > song[NowPlaying].MaxTrack) song[NowPlaying].Track = 1;
      song[NowPlaying].position = sf::Time::Zero;
			sprintf(strSong,"music/%s%d.ogg",song[NowPlaying].FileName,song[NowPlaying].Track);
      LoadSong(NowPlaying, strSong, sf::Time::Zero, false);
    }
    

    //Fade out old song
    Uint32 timeNow=SDL_GetTicks();
		Uint32 dTime=timeNow-musicTime;
		if(dTime > 300 || dTime<0){
			musicTime=timeNow;
			bFade=true;
		}

    if(LastPlayed > -1 && song[LastPlayed].song.getStatus()==sf::Music::Playing) {
      vol = song[LastPlayed].song.getVolume();
      if(bFade) {
        vol = vol-1;
        song[LastPlayed].song.setVolume(vol);
      }
			if(vol <= 0) {
				//Store song position and free resources
				song[LastPlayed].position = song[LastPlayed].song.getPlayingOffset();
        song[LastPlayed].song.pause();
				//bChangeSong = false;
        vol = 0;
			}
		}
    
    //Fade in new song
    vol = song[NowPlaying].song.getVolume();
    if(bFade) vol = vol+1;
    if(vol > Volume*10){
      //fail safe - turn off all other channels
      for(i=0;i<6;i++){
        if(i==NowPlaying) continue;
        if(song[i].song.getStatus()==sf::Music::Playing){
          song[i].position=song[i].song.getPlayingOffset();
          song[i].song.pause();
        }
      }
      vol = Volume*10;
      bChangeSong = false;
    }
    song[NowPlaying].song.setVolume(vol);
    
    return;
  }
  
  //Check if song needs changing
  if(song[NowPlaying].song.getStatus() == sf::Music::Stopped){
    song[NowPlaying].Track++;
    if(song[NowPlaying].Track > song[NowPlaying].MaxTrack) song[NowPlaying].Track = 1;
    song[NowPlaying].position = sf::Time::Zero;
		sprintf(strSong,"music/%s%d.ogg",song[NowPlaying].FileName,song[NowPlaying].Track);
    LoadSong(NowPlaying, strSong, sf::Time::Zero, false);
  }
}

bool cDA2Music::Init(){
  /*1
	BASS_Init(1, 44100, 0, 0, 0);
	PlayTheme();
	return true;
  */
}

void cDA2Music::KillSong(int map){
	int i=Music[map];
  song[i].song.stop();
}

void cDA2Music::KillTheme(){
	DA2ThemeStream.stop();
}

void cDA2Music::LoadSong(int i, char* fn, sf::Time offset, bool silent){  
  song[i].song.openFromFile(fn);
  song[i].song.setPlayingOffset(offset);
  //song[i].song.setVolume(0);
	song[i].song.setVolume(Volume*10);
  song[i].song.play(); 
}

void cDA2Music::PlayTheme(){
  DA2ThemeStream.setLoop(true);
	DA2ThemeStream.openFromFile("music/theme.ogg");
  DA2ThemeStream.play();
}

void cDA2Music::SetVolume(int vol){
	Volume=vol;
	song[NowPlaying].song.setVolume(Volume*10);
}