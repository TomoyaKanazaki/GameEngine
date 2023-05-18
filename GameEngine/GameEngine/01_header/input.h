//==========================================
//
//  ���͂̐���ɓ��������t�@�C��(input.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define NUM_KEY_MAX	(256) //�L�[�̍ő�l

//==========================================
//  �񋓌^��`
//==========================================
//�W���C�p�b�h�{�^��
typedef enum
{
	KEY_X = 0,
	KEY_Y,
	KEY_A,
	KEY_B,
	KEY_LB,
	KEY_RB,
	KEY_LT,
	KEY_RT,
	KEY_L3,
	KEY_R3,
	KEY_BACK,
	KEY_START,
	KEY_MAX
}JOYKEY_BUTTON;

//�W���C�p�b�h�\���L�[
typedef enum
{
	CROSS_UP = 0, //�����
	CROSS_RIGHT = 9000, //�E����
	CROSS_DOWN = 18000, //������
	CROSS_LEFT = 27000, //������
	CROSS_NONE = -1 //���͂��Ȃ���Ԃ̏��
}JOYKEY_CROSS;

//�W���C�X�e�B�b�N����
typedef enum
{
	STICK_UP = 180, //�����
	STICK_DOWN = 0, //������
	STICK_RIGHT = 90, //�E����
	STICK_LEFT = -90 //������
}JOYKEY_STICK;

//�}�E�X
typedef enum
{
	MOUSEBUTTON_LEFT = 0,
	MOUSEBUTTON_RIGHT,
	MOUSEBUTTON_WHEEL,
	MOUSEBUTTON_SIDE1,
	MOUSEBUTTON_SIDE2,
	BUTTON_MAX
}MOUSE_BUTTON;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
//�f�o�C�X����
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd);
void UninitDevice(void);
void UpdateDevice(void);

//�L�[�{�[�h================================
HRESULT InitKeyboard(HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

//���̎擾
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
//==========================================

//�W���C�p�b�h==============================
HRESULT InitJoyPad(HWND hWnd);
void UninitJoyPad(void);
void UpdateJoyPad(void);

//���̎擾
bool GetJoyPadButtonPress(JOYKEY_BUTTON nKey,int nPlayer);
bool GetJoyPadButtonTrigger(JOYKEY_BUTTON nKey, int nPlayer);
bool GetJoyPadCrossPress(int nDirection, int nPlayer);
bool GetJoyPadCrossTrigger(int nDirection, int nPlayer);
bool GetJoyPadCrossNone(int nPlayer);
D3DXVECTOR3 GetStickL(int nPlayer);
D3DXVECTOR3 GetStickR(int nPlayer);
bool GetStickTriggerL(int nDirection, int nPlayer);
bool GetStickTriggerR(int nDirection, int nPlayer);

//����
bool KnockStickL(DIJOYSTATE JoyKey, int nDead, int nPlayer); //���X�e�B�b�N�̓��͔���
bool KnockStickR(DIJOYSTATE JoyKey, int nDead, int nPlayer); //�E�X�e�B�b�N�̓��͔���
//==========================================

//�}�E�X====================================
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

//���̎擾
bool GetMousePress(MOUSE_BUTTON nKey);
bool GetMouseTrigger(MOUSE_BUTTON nKey);
D3DXVECTOR3 GetMouseMove(void);
bool GetMouseControll(void);
//==========================================

#endif
