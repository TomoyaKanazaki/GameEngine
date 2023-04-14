//==========================================
//
//  メッシュフィールド(meshfield.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include "main.h"

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	int nNumMesh; //総分割数(プリミティブ数)
	int nNumMesh_U; //横の分割数
	int nNumMesh_V; //縦の分割数
	int nNumVtx; //総頂点数
	int nNumVtx_U; //横の頂点数
	int nNumVtx_V; //縦の頂点数
	int nNumIdx; //総インデックス数
	D3DXVECTOR2 size; //全体の大きさ
}MeshData; //頂点情報系の構造体

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	MeshData data;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
}Mesh; //全メッシュフィールド情報の構造体

//==========================================
//  プロトタイプ宣言
//==========================================
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void ResetMeshField(void);
Mesh *GetMeshField(void);
LPDIRECT3DVERTEXBUFFER9 GetVtxData(void);
void SaveMeshField(void);

#endif
