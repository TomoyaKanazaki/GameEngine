//==========================================
//
//  �J�����̐���(camera.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

//==========================================
//  �J�����\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 posV; //���_
	D3DXVECTOR3 posR; //�����_
	D3DXVECTOR3 vecU; //������x�N�g��
	D3DXMATRIX mtxProjection; //�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView; //�r���[�}�g���b�N�X
}Camera;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCameraData(void);

#endif