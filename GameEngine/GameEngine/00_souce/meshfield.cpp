//==========================================
//
//  メッシュフィールド(meshfield.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshfield.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  マクロ定義
//==========================================
#define NUMMESH (D3DXVECTOR2(3, 2)) //分割数
#define NUMPOINT_U (int)(NUMMESH.x + 1.0f) //横方向の頂点数
#define NUMPOINT_V (int)(NUMMESH.y + 1.0f) //縦方向の頂点数
#define NUMPOINT (int)(NUMPOINT_U * NUMPOINT_V) //全体の頂点数
#define NUMINDEX (int)((NUMPOINT * 2) - (2 * (abs((int)NUMMESH.x - (int)NUMMESH.y)) + 4)) //インデックス数
#define NUMPOLYGON (int)(NUMINDEX - 2) //生成するポリゴン数

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nShadowNum;
}MeshField;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMEshField = NULL;
MeshField g_aMeshField; //床の情報

//==========================================
//  初期化処理
//==========================================
void InitMeshField(void)
{
	//各種変数の初期化
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nShadowNum = -1;

	//ポリゴンの初期化
	g_pVtxBuffMeshField = Init_3D_Polygon(NUMPOLYGON);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(-100, -0.01f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(100.0f, -0.01f, 300.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);

	pVtx[4].pos = D3DXVECTOR3(-300.0f, -0.01f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(-100, -0.01f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(100.0f, -0.01f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(300.0f, -0.01f, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(-300.0f, -0.01f, -300.0f);
	pVtx[9].pos = D3DXVECTOR3(-100, -0.01f, -300.0f);
	pVtx[10].pos = D3DXVECTOR3(100.0f, -0.01f, -300.0f);
	pVtx[11].pos = D3DXVECTOR3(300.0f, -0.01f, -300.0f);

	//頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * NUMINDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMEshField,
		NULL
	);

	//インデックス情報へのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMEshField->Lock(0, 0, (void**)&pIdx, 0);

	//頂点データの設定
	for (int nCntIdx = 0, nCntTurn = 0, nCntPoint = 0; nCntIdx < NUMINDEX; nCntIdx += 2)
	{
		if (nCntIdx == (2 * ((NUMPOINT_U * nCntTurn) + NUMPOINT_U + nCntTurn))) //折り返し地点
		{
			pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			pIdx[nCntIdx + 1] = nCntPoint + NUMPOINT_U;
			nCntTurn++;
		}
		else //その他
		{
			pIdx[nCntIdx] = nCntPoint + NUMPOINT_U;
			pIdx[nCntIdx + 1] = nCntPoint;
			nCntPoint++;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMEshField->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitMeshField(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffMeshField);

	//インデックスバッファの破棄
	if (g_pIdxBuffMEshField != NULL)
	{
		g_pIdxBuffMEshField->Release();
		g_pIdxBuffMEshField = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateMeshField(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawMeshField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshField.mtxWorld);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMEshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, *SetTexture(0));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		NUMPOINT, //頂点数
		0,
		NUMPOLYGON //プリミティブ数
	);
}
