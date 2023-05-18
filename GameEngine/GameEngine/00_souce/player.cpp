//==========================================
//
//  プレイヤーの処理(player.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "player.h"
#include "model.h"
#include "input.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_PLAYER (4) //プレイヤーの最大数

//==========================================
//  グローバル変数宣言
//==========================================
PLAYER g_aPlayer[MAX_PLAYER]; //プレイヤー情報

//==========================================
//  初期化処理
//==========================================
void InitPlayer()
{
	//変数の初期化
	ZeroMemory(&g_aPlayer[0], sizeof(PLAYER) * MAX_PLAYER);

	//モデルの設定
	g_aPlayer[0].nNumModel = 2;
	g_aPlayer[0].aModel[0] = *GetModelData(MODELTYPE_PLAYER);
	g_aPlayer[0].aModel[1] = *GetModelData(MODELTYPE_ENEMY);

	//各パーツの階層構造設定
	g_aPlayer[0].aModel[0].nIdxModelParent = -1;
	g_aPlayer[0].aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer[0].aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_aPlayer[0].aModel[1].nIdxModelParent = 0;
	g_aPlayer[0].aModel[1].pos = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	g_aPlayer[0].aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_aPlayer[0].bUse = true;
}

//==========================================
//  終了処理
//==========================================
void UninitPlayer()
{

}

//==========================================
//  更新処理
//==========================================
void UpdatePlayer()
{
	if (GetKeyboardPress(DIK_W))
	{
		g_aPlayer[0].move.z = 10;
	}
	if (GetKeyboardPress(DIK_S))
	{
		g_aPlayer[0].move.z = -10;
	}
	if (GetKeyboardPress(DIK_A))
	{
		g_aPlayer[0].move.x = -10;
	}
	if (GetKeyboardPress(DIK_D))
	{
		g_aPlayer[0].move.x = 10;
	}

	g_aPlayer[0].pos += g_aPlayer[0].move;

	g_aPlayer[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================
//  描画処理
//==========================================
void DrawPlayer()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef; //現在のマテリアルの保存用
	D3DXMATERIAL *pMat; //マテリアルへのポインタ

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//各パーツの描画
			for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumModel; nCntParts++)
			{
				//計算用変数宣言
				D3DXMATRIX mtxRotModel, mtxTransModel;
				D3DXMATRIX mtxParent; //親マトリックス

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld);

				//向きの反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].aModel[nCntParts].rot.y, g_aPlayer[nCntPlayer].aModel[nCntParts].rot.x, g_aPlayer[nCntPlayer].aModel[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &mtxRot);

				//位置の反映
				D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].aModel[nCntParts].pos.x, g_aPlayer[nCntPlayer].aModel[nCntParts].pos.y, g_aPlayer[nCntPlayer].aModel[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &mtxTrans);

				//親マトリックスの設定
				if (g_aPlayer[nCntPlayer].aModel[nCntParts].nIdxModelParent != -1)
				{
					mtxParent = g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].aModel[nCntParts].nIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
				}

				//ワールドマトリックスと親マトリックスをかけ合わせる
				D3DXMatrixMultiply
				(
					&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld,
					&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld,
					&mtxParent
				);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].aModel[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].aModel[nCntParts].dwNumMat; nCntMat++)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_aPlayer[nCntPlayer].aModel[nCntParts].pTexture[nCntMat]);

					//モデルの描画
					g_aPlayer[nCntPlayer].aModel[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				//保存しているマテリアルを復元
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//==========================================
//  プレイヤー情報の取得
//==========================================
PLAYER *GetPlayerData()
{
	return &g_aPlayer[0];
}
