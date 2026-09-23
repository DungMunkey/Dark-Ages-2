#include "Camera.h"
#include <iostream>

cCameraController::cCameraController(){
	CameraX=0;
	CameraY=0;
	TilePosX=0;
	TilePosY=0;
	OffsetX=0;
	OffsetY=0;
}

void cCameraController::BumpCamera(int shiftX, int shiftY){
	CameraX += shiftX;
	CameraY += shiftY;
	TilePosX = CameraX/32;
	TilePosY = CameraY/32;
	OffsetX = CameraX % 32;
	OffsetY = CameraY % 32;
}

void cCameraController::SetCamera(int PosX, int PosY){
	CameraX = PosX;
	CameraY = PosY;
	TilePosX = CameraX/32;
	TilePosY = CameraY/32;
	OffsetX = CameraX % 32;
	OffsetY = CameraY % 32;
}

void cCameraController::PanCamera(int PosX, int PosY, int Speed) {
	int j;
	j=CameraX-PosX;
	if(j<0){
		if(abs(j)>Speed) CameraX+=Speed;
		else CameraX+=abs(j);
	} else {
		if(j>Speed) CameraX-=Speed;
		else CameraX-=j;
	}
	j=CameraY-PosY;
	if(j<0){
		if(abs(j)>Speed) CameraY+=Speed;
		else CameraY+=abs(j);
	} else {
		if(j>Speed) CameraY-=Speed;
		else CameraY-=j;
	}
	TilePosX = CameraX/32;
	TilePosY = CameraY/32;
	OffsetX = CameraX % 32;
	OffsetY = CameraY % 32;
}







	