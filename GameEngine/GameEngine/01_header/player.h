//==========================================
//
//  �v���C���[�̏���(player.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"
#include "model.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_PLAYERMODEL (32) //�v���C���[�Ɏg�p���郂�f���̍ő吔

//==========================================
//  �v���C���[�\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	D3DXVECTOR3 posOld; //�O����W
	D3DXVECTOR3 rot; //�p�x
	D3DXVECTOR3 move; //�ړ���
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	MODEL aModel[MAX_PLAYERMODEL]; //�g�p���郂�f��
	int nNumModel; //�g�p���Ă��郂�f���̑���
	bool bUse;
}PLAYER;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayerData(void);

#endif
