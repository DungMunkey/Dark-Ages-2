#ifndef _DA2_MUSIC_H
#define _DA2_MUSIC_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

enum SongName {
  snBattle,
  snTown1,
  snTown2,
  snTravel,
  snDungeon,
  snCutScene
};

typedef struct SongType {
  MIX_Track* track;
  MIX_Audio* audio;    //the currently-loaded file for this slot; replaced (and the old one destroyed) whenever the track number advances
  int Track;
  int MaxTrack;
  Sint64 position;     //playback position in sample frames, remembered while paused for a crossfade
  char FileName[9];
} SongType;

class cDA2Music {

public:
	cDA2Music();
	~cDA2Music();

	void ChangeSong(int map);
	void CheckState();
	bool Init();
	void KillSong(int map);
	void KillTheme();
	void PlayTheme();
	void SetVolume(int vol);

protected:

private:
	SongType song[6];

	bool bChangeSong;

	int Music[500]; //track number to play for all 500 maps.
	int NowPlaying;
	int LastPlayed;
	int Volume;

	Uint32 musicTime;

	MIX_Mixer* mixer;      //the audio device; NULL if none was available (the game then just runs silently)
	MIX_Track* themeTrack; //separate from song[] - only used for the title screen theme
	MIX_Audio* themeAudio;

	bool stopped(int i); //true if slot i is neither playing nor paused (i.e. its current file ran out)
	void LoadSong(int i, char* fn, Sint64 offset, bool silent);

};

#endif
