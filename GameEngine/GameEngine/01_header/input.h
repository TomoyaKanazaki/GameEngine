//==========================================
//
//  入力の制御に特化したファイル(input.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define NUM_KEY_MAX	(256) //キーの最大値

//==========================================
//  列挙型定義
//==========================================
//ジョイパッドボタン
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

//ジョイパッド十字キー
typedef enum
{
	CROSS_UP = 0, //上方向
	CROSS_RIGHT = 9000, //右方向
	CROSS_DOWN = 18000, //下方向
	CROSS_LEFT = 27000, //左方向
	CROSS_NONE = -1 //入力がない状態の情報
}JOYKEY_CROSS;

//ジョイスティック方向
typedef enum
{
	STICK_UP = 180, //上方向
	STICK_DOWN = 0, //下方向
	STICK_RIGHT = 90, //右方向
	STICK_LEFT = -90 //左方向
}JOYKEY_STICK;

//マウス
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
//  プロトタイプ宣言
//==========================================
//デバイス統括
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd);
void UninitDevice(void);
void UpdateDevice(void);

//キーボード================================
HRESULT InitKeyboard(HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

//情報の取得
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
//==========================================

//ジョイパッド==============================
HRESULT InitJoyPad(HWND hWnd);
void UninitJoyPad(void);
void UpdateJoyPad(void);

//情報の取得
bool GetJoyPadButtonPress(JOYKEY_BUTTON nKey,int nPlayer);
bool GetJoyPadButtonTrigger(JOYKEY_BUTTON nKey, int nPlayer);
bool GetJoyPadCrossPress(int nDirection, int nPlayer);
bool GetJoyPadCrossTrigger(int nDirection, int nPlayer);
bool GetJoyPadCrossNone(int nPlayer);
D3DXVECTOR3 GetStickL(int nPlayer);
D3DXVECTOR3 GetStickR(int nPlayer);
bool GetStickTriggerL(int nDirection, int nPlayer);
bool GetStickTriggerR(int nDirection, int nPlayer);

//判定
bool KnockStickL(DIJOYSTATE JoyKey, int nDead, int nPlayer); //左スティックの入力判定
bool KnockStickR(DIJOYSTATE JoyKey, int nDead, int nPlayer); //右スティックの入力判定
//==========================================

//マウス====================================
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

//情報の取得
bool GetMousePress(MOUSE_BUTTON nKey);
bool GetMouseTrigger(MOUSE_BUTTON nKey);
D3DXVECTOR3 GetMouseMove(void);
bool GetMouseControll(void);
//==========================================

#endif
