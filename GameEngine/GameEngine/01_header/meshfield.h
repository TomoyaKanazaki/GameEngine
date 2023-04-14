//==========================================
//
//  ���b�V���t�B�[���h(meshfield.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include "main.h"

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	int nNumMesh; //��������(�v���~�e�B�u��)
	int nNumMesh_U; //���̕�����
	int nNumMesh_V; //�c�̕�����
	int nNumVtx; //�����_��
	int nNumVtx_U; //���̒��_��
	int nNumVtx_V; //�c�̒��_��
	int nNumIdx; //���C���f�b�N�X��
	D3DXVECTOR2 size; //�S�̂̑傫��
}MeshData; //���_���n�̍\����

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	MeshData data;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
}Mesh; //�S���b�V���t�B�[���h���̍\����

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void ResetMeshField(void);
Mesh *GetMeshField(void);
LPDIRECT3DVERTEXBUFFER9 GetVtxData(void);
void SaveMeshField(void);

#endif
