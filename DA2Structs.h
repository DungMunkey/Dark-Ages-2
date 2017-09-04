#ifndef _DA2STRUCTS_H
#define _DA2STRUCTS_H

typedef struct sConf{
  int w;
  int h;
  int vol;
  bool fullScreen;
  sConf(){
    w=1280;
    h=960;
    vol=5;
    fullScreen=false;
  }
}sConf;

#endif