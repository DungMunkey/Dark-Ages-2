#include "DA2Music.h"
#include <cstdio>
#include <cstring>

cDA2Music::cDA2Music(){
	int i;

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
  strcpy(song[5].FileName,"Cutscene");

  for(i=0;i<6;i++){
    song[i].Track = 1;
    song[i].position = 0;
    song[i].track = NULL;
    song[i].audio = NULL;
  }

	FILE* f=fopen("music/SCORE.DAT","rt");
	for(i=0;i<500;i++) fscanf(f,"%d\n",&Music[i]);
	fclose(f);

  LastPlayed = -1;
  NowPlaying = -1;
	Volume = 5;
	bChangeSong=false;

	mixer=NULL;
	themeTrack=NULL;
	themeAudio=NULL;
}

cDA2Music::~cDA2Music(){
	int i;
	for(i=0;i<6;i++){
		if(song[i].track != NULL) MIX_DestroyTrack(song[i].track);
		if(song[i].audio != NULL) MIX_DestroyAudio(song[i].audio);
	}
	if(themeTrack != NULL) MIX_DestroyTrack(themeTrack);
	if(themeAudio != NULL) MIX_DestroyAudio(themeAudio);
	if(mixer != NULL) MIX_DestroyMixer(mixer);
	MIX_Quit();
}

bool cDA2Music::Init(){
	MIX_Init();
	mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if(mixer == NULL){
		SDL_Log("No audio output: %s", SDL_GetError());
		return true; //the game runs silently
	}

	int i;
	for(i=0;i<6;i++) song[i].track = MIX_CreateTrack(mixer);
	themeTrack = MIX_CreateTrack(mixer);

	return true;
}

bool cDA2Music::stopped(int i){
	return !MIX_TrackPlaying(song[i].track) && !MIX_TrackPaused(song[i].track);
}

void cDA2Music::ChangeSong(int map){
  char strSong[256];
	int i;

	if(map<0) i=snBattle;
  else if (map==99999) i=snCutScene;
	else i=Music[map];

  if(i == NowPlaying) return;
  if(mixer == NULL) { NowPlaying = i; return; }

  //If song is not loaded, load it at last position
  if(stopped(i)) {
		sprintf(strSong,"music/%s%d.ogg",song[i].FileName,song[i].Track);
    LoadSong(i, strSong, song[i].position, true);
  } else if(MIX_TrackPaused(song[i].track)){
    MIX_ResumeTrack(song[i].track);
	}

  LastPlayed = NowPlaying;
  NowPlaying = i;
  bChangeSong = true;
}

void cDA2Music::CheckState() {
	if(NowPlaying==-1) return;
	if(mixer == NULL) return;

	bool bFade=false;
	char strSong[256];
	float vol;
  int i;

  //Handle songs changing due to map change
  if(bChangeSong){

    //Check if songs are playing, if not load the new one
    if(LastPlayed > -1) {
			if(stopped(LastPlayed)) {
				song[LastPlayed].Track++;
        if(song[LastPlayed].Track > song[LastPlayed].MaxTrack) song[LastPlayed].Track = 1;
        song[LastPlayed].position = 0;
				sprintf(strSong,"music/%s%d.ogg",song[LastPlayed].FileName,song[LastPlayed].Track);
        LoadSong(LastPlayed, strSong, 0, false);
			}
		}

    if(stopped(NowPlaying)){
      song[NowPlaying].Track++;
      if(song[NowPlaying].Track > song[NowPlaying].MaxTrack) song[NowPlaying].Track = 1;
      song[NowPlaying].position = 0;
			sprintf(strSong,"music/%s%d.ogg",song[NowPlaying].FileName,song[NowPlaying].Track);
      LoadSong(NowPlaying, strSong, 0, false);
    }


    //Fade out old song
    Uint32 timeNow=SDL_GetTicks();
		Uint32 dTime=timeNow-musicTime;
		if(dTime > 300 || dTime<0){
			musicTime=timeNow;
			bFade=true;
		}

    if(LastPlayed > -1 && MIX_TrackPlaying(song[LastPlayed].track)) {
      vol = MIX_GetTrackGain(song[LastPlayed].track)*100.0f;
      if(bFade) {
        vol = vol-1;
        MIX_SetTrackGain(song[LastPlayed].track, vol/100.0f);
      }
			if(vol <= 0) {
				//Store song position and free resources
				song[LastPlayed].position = MIX_GetTrackPlaybackPosition(song[LastPlayed].track);
        MIX_PauseTrack(song[LastPlayed].track);
				//bChangeSong = false;
        vol = 0;
			}
		}

    //Fade in new song
    vol = MIX_GetTrackGain(song[NowPlaying].track)*100.0f;
    if(bFade) vol = vol+1;
    if(vol > Volume*10){
      //fail safe - turn off all other channels
      for(i=0;i<6;i++){
        if(i==NowPlaying) continue;
        if(MIX_TrackPlaying(song[i].track)){
          song[i].position=MIX_GetTrackPlaybackPosition(song[i].track);
          MIX_PauseTrack(song[i].track);
        }
      }
      vol = Volume*10;
      bChangeSong = false;
    }
    MIX_SetTrackGain(song[NowPlaying].track, vol/100.0f);

    return;
  }

  //Check if song needs changing
  if(stopped(NowPlaying)){
    song[NowPlaying].Track++;
    if(song[NowPlaying].Track > song[NowPlaying].MaxTrack) song[NowPlaying].Track = 1;
    song[NowPlaying].position = 0;
		sprintf(strSong,"music/%s%d.ogg",song[NowPlaying].FileName,song[NowPlaying].Track);
    LoadSong(NowPlaying, strSong, 0, false);
  }
}

void cDA2Music::KillSong(int map){
	if(mixer == NULL) return;
	int i=Music[map];
  MIX_StopTrack(song[i].track, 0);
}

void cDA2Music::KillTheme(){
	if(themeTrack != NULL) MIX_StopTrack(themeTrack, 0);
}

void cDA2Music::LoadSong(int i, char* fn, Sint64 offset, bool silent){
  if(song[i].audio != NULL) MIX_DestroyAudio(song[i].audio);
  song[i].audio = MIX_LoadAudio(mixer, fn, false); //false = stream from disk rather than decode it all up front
  MIX_SetTrackAudio(song[i].track, song[i].audio);
  MIX_SetTrackPlaybackPosition(song[i].track, offset);
	MIX_SetTrackGain(song[i].track, (Volume*10)/100.0f);
  MIX_PlayTrack(song[i].track, 0);
}

void cDA2Music::PlayTheme(){
	if(mixer == NULL) return;
	if(themeAudio != NULL) MIX_DestroyAudio(themeAudio);
	themeAudio = MIX_LoadAudio(mixer, "music/theme.ogg", false);
	MIX_SetTrackAudio(themeTrack, themeAudio);
	SDL_PropertiesID options = SDL_CreateProperties();
	SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
	MIX_PlayTrack(themeTrack, options);
	SDL_DestroyProperties(options);
}

void cDA2Music::SetVolume(int vol){
	Volume=vol;
	if(mixer == NULL || NowPlaying == -1) return;
	MIX_SetTrackGain(song[NowPlaying].track, (Volume*10)/100.0f);
}
