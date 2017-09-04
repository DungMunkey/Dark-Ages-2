#ifndef _CAMERA_H
#define _CAMERA_H

class cCameraController {

public:
	int TilePosX;
	int TilePosY;
	int OffsetX;
	int OffsetY;
	int CameraX;
	int CameraY;

	cCameraController();

	void BumpCamera(int shiftX, int shiftY);
	void PanCamera(int PosX, int PosY, int Speed);
	void SetCamera(int PosX, int PosY);

private:
	//int CameraX;
	//int CameraY;

};

#endif