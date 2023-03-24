//==========================================
//
//  ポリゴンの汎用処理(polygon.cpp)
//  Author : Tommoya Kanazaki
//
//==========================================
#include "polygon.h"

//==========================================
//  3Dバーテックスバッファの生成
//==========================================
LPDIRECT3DVERTEXBUFFER9 Init_3D_Polygon(int nNum)
{
	//ローカル変数宣言
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * nNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL
	);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < nNum; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		if (nNum != 1)
		{
			//頂点データのポインタを進める
			pVtx += 4;
		}
	}

	//頂点バッファをアンロック
	pVtxBuff->Unlock();

	return pVtxBuff;
}

//==========================================
//  2Dバーテックスバッファの生成
//==========================================
LPDIRECT3DVERTEXBUFFER9 Init_2D_Polygon(int nNum)
{
	//ローカル変数宣言
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * nNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < nNum; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//法線ベクトルの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		if (nNum != 1)
		{
			//頂点データのポインタを進める
			pVtx += 4;
		}
	}

	//頂点バッファをアンロック
	pVtxBuff->Unlock();

	return pVtxBuff;
}

//==========================================
//  終了処理
//==========================================
void UninitPolygon(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
{
	//頂点バッファの破棄
	if ((*pVtxBuff) != NULL)
	{
		(*pVtxBuff)->Release();
		(*pVtxBuff) = NULL;
	}
}
