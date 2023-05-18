//==========================================
//
//  プレイヤーの処理(player.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"
#include "model.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_PLAYERMODEL (32) //プレイヤーに使用するモデルの最大数

//==========================================
//  プレイヤー構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //中心座標
	D3DXVECTOR3 posOld; //前回座標
	D3DXVECTOR3 rot; //角度
	D3DXVECTOR3 move; //移動量
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	MODEL aModel[MAX_PLAYERMODEL]; //使用するモデル
	int nNumModel; //使用しているモデルの総数
	bool bUse;
}PLAYER;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayerData(void);

#endif
