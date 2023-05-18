//==========================================
//
//  モデルの管理(model.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MODEL_H_
#define _MODEL_H_
#include "main.h"

//==========================================
//  モデルタイプの列挙型定義
//==========================================
typedef enum
{
	MODELTYPE_PLAYER = 0, //プレイヤーのモデル
	MODELTYPE_ENEMY, //敵のモデル
	MODELTYPE_MAX
}MODEL_TYPE;

//==========================================
//  モデル構造体定義
//==========================================
typedef struct
{
	LPDIRECT3DTEXTURE9 *pTexture; //テクスチャへのポインタ
	LPD3DXMESH pMesh; //頂点情報のポインタ
	LPD3DXBUFFER pBuffMat; //マテリアル情報のポインタ
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	DWORD dwNumMat; //マテリアル数
	D3DXVECTOR3 vtxMax; //頂点の最大値
	D3DXVECTOR3 vtxMin; //頂点の最小値
	D3DXVECTOR3 pos; //位置
	D3DXVECTOR3 rot; //角度
	float fHeight; //モデルの高さ
	float fRadius; //モデルの半径
	int nIdxModelParent; //親モデルのインデックス
}MODEL;

//==========================================
//  プロトタイプ宣言
//==========================================
HRESULT InitModel(void); //読み込み、初期化
void UninitModel(void); //破棄
MODEL *GetModelData(int nIdx); //モデル情報の取得

#endif
