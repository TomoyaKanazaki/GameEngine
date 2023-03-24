//==========================================
//
//  カメラの制御(camera.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

//==========================================
//  カメラ構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 posV; //視点
	D3DXVECTOR3 posR; //注視点
	D3DXVECTOR3 vecU; //上方向ベクトル
	D3DXMATRIX mtxProjection; //プロジェクションマトリックス
	D3DXMATRIX mtxView; //ビューマトリックス
}Camera;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCameraData(void);

#endif
