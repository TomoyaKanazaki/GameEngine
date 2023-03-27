//==========================================================
//
//�O���t�@�C���ǂݍ��ݏ��� [fileload.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

//==========================================================
//�}�N����`
//==========================================================
//�m�F����
#define START_SCRIPTTXT	"SCRIPT"		//�X�N���v�g�J�n�m�F����
#define END_SCRIPTTXT	"END_SCRIPT"	//�X�N���v�g�I���m�F����
#define TEXTURENUM_TXT	"NUM_TEXTURE"	//�e�N�X�`���g�p�����m�F����
#define TEXTUREFILENAME_TXT	"TEXTURE_FILENAME"	//�e�N�X�`���t�@�C�����m�F����

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitFileLoad(void);
void UninitFileLoad(void);

//�e�ǂݍ��݃t�@�C����
void OpenLoadTxtFile(const char *pFileName);
int GetTextureCnt(void);
const char *pTextureFileName(int nCntTxture);

#endif