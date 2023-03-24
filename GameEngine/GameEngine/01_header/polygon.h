//==========================================
//
//  ポリゴンの汎用処理(polygon.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _POLYGON_H_
#define _POLYGON_H_
#include "main.h"

//==========================================
//  プロトタイプ宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 Init_3D_Polygon(int nNum = 1);
LPDIRECT3DVERTEXBUFFER9 Init_2D_Polygon(int nNum = 1);
void UninitPolygon(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff);

#endif
