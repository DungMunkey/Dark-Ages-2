#ifndef _DA2_MUSIC_H
#define _DA2_MUSIC_H

#include <SDL.h>
#include "SFML/Audio.hpp"
#include <iostream>

enum SongName {
  snBattle,
  snTown1,
  snTown2,
  snTravel,
  snDungeon,
  snCutScene
};

typedef struct SongType {
  sf::Music song;
  int Track;
  int MaxTrack;
  sf::Time position;
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
  //1 sf::Music song[6];

	bool bChangeSong;

	int Music[500]; //track number to play for all 500 maps.
	int NowPlaying;
	int LastPlayed;
	int Volume;

	Uint32 musicTime;

	//HSTREAM DA2ThemeStream;
  sf::Music DA2ThemeStream;

	void LoadSong(int i, char* fn, sf::Time offset, bool silent);

	/*


Public Function CheckSong(ByVal i As Integer) As Boolean
  'True if song is still playing
  If BASS_ChannelIsActive(song(i).handle) = BASS_ACTIVE_STOPPED Then
    CheckSong = False
  Else
    CheckSong = True
  End If
End Function

Public Sub PauseMusic()
  Dim i As Integer
  
  For i = 0 To 5
    If BASS_ChannelIsActive(song(i).handle) = BASS_ACTIVE_PLAYING Then
      Call BASS_ChannelPause(song(i).handle)
    End If
  Next i
  
End Sub

Public Sub ResumeMusic()
  Dim i As Integer
  
  For i = 0 To 5
    If BASS_ChannelIsActive(song(i).handle) = BASS_ACTIVE_PAUSED Then
      Call BASS_ChannelResume(song(i).handle)
    End If
  Next i
  
End Sub

*/

};

#endif