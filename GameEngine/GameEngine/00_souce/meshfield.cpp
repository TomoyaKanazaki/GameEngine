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
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL; //頂点バッファ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL; //インデックスバッファ
Mesh g_aMeshField; //床の情報

//==========================================
//  初期化処理
//==========================================
void InitMeshField(void)
{
	//各種変数の初期化
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.data.size = D3DXVECTOR2(500.0f, 500.0f);
	g_aMeshField.data.nNumMesh_U = 4;
	g_aMeshField.data.nNumMesh_V = 4;

	//頂点情報を生成
	ResetMeshField();
}

//==========================================
//  終了処理
//==========================================
void UninitMeshField(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffMeshField);

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
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
	pDevice->SetIndices(g_pIdxBuffMeshField);

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
		g_aMeshField.data.nNumVtx, //頂点数
		0,
		g_aMeshField.data.nNumMesh //プリミティブ数
	);
}

//==========================================
//  メッシュフィールドの再設定
//==========================================
void ResetMeshField()
{
	//頂点生成用変数の設定
	g_aMeshField.data.nNumVtx_U = g_aMeshField.data.nNumMesh_U + 1;
	g_aMeshField.data.nNumVtx_V = g_aMeshField.data.nNumMesh_V + 1;
	g_aMeshField.data.nNumVtx = g_aMeshField.data.nNumVtx_U * g_aMeshField.data.nNumVtx_V;
	g_aMeshField.data.nNumIdx = (g_aMeshField.data.nNumVtx * 2) - (2 * (abs(g_aMeshField.data.nNumMesh_U - g_aMeshField.data.nNumMesh_V)) + 4);
	g_aMeshField.data.nNumMesh = g_aMeshField.data.nNumIdx - 2;

	//ポリゴンの初期化
	g_pVtxBuffMeshField = Init_3D_Polygon(g_aMeshField.data.nNumMesh);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntVtxU = 0; nCntVtxU < g_aMeshField.data.nNumVtx_U; nCntVtxU++)
	{
		for (int nCntVtxV = 0; nCntVtxV < g_aMeshField.data.nNumVtx_V; nCntVtxV++)
		{
			pVtx[nCntVtxU + (nCntVtxV * g_aMeshField.data.nNumVtx_U)].pos = D3DXVECTOR3
			(
				-(g_aMeshField.data.size.x * 0.5f) + ((g_aMeshField.data.size.x / g_aMeshField.data.nNumMesh_U) * nCntVtxU),
				0.0f,
				(g_aMeshField.data.size.y * 0.5f) - ((g_aMeshField.data.size.y / g_aMeshField.data.nNumMesh_V) * nCntVtxV)
			);
		}
	}

	for (int nCntVtx = 0; nCntVtx < g_aMeshField.data.nNumVtx; nCntVtx++)
	{
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * g_aMeshField.data.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL
	);

	//インデックス情報へのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//頂点データの設定
	for (int nCntIdx = 0, nCntTurn = 0, nCntVtx = 0; nCntIdx < g_aMeshField.data.nNumIdx; nCntIdx += 2)
	{
		if (nCntIdx == (2 * ((g_aMeshField.data.nNumVtx_U * nCntTurn) + g_aMeshField.data.nNumVtx_U + nCntTurn))) //折り返し地点
		{
			pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			pIdx[nCntIdx + 1] = nCntVtx + g_aMeshField.data.nNumVtx_U;
			nCntTurn++;
		}
		else //その他
		{
			pIdx[nCntIdx] = nCntVtx + g_aMeshField.data.nNumVtx_U;
			pIdx[nCntIdx + 1] = nCntVtx;
			nCntVtx++;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshField->Unlock();
}

//==========================================
//  フィールド情報の取得
//==========================================
Mesh *GetMeshField()
{
	return &g_aMeshField;
}

//==========================================
//  頂点情報の取得
//==========================================
LPDIRECT3DVERTEXBUFFER9 GetVtxData(void)
{
	return g_pVtxBuffMeshField;
}

//==========================================
//  メッシュ情報のセーブ
//==========================================
void SaveMeshField()
{

}
