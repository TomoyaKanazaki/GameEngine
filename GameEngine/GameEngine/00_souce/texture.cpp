//==========================================================
//
//テクスチャ処理 [texture.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "texture.h"
#include "fileload.h"

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DTEXTURE9 g_apTexture[MAX_TEXTUREFILE] = {};			//テクスチャへのポインタ配列

//==========================================================
//テクスチャ初期化処理
//==========================================================
void InitTexture(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//テクスチャの読み込み
	for (int nCntTexture = 0; nCntTexture < GetTextureCnt(); nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, pTextureFileName(nCntTexture), &g_apTexture[nCntTexture]);
	}
}

//==========================================================
//テクスチャ終了処理
//==========================================================
void UninitTexture(void)
{
	//テクスチャの廃棄
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTUREFILE; nCntTexture++)
	{
		if (g_apTexture[nCntTexture] != NULL)
		{//テクスチャデータが存在している場合
			g_apTexture[nCntTexture]->Release();
			g_apTexture[nCntTexture] = NULL;
		}
	}
}

//==========================================================
//テクスチャ更新処理
//==========================================================
void UpdateTexture(void)
{

}

//==========================================================
//指定されたテクスチャ情報を渡す
//==========================================================
LPDIRECT3DTEXTURE9 *SetTexture(int nIdx)
{
	return &g_apTexture[nIdx];
}