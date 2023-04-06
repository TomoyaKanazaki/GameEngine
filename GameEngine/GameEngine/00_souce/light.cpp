//==========================================
//
//  ライトの制御(light.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "light.h"

//==========================================
//  マクロ定義
//==========================================
#define NUM_LIGHT (3) //ライトの数

//==========================================
//  グローバル変数宣言
//==========================================
D3DLIGHT9 g_Light[NUM_LIGHT]; //ライト情報

//==========================================
//  初期化処理
//==========================================
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXVECTOR3 vecDir; //設定用の方向ベクトル

	for (int nCnt = 0; nCnt < NUM_LIGHT; nCnt++)
	{
		//ライト情報のクリア
		ZeroMemory(&g_Light[nCnt], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_Light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの方向を設定
		switch (nCnt)
		{
		case 0:
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			g_Light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			g_Light[nCnt].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			break;
		case 2:
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			g_Light[nCnt].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir); //ベクトルの正規化
		g_Light[nCnt].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCnt, &g_Light[nCnt]);

		//ライトを有効化する
		pDevice->LightEnable(nCnt, TRUE);
	}
}

//==========================================
//  終了処理
//==========================================
void UninitLight(void)
{

}

//==========================================
//  更新処理
//==========================================
void UpdateLight(void)
{

}
