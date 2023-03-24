//==========================================
//
//  ゲーム全般の制御(main.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <time.h>
#include "main.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "ui.h"

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3D9 g_pD3D = NULL; //Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL; //フォントへのポインタ
int g_nCountFPS = 0; //FPSカウンタ

#ifdef _DEBUG

#else

#endif //_DEBUG

//--------------------------------------------------
// 入力検知 (先輩譲渡)
//--------------------------------------------------
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//==========================================
//  メイン関数
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX), //WNDCLASSEXのメモリサイズ
		CS_CLASSDC, //ウィンドウのスタイル
		WindowsProc, //ウィンドウプロシージャ
		0, //0にする(通常は使用しない)
		0, //0にする(通常は使用しない)
		hInstance, //インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION), //タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW), //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1), //クライアントの背景色
		NULL, //メニューバー
		CLASS_NAME, //ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION), //ファイルのアイコン
	};
	HWND hWnd; //ウィンドウハンドル
	MSG msg; //メッセージを格納する変数
	DWORD dwCurrentTime; //現在時刻
	DWORD dwExecLastTime; //最後に処理した時刻
	DWORD dwFrameCount; //フレームカウント
	DWORD dwFPSLastTime; //最後にFPSを計測した時間

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx
	(
		0, //拡張ウィンドウスタイル
		CLASS_NAME, //ウィンドウクラスの名前
		WINDOW_NAME, //ウィンドウの名前
		WS_OVERLAPPEDWINDOW, //ウィンドウスタイル
		CW_USEDEFAULT, //ウィンドウの左上X座標
		CW_USEDEFAULT, //ウィンドウの左上Y座標
		(rect.right - rect.left), //ウィンドウの幅
		(rect.bottom - rect.top), //ウィンドウの高さ
		NULL, //新規ウィンドウのハンドル
		NULL, //メニューハンドルまたは子ウィンドウID
		hInstance, //インスタンスハンドル
		NULL
	);

	//初期化処理
#ifdef _DEBUG
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		//初期化処理に失敗した場合
		return -1;
	}
#else
	if (FAILED(Init(hInstance, hWnd, FALSE)))
	{
		//初期化処理に失敗した場合
		return -1;
	}
#endif

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0; //初期化
	dwExecLastTime = timeGetTime(); //現在時刻を取得

	//デバッグ表示用の数値の設定
	dwFrameCount = 0; //初期化
	dwFPSLastTime = timeGetTime(); //現在時刻を取得

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow); //ウィンドウの表示状態を設定
	UpdateWindow(hWnd); //クライアント領域

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//windowsの処理
			if (msg.message == WM_QUIT)
			{
				//WM_QUITメッセージを受け取った場合
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg); //仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg); //ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{
			//DirectXの処理
			dwCurrentTime = timeGetTime(); //現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				//0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime; //FPSを測定した時間
				dwFrameCount = 0; //フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 144))
			{
				//60分の1秒経過
				dwExecLastTime = dwCurrentTime;
				dwFrameCount++; //フレームカウントを加算

								//更新処理
				Update();
				//描画処理
				Draw();
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウのクラスの解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//==========================================
//  ウィンドウプロージャ
//==========================================
LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//imgui専用マウス処理
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	switch (uMsg)
	{
	case WM_DESTROY: //ウィンドウ破棄のメッセージ
		//WM_DUITのメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //キー押下メッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE: //キー押下メッセージ
		//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //既定の処理を返す
}

//==========================================
//  初期化処理
//==========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm; //ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp; //プレゼンテーションパラメータ

	//Direct3Dオブジェクト
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH; //ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; //ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format; //バックバッファの形式
	d3dpp.BackBufferCount = 1; //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE; //デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow; //ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //インターバル

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		if (FAILED(g_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{
			if (FAILED(g_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用フォントの生成
	D3DXCreateFont
	(
		g_pD3DDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont
	);

	//各種オブジェクトの初期化処理

	//乱数シードの設定
	srand((unsigned int)time(0));

	//テクスチャの初期化
	InitTexture();

	//カメラの初期化
	InitCamera();

	//ライトの初期化
	InitLight();

	//UIの初期化
	InitUi(hWnd);

	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void Uninit(void)
{
	//各種オブジェクトの終了処理

	//テクスチャの終了
	UninitTexture();

	//カメラの終了
	UninitCamera();

	//ライトの終了
	UninitLight();

	//UIの終了
	UninitUi();

	//デバッグ用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void Update(void)
{
	//カメラの更新
	UpdateCamera();

	//ライトの更新
	UpdateLight();

	//UIの更新
	UpdateUi();
}

//==========================================
//  描画処理
//==========================================
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0
	);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//カメラの設定
		SetCamera();

		//UIの描画
		DrawUi();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの切り替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==========================================
//  デバイスの取得
//==========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==========================================
//  フォントの取得
//==========================================
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}
