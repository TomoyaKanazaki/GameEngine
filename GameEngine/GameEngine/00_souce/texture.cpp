//==========================================
//
//  テクスチャ管理用ファイル(texture.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "texture.h"

//==========================================
//  テクスチャファイル一覧
//==========================================
const char *c_pTextureFilePass[TEXTURE_MAX] =
{
	"data/TEXTURE/effect_000.jpg", //エフェクトでデフォルトに使うテクスチャ
	"data/TEXTURE/effect_001.jpg", //エフェクトに使う四角のテクスチャ
}; //テクスチャパス

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTexture[TEXTURE_MAX] = {};

//==========================================
//  初期化処理
//==========================================
void InitTexture()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_pTextureFilePass[nCnt],
			&g_pTexture[nCnt]
		);
	}
}

//==========================================
//  終了処理
//==========================================
void UninitTexture()
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < TEXTURE_MAX; nCnt++)
	{
		if (g_pTexture[nCnt] != NULL)
		{
			g_pTexture[nCnt]->Release();
			g_pTexture[nCnt] = NULL;
		}
	}
}

//==========================================
//  テクスチャの取得
//==========================================
LPDIRECT3DTEXTURE9 GetTexture(int TextureLabel)
{
	return g_pTexture[TextureLabel];
}
