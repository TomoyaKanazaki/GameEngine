//==========================================
//
//  カメラの制御(camera.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "main.h"
#include "camera.h"

//==========================================
//  マクロ定義
//==========================================
#define CAMERA_FOVY (54.0f) //視野角

//==========================================
//  グローバル変数
//==========================================
Camera g_camera; //カメラの情報

//==========================================
//カメラの初期化処理
//==========================================
void InitCamera()
{
	//各種変数の初期化
	g_camera.posV = D3DXVECTOR3(0.0f, 1000.0f, -1000.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//==========================================
//  カメラの終了処理
//==========================================
void UninitCamera(void)
{

}

//==========================================
//  カメラの更新処理
//==========================================
void UpdateCamera(void)
{

}

//==========================================
//カメラの設定処理
//==========================================
void SetCamera(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	(
		&g_camera.mtxProjection,
		D3DXToRadian(CAMERA_FOVY),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		10000.0f
	);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//==========================================
//  カメラ情報の取得
//==========================================
Camera *GetCameraData()
{
	return &g_camera;
}
