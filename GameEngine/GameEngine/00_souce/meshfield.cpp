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
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL; //���_�o�b�t�@
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL; //�C���f�b�N�X�o�b�t�@
Mesh g_aMeshField; //���̏��

//==========================================
//  ����������
//==========================================
void InitMeshField(void)
{
	//�e��ϐ��̏�����
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.data.size = D3DXVECTOR2(500.0f, 500.0f);
	g_aMeshField.data.nNumMesh_U = 4;
	g_aMeshField.data.nNumMesh_V = 4;

	//���_���𐶐�
	ResetMeshField();
}

//==========================================
//  �I������
//==========================================
void UninitMeshField(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffMeshField);

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
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
	pDevice->SetIndices(g_pIdxBuffMeshField);

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
		g_aMeshField.data.nNumVtx, //���_��
		0,
		g_aMeshField.data.nNumMesh //�v���~�e�B�u��
	);
}

//==========================================
//  ���b�V���t�B�[���h�̍Đݒ�
//==========================================
void ResetMeshField()
{
	//���_�����p�ϐ��̐ݒ�
	g_aMeshField.data.nNumVtx_U = g_aMeshField.data.nNumMesh_U + 1;
	g_aMeshField.data.nNumVtx_V = g_aMeshField.data.nNumMesh_V + 1;
	g_aMeshField.data.nNumVtx = g_aMeshField.data.nNumVtx_U * g_aMeshField.data.nNumVtx_V;
	g_aMeshField.data.nNumIdx = (g_aMeshField.data.nNumVtx * 2) - (2 * (abs(g_aMeshField.data.nNumMesh_U - g_aMeshField.data.nNumMesh_V)) + 4);
	g_aMeshField.data.nNumMesh = g_aMeshField.data.nNumIdx - 2;

	//�|���S���̏�����
	g_pVtxBuffMeshField = Init_3D_Polygon(g_aMeshField.data.nNumMesh);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntVtxU = 0; nCntVtxU < g_aMeshField.data.nNumVtx_U; nCntVtxU++)
	{
		for (int nCntVtxV = 0; nCntVtxV < g_aMeshField.data.nNumVtx_V; nCntVtxV++)
		{
			pVtx[nCntVtxU + (nCntVtxV * g_aMeshField.data.nNumVtx_U)].pos = D3DXVECTOR3
			(
				-(g_aMeshField.data.size.x * 0.5f) + ((g_aMeshField.data.size.x / g_aMeshField.data.nNumMesh_U) * nCntVtxU),
				0.0f,
				(g_aMeshField.data.size.y * 0.5f) - ((g_aMeshField.data.size.y / g_aMeshField.data.nNumMesh_V) * nCntVtxV)
			);
		}
	}

	for (int nCntVtx = 0; nCntVtx < g_aMeshField.data.nNumVtx; nCntVtx++)
	{
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * g_aMeshField.data.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL
	);

	//�C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//���_�f�[�^�̐ݒ�
	for (int nCntIdx = 0, nCntTurn = 0, nCntVtx = 0; nCntIdx < g_aMeshField.data.nNumIdx; nCntIdx += 2)
	{
		if (nCntIdx == (2 * ((g_aMeshField.data.nNumVtx_U * nCntTurn) + g_aMeshField.data.nNumVtx_U + nCntTurn))) //�܂�Ԃ��n�_
		{
			pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			pIdx[nCntIdx + 1] = nCntVtx + g_aMeshField.data.nNumVtx_U;
			nCntTurn++;
		}
		else //���̑�
		{
			pIdx[nCntIdx] = nCntVtx + g_aMeshField.data.nNumVtx_U;
			pIdx[nCntIdx + 1] = nCntVtx;
			nCntVtx++;
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshField->Unlock();
}

//==========================================
//  �t�B�[���h���̎擾
//==========================================
Mesh *GetMeshField()
{
	return &g_aMeshField;
}

//==========================================
//  ���_���̎擾
//==========================================
LPDIRECT3DVERTEXBUFFER9 GetVtxData(void)
{
	return g_pVtxBuffMeshField;
}

//==========================================
//  ���b�V�����̃Z�[�u
//==========================================
void SaveMeshField()
{

}
