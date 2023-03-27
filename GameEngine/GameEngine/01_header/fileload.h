//==========================================================
//
//外部ファイル読み込み処理 [fileload.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

//==========================================================
//マクロ定義
//==========================================================
//確認文字
#define START_SCRIPTTXT	"SCRIPT"		//スクリプト開始確認文字
#define END_SCRIPTTXT	"END_SCRIPT"	//スクリプト終了確認文字
#define TEXTURENUM_TXT	"NUM_TEXTURE"	//テクスチャ使用枚数確認文字
#define TEXTUREFILENAME_TXT	"TEXTURE_FILENAME"	//テクスチャファイル名確認文字

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitFileLoad(void);
void UninitFileLoad(void);

//各読み込みファイル別
void OpenLoadTxtFile(const char *pFileName);
int GetTextureCnt(void);
const char *pTextureFileName(int nCntTxture);

#endif