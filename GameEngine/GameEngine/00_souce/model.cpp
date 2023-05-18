//==========================================
//
//  ���f���̊Ǘ�(model.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "model.h"

//==========================================
//  �}�N����`
//==========================================
#define MIN_SIZE_DEFAULT (D3DXVECTOR3(99999.9f, 99999.9f, 99999.9f)) //�ŏ����_�̊�l
#define MAX_SIZE_DEFAULT (D3DXVECTOR3(-99999.9f, -99999.9f, -99999.9f)) //�ő咸�_�̊�l

//==========================================
//  ���f���t�@�C���p�X�ꗗ
//==========================================
const char *c_apModelPass[] = 
{
	"02_data/02_MODEL/00_Player.x", //�v���C���[
	"02_data/02_MODEL/01_Enemy.x" //�G
};

//==========================================
//  �v���g�^�C�v�錾
//==========================================
HRESULT LoadModel(void); //x�t�@�C���̓ǂݍ���
HRESULT LoadTexture(void); //�e�N�X�`���̓ǂݍ���
void CalcModelSize(void); //���f���T�C�Y�̌v��

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
MODEL g_aModel[MODELTYPE_MAX]; //���f�����

//==========================================
//  ����������
//==========================================
HRESULT InitModel()
{
	//�ϐ��̏�����
	ZeroMemory(&g_aModel[0], sizeof(MODEL) * MODELTYPE_MAX);
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		g_aModel[nCntModel].vtxMax = MAX_SIZE_DEFAULT;
		g_aModel[nCntModel].vtxMin = MIN_SIZE_DEFAULT;
	}

	//x�t�@�C���̓ǂݍ���
	if (FAILED(LoadModel()))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	if (FAILED(LoadTexture()))
	{
		return E_FAIL;
	}

	//���f���T�C�Y�̌v��
	CalcModelSize();

	return S_OK;
}

//==========================================
//  �I������
//==========================================
void UninitModel()
{
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//�e�N�X�`���̔j��
		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (g_aModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_aModel[nCntModel].pTexture[nCntMat]->Release();
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		//�������̊J��
		free(g_aModel[nCntModel].pTexture);

		//���_�o�b�t�@�̔j��
		if (g_aModel[nCntModel].pMesh != NULL)
		{
			g_aModel[nCntModel].pMesh->Release();
			g_aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aModel[nCntModel].pBuffMat != NULL)
		{
			g_aModel[nCntModel].pBuffMat->Release();
			g_aModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//==========================================
//  x�t�@�C���̓ǂݍ���
//==========================================
HRESULT LoadModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//�t�@�C���̓ǂݍ���
		HRESULT hr = D3DXLoadMeshFromX
		(
			c_apModelPass[nCntModel], //���f���p�X
			D3DXMESH_SYSTEMMEM, //���_��񐶐��I�v�V����
			pDevice, //�f�o�C�X�ւ̃|�C���^
			NULL, //�אڐ��f�[�^
			&g_aModel[nCntModel].pBuffMat, //�}�e���A���ւ̃|�C���^
			NULL, //�G�t�F�N�g�f�[�^
			&g_aModel[nCntModel].dwNumMat, //�}�e���A����
			&g_aModel[nCntModel].pMesh //���_���̃|�C���^
		);

		//�G���[���b�Z�[�W
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to load x file", "Error", MB_ICONWARNING);
			return E_FAIL;
		}

		//�g�p�����A�h���X���擾
		g_aModel[nCntModel].pTexture = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9*) * g_aModel[nCntModel].dwNumMat);

		//NULL�`�F�b�N
		if (g_aModel[nCntModel].pTexture == NULL)
		{
			//�G���[���b�Z�[�W
			MessageBox(NULL, "Failed to allocate memory", "Error", MB_ICONWARNING);

			//�m�ۂɐ����������̃��������J������
			for (int nCntFree = 0; nCntFree < nCntModel; nCntFree++)
			{
				free(g_aModel[nCntFree].pTexture);
			}
			return E_FAIL;
		}
	}

	return S_OK;
}

//==========================================
//  �e�N�X�`���̓ǂݍ���
//==========================================
HRESULT LoadTexture(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//�}�e���A�����̎擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pBuffMat->GetBufferPointer();

		//�e�N�X�`���ǂݍ���
		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModel[nCntModel].pTexture[nCntMat])))
				{
					//�G���[���b�Z�[�W
					MessageBox(NULL, "Failure to load textures", "Error�I", MB_ICONWARNING);
					return E_FAIL;
				}
			}
			else
			{
				g_aModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}
	}

	return S_OK;
}

//==========================================
//  ���f���T�C�Y�̌v��
//==========================================
void CalcModelSize(void)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 vtx; //�v�Z�p�ϐ�

	//���_�o�b�t�@�̐���
	BYTE *pVtxBuff;

	//�T�C�Y���v��
	for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//�v�Z�p�ϐ��錾
		int nNumVtx = g_aModel[nCntModel].pMesh->GetNumVertices(); //���_���̎擾
		DWORD dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCntModel].pMesh->GetFVF()); //���_�t�H�[�}�b�g�T�C�Y�̎擾

		//���_�o�b�t�@�����b�N
		g_aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���̎擾
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			//�ŏ��A�ő�̒��_���Z�o
			if (vtx.x < g_aModel[nCntModel].vtxMin.x)
			{
				g_aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.x > g_aModel[nCntModel].vtxMax.x)
			{
				g_aModel[nCntModel].vtxMax.x = vtx.x;
			}
			if (vtx.y < g_aModel[nCntModel].vtxMin.y)
			{
				g_aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.y > g_aModel[nCntModel].vtxMax.y)
			{
				g_aModel[nCntModel].vtxMax.y = vtx.y;
			}
			if (vtx.z < g_aModel[nCntModel].vtxMin.z)
			{
				g_aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.z > g_aModel[nCntModel].vtxMax.z)
			{
				g_aModel[nCntModel].vtxMax.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;
		}

		//���f���̒��_�o�b�t�@���A�����b�N
		g_aModel[nCntModel].pMesh->UnlockVertexBuffer();

		//���f���T�C�Y���Z�o
		D3DXVECTOR3 size = g_aModel[nCntModel].vtxMax - g_aModel[nCntModel].vtxMin;

		// ���f���̍������Z�o
		g_aModel[nCntModel].fHeight = size.y;

		// ���f�����a���Z�o
		g_aModel[nCntModel].fRadius = ((size.x * 0.5f) + (size.z * 0.5f)) * 0.5f;
	}
}

//==========================================
//  ���f�����̎擾
//==========================================
MODEL *GetModelData(int nIdx)
{
	return &g_aModel[nIdx];
}
