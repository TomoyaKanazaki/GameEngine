//==========================================
//
//  ���f���̊Ǘ�(model.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MODEL_H_
#define _MODEL_H_
#include "main.h"

//==========================================
//  ���f���^�C�v�̗񋓌^��`
//==========================================
typedef enum
{
	MODELTYPE_PLAYER = 0, //�v���C���[�̃��f��
	MODELTYPE_ENEMY, //�G�̃��f��
	MODELTYPE_MAX
}MODEL_TYPE;

//==========================================
//  ���f���\���̒�`
//==========================================
typedef struct
{
	LPDIRECT3DTEXTURE9 *pTexture; //�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH pMesh; //���_���̃|�C���^
	LPD3DXBUFFER pBuffMat; //�}�e���A�����̃|�C���^
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	DWORD dwNumMat; //�}�e���A����
	D3DXVECTOR3 vtxMax; //���_�̍ő�l
	D3DXVECTOR3 vtxMin; //���_�̍ŏ��l
	D3DXVECTOR3 pos; //�ʒu
	D3DXVECTOR3 rot; //�p�x
	float fHeight; //���f���̍���
	float fRadius; //���f���̔��a
	int nIdxModelParent; //�e���f���̃C���f�b�N�X
}MODEL;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
HRESULT InitModel(void); //�ǂݍ��݁A������
void UninitModel(void); //�j��
MODEL *GetModelData(int nIdx); //���f�����̎擾

#endif
