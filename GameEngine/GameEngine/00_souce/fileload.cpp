//==========================================================
//
//�O���t�@�C���ǂݍ��ݏ��� [fileload.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "fileload.h"
#include <stdio.h>
#include <string.h>
#include "texture.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_FILESTRING	(256)	//�t�@�C���̕����ő吔
#define TEXTURE_TXTFILENAME		"data\\TXT\\texture.txt"	//�e�N�X�`���ǂݍ��ݗptxt�t�@�C��

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void LoadTxtFileData(FILE *pFile);
void LoadTextureNum(FILE *pFile);
void LoadTextureFileName(FILE *pFile);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
char g_aTextureFileName[MAX_TEXTUREFILE][MAX_FILESTRING];	//�e�N�X�`���f�[�^�̃t�@�C�����i�[�p(MAX_TEXTUREFILE��texture.h)
int g_nTextureNum;		//�ǂݍ��񂾃e�N�X�`������ێ�

//==========================================================
//�t�@�C���ǂݍ��ݏ���������
//==========================================================
void InitFileLoad(void)
{
	//�e�N�X�`���t�@�C����������
	memset(&g_aTextureFileName, '\0', sizeof(g_aTextureFileName));

	//�ǂݍ��ݐ�������
	g_nTextureNum = 0;		//�e�N�X�`����

	//�e�L�X�g�t�@�C���ǂݍ���
	OpenLoadTxtFile(TEXTURE_TXTFILENAME);
}

//==========================================================
//�t�@�C���ǂݍ��ݏI������
//==========================================================
void UninitFileLoad(void)
{

}

//==========================================================
//�w�肳�ꂽ�e�L�X�g�t�@�C�����J��
//==========================================================
void OpenLoadTxtFile(const char *pFileName)
{
	FILE *pFile;

	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		char aStr[256];

		//�J�n�����܂œǂݍ���
		while (1)
		{
			//�e�L�X�g�ǂݍ���
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], START_SCRIPTTXT) == 0)
			{//�X�N���v�g�J�n�̕������m�F�ł����ꍇ
			 //�e�f�[�^�̓ǂݍ��݊J�n
				LoadTxtFileData(pFile);

				break;
			}
			else if (nResult == EOF)
			{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
				break;
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

}

//==========================================================
//�t�@�C�����e�ǂݍ��ݏ���
//==========================================================
void LoadTxtFileData(FILE *pFile)
{
	char aStr[256] = {};	//�]���ȕ��͓ǂݍ��ݗp

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		//�ǂݍ��ݓ��e�m�F
		if (strcmp(&aStr[0], TEXTURENUM_TXT) == 0)
		{//�g�p����e�N�X�`�������̏ꍇ
			LoadTextureNum(pFile);
		}

		//�t�@�C�����m�F
		if (strcmp(&aStr[0], TEXTUREFILENAME_TXT) == 0)
		{//�e�N�X�`���t�@�C�����̏ꍇ
			LoadTextureFileName(pFile);
		}

		//�I���m�F
		if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//�X�N���v�g�J�n�̕������m�F�ł����ꍇ
			break;
		}
		else if (nResult == EOF)
		{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
			break;
		}
	}
}

//==========================================================
//�e�N�X�`�������擾
//==========================================================
void LoadTextureNum(FILE *pFile)
{
	char aStr[4];	//�]���ȕ��͓ǂݍ��ݗp

	fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
	fscanf(pFile, "%d", &g_nTextureNum);	//�g�p�����ǂݍ���
}

//==========================================================
//�e�N�X�`���t�@�C�����擾
//==========================================================
void LoadTextureFileName(FILE *pFile)
{
	char aStr[4] = {};	//�]���ȕ��͓ǂݍ��ݗp

	fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���

	for (int nCntTexture = 0; nCntTexture < g_nTextureNum; nCntTexture++)
	{
		if (g_aTextureFileName[nCntTexture][0] == '\0')
		{//�t�@�C����������ꏊ���g���Ă��Ȃ��ꍇ
			fscanf(pFile, "%s", &g_aTextureFileName[nCntTexture][0]);	//(=)�ǂݍ���
			break;
		}
	}
}

//==========================================================
//�e�N�X�`���ǂݍ��ݐ��擾
//==========================================================
int GetTextureCnt(void)
{
	return g_nTextureNum;
}

//==========================================================
//�e�N�X�`���t�@�C�����擾
//==========================================================
const char *pTextureFileName(int nCntTxture)
{
	return &g_aTextureFileName[nCntTxture][0];
}