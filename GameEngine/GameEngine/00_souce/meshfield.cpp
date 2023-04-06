//==========================================
//
//  ���b�V���t�B�[���h(meshfield.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshfield.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  �}�N����`
//==========================================
#define NUMMESH (D3DXVECTOR2(3, 2)) //������
#define NUMPOINT_U (int)(NUMMESH.x + 1.0f) //�������̒��_��
#define NUMPOINT_V (int)(NUMMESH.y + 1.0f) //�c�����̒��_��
#define NUMPOINT (int)(NUMPOINT_U * NUMPOINT_V) //�S�̂̒��_��
#define NUMINDEX (int)((NUMPOINT * 2) - (2 * (abs((int)NUMMESH.x - (int)NUMMESH.y)) + 4)) //�C���f�b�N�X��
#define NUMPOLYGON (int)(NUMINDEX - 2) //��������|���S����

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	int nShadowNum;
}MeshField;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMEshField = NULL;
MeshField g_aMeshField; //���̏��

//==========================================
//  ����������
//==========================================
void InitMeshField(void)
{
	//�e��ϐ��̏�����
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nShadowNum = -1;

	//�|���S���̏�����
	g_pVtxBuffMeshField = Init_3D_Polygon(NUMPOLYGON);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(-100, -0.01f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(100.0f, -0.01f, 300.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);

	pVtx[4].pos = D3DXVECTOR3(-300.0f, -0.01f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(-100, -0.01f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(100.0f, -0.01f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(300.0f, -0.01f, 0.0f);

	pVtx[8].pos = D3DXVECTOR3(-300.0f, -0.01f, -300.0f);
	pVtx[9].pos = D3DXVECTOR3(-100, -0.01f, -300.0f);
	pVtx[10].pos = D3DXVECTOR3(100.0f, -0.01f, -300.0f);
	pVtx[11].pos = D3DXVECTOR3(300.0f, -0.01f, -300.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * NUMINDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMEshField,
		NULL
	);

	//�C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMEshField->Lock(0, 0, (void**)&pIdx, 0);

	//���_�f�[�^�̐ݒ�
	for (int nCntIdx = 0, nCntTurn = 0, nCntPoint = 0; nCntIdx < NUMINDEX; nCntIdx += 2)
	{
		if (nCntIdx == (2 * ((NUMPOINT_U * nCntTurn) + NUMPOINT_U + nCntTurn))) //�܂�Ԃ��n�_
		{
			pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			pIdx[nCntIdx + 1] = nCntPoint + NUMPOINT_U;
			nCntTurn++;
		}
		else //���̑�
		{
			pIdx[nCntIdx] = nCntPoint + NUMPOINT_U;
			pIdx[nCntIdx + 1] = nCntPoint;
			nCntPoint++;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMEshField->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMeshField(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffMeshField);

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMEshField != NULL)
	{
		g_pIdxBuffMEshField->Release();
		g_pIdxBuffMEshField = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateMeshField(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshField.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMEshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, *SetTexture(0));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		NUMPOINT, //���_��
		0,
		NUMPOLYGON //�v���~�e�B�u��
	);
}
