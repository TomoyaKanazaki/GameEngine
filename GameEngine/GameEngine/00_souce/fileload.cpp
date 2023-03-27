//==========================================================
//
//外部ファイル読み込み処理 [fileload.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "fileload.h"
#include <stdio.h>
#include <string.h>
#include "texture.h"

//==========================================================
//マクロ定義
//==========================================================
#define MAX_FILESTRING	(256)	//ファイルの文字最大数
#define TEXTURE_TXTFILENAME		"data\\TXT\\texture.txt"	//テクスチャ読み込み用txtファイル

//==========================================================
//プロトタイプ宣言
//==========================================================
void LoadTxtFileData(FILE *pFile);
void LoadTextureNum(FILE *pFile);
void LoadTextureFileName(FILE *pFile);

//==========================================================
//グローバル変数宣言
//==========================================================
char g_aTextureFileName[MAX_TEXTUREFILE][MAX_FILESTRING];	//テクスチャデータのファイル名格納用(MAX_TEXTUREFILEはtexture.h)
int g_nTextureNum;		//読み込んだテクスチャ数を保持

//==========================================================
//ファイル読み込み初期化処理
//==========================================================
void InitFileLoad(void)
{
	//テクスチャファイル名初期化
	memset(&g_aTextureFileName, '\0', sizeof(g_aTextureFileName));

	//読み込み数初期化
	g_nTextureNum = 0;		//テクスチャ数

	//テキストファイル読み込み
	OpenLoadTxtFile(TEXTURE_TXTFILENAME);
}

//==========================================================
//ファイル読み込み終了処理
//==========================================================
void UninitFileLoad(void)
{

}

//==========================================================
//指定されたテキストファイルを開く
//==========================================================
void OpenLoadTxtFile(const char *pFileName)
{
	FILE *pFile;

	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		char aStr[256];

		//開始文字まで読み込む
		while (1)
		{
			//テキスト読み込み
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], START_SCRIPTTXT) == 0)
			{//スクリプト開始の文字が確認できた場合
			 //各データの読み込み開始
				LoadTxtFileData(pFile);

				break;
			}
			else if (nResult == EOF)
			{//ファイルの最後まで読み込んでしまった場合
				break;
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		return;
	}

}

//==========================================================
//ファイル内容読み込み処理
//==========================================================
void LoadTxtFileData(FILE *pFile)
{
	char aStr[256] = {};	//余分な文章読み込み用

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		//読み込み内容確認
		if (strcmp(&aStr[0], TEXTURENUM_TXT) == 0)
		{//使用するテクスチャ枚数の場合
			LoadTextureNum(pFile);
		}

		//ファイル名確認
		if (strcmp(&aStr[0], TEXTUREFILENAME_TXT) == 0)
		{//テクスチャファイル名の場合
			LoadTextureFileName(pFile);
		}

		//終了確認
		if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//スクリプト開始の文字が確認できた場合
			break;
		}
		else if (nResult == EOF)
		{//ファイルの最後まで読み込んでしまった場合
			break;
		}
	}
}

//==========================================================
//テクスチャ枚数取得
//==========================================================
void LoadTextureNum(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%d", &g_nTextureNum);	//使用枚数読み込み
}

//==========================================================
//テクスチャファイル名取得
//==========================================================
void LoadTextureFileName(FILE *pFile)
{
	char aStr[4] = {};	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み

	for (int nCntTexture = 0; nCntTexture < g_nTextureNum; nCntTexture++)
	{
		if (g_aTextureFileName[nCntTexture][0] == '\0')
		{//ファイル名を入れる場所が使われていない場合
			fscanf(pFile, "%s", &g_aTextureFileName[nCntTexture][0]);	//(=)読み込み
			break;
		}
	}
}

//==========================================================
//テクスチャ読み込み数取得
//==========================================================
int GetTextureCnt(void)
{
	return g_nTextureNum;
}

//==========================================================
//テクスチャファイル名取得
//==========================================================
const char *pTextureFileName(int nCntTxture)
{
	return &g_aTextureFileName[nCntTxture][0];
}