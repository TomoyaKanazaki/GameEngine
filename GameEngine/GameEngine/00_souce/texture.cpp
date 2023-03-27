//==========================================================
//
//�e�N�X�`������ [texture.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "texture.h"
#include "fileload.h"

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_apTexture[MAX_TEXTUREFILE] = {};			//�e�N�X�`���ւ̃|�C���^�z��

//==========================================================
//�e�N�X�`������������
//==========================================================
void InitTexture(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTexture = 0; nCntTexture < GetTextureCnt(); nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, pTextureFileName(nCntTexture), &g_apTexture[nCntTexture]);
	}
}

//==========================================================
//�e�N�X�`���I������
//==========================================================
void UninitTexture(void)
{
	//�e�N�X�`���̔p��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTUREFILE; nCntTexture++)
	{
		if (g_apTexture[nCntTexture] != NULL)
		{//�e�N�X�`���f�[�^�����݂��Ă���ꍇ
			g_apTexture[nCntTexture]->Release();
			g_apTexture[nCntTexture] = NULL;
		}
	}
}

//==========================================================
//�e�N�X�`���X�V����
//==========================================================
void UpdateTexture(void)
{

}

//==========================================================
//�w�肳�ꂽ�e�N�X�`������n��
//==========================================================
LPDIRECT3DTEXTURE9 *SetTexture(int nIdx)
{
	return &g_apTexture[nIdx];
}