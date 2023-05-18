//==========================================
//
//  �v���C���[�̏���(player.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "player.h"
#include "model.h"
#include "input.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_PLAYER (4) //�v���C���[�̍ő吔

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
PLAYER g_aPlayer[MAX_PLAYER]; //�v���C���[���

//==========================================
//  ����������
//==========================================
void InitPlayer()
{
	//�ϐ��̏�����
	ZeroMemory(&g_aPlayer[0], sizeof(PLAYER) * MAX_PLAYER);

	//���f���̐ݒ�
	g_aPlayer[0].nNumModel = 2;
	g_aPlayer[0].aModel[0] = *GetModelData(MODELTYPE_PLAYER);
	g_aPlayer[0].aModel[1] = *GetModelData(MODELTYPE_ENEMY);

	//�e�p�[�c�̊K�w�\���ݒ�
	g_aPlayer[0].aModel[0].nIdxModelParent = -1;
	g_aPlayer[0].aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer[0].aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_aPlayer[0].aModel[1].nIdxModelParent = 0;
	g_aPlayer[0].aModel[1].pos = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	g_aPlayer[0].aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_aPlayer[0].bUse = true;
}

//==========================================
//  �I������
//==========================================
void UninitPlayer()
{

}

//==========================================
//  �X�V����
//==========================================
void UpdatePlayer()
{
	if (GetKeyboardPress(DIK_W))
	{
		g_aPlayer[0].move.z = 10;
	}
	if (GetKeyboardPress(DIK_S))
	{
		g_aPlayer[0].move.z = -10;
	}
	if (GetKeyboardPress(DIK_A))
	{
		g_aPlayer[0].move.x = -10;
	}
	if (GetKeyboardPress(DIK_D))
	{
		g_aPlayer[0].move.x = 10;
	}

	g_aPlayer[0].pos += g_aPlayer[0].move;

	g_aPlayer[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================
//  �`�揈��
//==========================================
void DrawPlayer()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef; //���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat; //�}�e���A���ւ̃|�C���^

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (g_aPlayer[nCntPlayer].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].mtxWorld);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].rot.y, g_aPlayer[nCntPlayer].rot.x, g_aPlayer[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].pos.x, g_aPlayer[nCntPlayer].pos.y, g_aPlayer[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].mtxWorld, &g_aPlayer[nCntPlayer].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�e�p�[�c�̕`��
			for (int nCntParts = 0; nCntParts < g_aPlayer[nCntPlayer].nNumModel; nCntParts++)
			{
				//�v�Z�p�ϐ��錾
				D3DXMATRIX mtxRotModel, mtxTransModel;
				D3DXMATRIX mtxParent; //�e�}�g���b�N�X

				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld);

				//�����̔��f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[nCntPlayer].aModel[nCntParts].rot.y, g_aPlayer[nCntPlayer].aModel[nCntParts].rot.x, g_aPlayer[nCntPlayer].aModel[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &mtxRot);

				//�ʒu�̔��f
				D3DXMatrixTranslation(&mtxTrans, g_aPlayer[nCntPlayer].aModel[nCntParts].pos.x, g_aPlayer[nCntPlayer].aModel[nCntParts].pos.y, g_aPlayer[nCntPlayer].aModel[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld, &mtxTrans);

				//�e�}�g���b�N�X�̐ݒ�
				if (g_aPlayer[nCntPlayer].aModel[nCntParts].nIdxModelParent != -1)
				{
					mtxParent = g_aPlayer[nCntPlayer].aModel[g_aPlayer[nCntPlayer].aModel[nCntParts].nIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_aPlayer[nCntPlayer].mtxWorld;
				}

				//���[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���������킹��
				D3DXMatrixMultiply
				(
					&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld,
					&g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld,
					&mtxParent
				);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[nCntPlayer].aModel[nCntParts].mtxWorld);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aPlayer[nCntPlayer].aModel[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_aPlayer[nCntPlayer].aModel[nCntParts].dwNumMat; nCntMat++)
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aPlayer[nCntPlayer].aModel[nCntParts].pTexture[nCntMat]);

					//���f���̕`��
					g_aPlayer[nCntPlayer].aModel[nCntParts].pMesh->DrawSubset(nCntMat);
				}

				//�ۑ����Ă���}�e���A���𕜌�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//==========================================
//  �v���C���[���̎擾
//==========================================
PLAYER *GetPlayerData()
{
	return &g_aPlayer[0];
}
