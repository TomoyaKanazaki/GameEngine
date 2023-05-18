//==========================================
//
//  モデルの管理(model.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "model.h"

//==========================================
//  マクロ定義
//==========================================
#define MIN_SIZE_DEFAULT (D3DXVECTOR3(99999.9f, 99999.9f, 99999.9f)) //最小頂点の基準値
#define MAX_SIZE_DEFAULT (D3DXVECTOR3(-99999.9f, -99999.9f, -99999.9f)) //最大頂点の基準値

//==========================================
//  モデルファイルパス一覧
//==========================================
const char *c_apModelPass[] = 
{
	"02_data/02_MODEL/00_Player.x", //プレイヤー
	"02_data/02_MODEL/01_Enemy.x" //敵
};

//==========================================
//  プロトタイプ宣言
//==========================================
HRESULT LoadModel(void); //xファイルの読み込み
HRESULT LoadTexture(void); //テクスチャの読み込み
void CalcModelSize(void); //モデルサイズの計測

//==========================================
//  グローバル変数宣言
//==========================================
MODEL g_aModel[MODELTYPE_MAX]; //モデル情報

//==========================================
//  初期化処理
//==========================================
HRESULT InitModel()
{
	//変数の初期化
	ZeroMemory(&g_aModel[0], sizeof(MODEL) * MODELTYPE_MAX);
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		g_aModel[nCntModel].vtxMax = MAX_SIZE_DEFAULT;
		g_aModel[nCntModel].vtxMin = MIN_SIZE_DEFAULT;
	}

	//xファイルの読み込み
	if (FAILED(LoadModel()))
	{
		return E_FAIL;
	}

	//テクスチャの読み込み
	if (FAILED(LoadTexture()))
	{
		return E_FAIL;
	}

	//モデルサイズの計測
	CalcModelSize();

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void UninitModel()
{
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//テクスチャの破棄
		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//メモリの開放
		free(g_aModel[nCntModel].pTexture);

		//頂点バッファの破棄
		if (g_aModel[nCntModel].pMesh != NULL)
		{
			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aModel[nCntModel].pBuffMat != NULL)
		{
			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//==========================================
//  xファイルの読み込み
//==========================================
HRESULT LoadModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//ファイルの読み込み
		HRESULT hr = D3DXLoadMeshFromX
		(
			c_apModelPass[nCntModel], //モデルパス
			D3DXMESH_SYSTEMMEM, //頂点情報生成オプション
			pDevice, //デバイスへのポインタ
			NULL, //隣接性データ
			&g_aModel[nCntModel].pBuffMat, //マテリアルへのポインタ
			NULL, //エフェクトデータ
			&g_aModel[nCntModel].dwNumMat, //マテリアル数
			&g_aModel[nCntModel].pMesh //頂点情報のポインタ
		);

		//エラーメッセージ
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to load x file", "Error", MB_ICONWARNING);
			return E_FAIL;
		}

		//使用されるアドレスを取得
		g_aModel[nCntModel].pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9*) * g_aModel[nCntModel].dwNumMat);

		//NULLチェック
		if (g_aModel[nCntModel].pTexture == NULL)
		{
			//エラーメッセージ
			MessageBox(NULL, "Failed to allocate memory", "Error", MB_ICONWARNING);

			//確保に成功した分のメモリを開放する
			for (int nCntFree = 0; nCntFree < nCntModel; nCntFree++)
			{
				free(g_aModel[nCntFree].pTexture);
			}
			return E_FAIL;
		}
	}

	return S_OK;
}

//==========================================
//  テクスチャの読み込み
//==========================================
HRESULT LoadTexture(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//マテリアル情報の取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		//テクスチャ読み込み
		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat])))
				{
					//エラーメッセージ
					MessageBox(NULL, "Failure to load textures", "Error！", MB_ICONWARNING);
					return E_FAIL;
				}
			}
			else
			{
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}
	}

	return S_OK;
}

//==========================================
//  モデルサイズの計測
//==========================================
void CalcModelSize(void)
{
	//ローカル変数宣言
	D3DXVECTOR3 vtx; //計算用変数

	//頂点バッファの生成
	BYTE *pVtxBuff;

	//サイズを計測
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//計算用変数宣言
		int nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices(); //頂点数の取得
		DWORD dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF()); //頂点フォーマットサイズの取得

		//頂点バッファをロック
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点情報の取得
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			//最小、最大の頂点を算出
			if (vtx.x < g_aModel[nCntModel].vtxMin.x)
			{
				g_aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModel[nCntModel].vtxMax.x)
			{
				g_aModel[nCntModel].vtxMax.x = vtx.x;
			}
			if (vtx.y < g_aModel[nCntModel].vtxMin.y)
			{
				g_aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModel[nCntModel].vtxMax.y)
			{
				g_aModel[nCntModel].vtxMax.y = vtx.y;
			}
			if (vtx.z < g_aModel[nCntModel].vtxMin.z)
			{
				g_aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModel[nCntModel].vtxMax.z)
			{
				g_aModel[nCntModel].vtxMax.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += dwSizeFVF;
		}

		//モデルの頂点バッファをアンロック
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		//モデルサイズを算出
		D3DXVECTOR3 size = g_aModel[nCntModel].vtxMax - g_aModel[nCntModel].vtxMin;

		// モデルの高さを算出
		g_aModel[nCntModel].fHeight = size.y;

		// モデル半径を算出
		g_aModel[nCntModel].fRadius = ((size.x * 0.5f) + (size.z * 0.5f)) * 0.5f;
	}
}

//==========================================
//  モデル情報の取得
//==========================================
MODEL *GetModelData(int nIdx)
{
	return &g_aModel[nIdx];
}
