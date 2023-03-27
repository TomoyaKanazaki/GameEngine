//==========================================
//
//  UI表示の制御(ui.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <string.h>
#include "ui.h"
#include "camera.h"
#include "texture.h"

//==========================================
//  マクロ定義
//==========================================
#define FONTSIZE (15.0f) //UIに使用するフォントのサイズ
#define ERRORMESSAGE "Failed to save." //エラーメッセージ

//==========================================
//  セーブデータ保存先の列挙型定義
//==========================================
typedef enum
{
	SAVE_ALL = 0, //全データ
	SAVE_MAP, //マップ
	SAVE_COLLISION, //当たり判定
	SAVE_2DUI, //2DUI
	SAVE_LIGHT, //ライト
	SAVE_DRAW, //描画
	SAVE_PARTICLE, //パーティクル
	SAVE_MAX
}WHAT_SAVE;

//==========================================
//  セーブ先ファイル一覧
//==========================================
const char *c_apSaveFilePass[SAVE_MAX] =
{
	"02_data/01_BIN/00_Save_All.bin",
	"02_data/01_BIN/01_Save_Map.bin",
	"02_data/01_BIN/02_Save_Collision.bin",
	"02_data/01_BIN/03_Save_2DUI.bin",
	"02_data/01_BIN/04_Save_Light.bin",
	"02_data/01_BIN/05_Save_Draw.bin",
	"02_data/01_BIN/06_Save_Particle.bin"
};

//==========================================
//  セーブ用構造体定義
//==========================================
typedef struct
{

}SAVE_DATA;

//==========================================
//  プロトタイプ宣言
//==========================================
//メインシステム--------------------------
void InitVariable(void); //変数の初期化
void CreateSystemWindow(void); //システムウィンドウの生成
void SubWindowSystem(void); //各ウィンドウを生成する
void SaveSystem(void); //全システムのセーブ
//------------------------------------------

//マップ------------------------------------
void CreateMapWindow(void); //マップエディタウィンドウの生成
void MapEditterTab(void); //マップ配置タブ
void MeshEditterTab(void); //メッシュ編集タブ
//------------------------------------------

//当たり判定--------------------------------
void CreateCollisionWindow(void); //当たり判定ウィンドウの生成
//------------------------------------------

//2DUI--------------------------------------
void Create2DUInWindow(void); //2DUIウィンドウの生成
//------------------------------------------

//ライト------------------------------------
void CreateLightWindow(void); //ライト編集ウィンドウの生成
//------------------------------------------

//描画関連----------------------------------
void CreateDrawWindow(void); //描画関連ウィンドウの生成
//------------------------------------------

//パーティクル------------------------------
void CreateParticleWindow(void); //パーティクルウィンドウの生成
//------------------------------------------

//==========================================
//  グローバル変数宣言
//==========================================
bool g_bMap; //マップエディタの有無
bool g_bCollider; //当たり判定エディタの有無
bool g_b2DUI; //2DUIエディタの有無
bool g_bLight; //ライトエディタの有無
bool g_bDraw; //描画関連の有無
bool g_bParticle; //パーティクルの有無

//==========================================
//  初期化処理
//==========================================
void InitUi(HWND hWnd)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ImGui環境の設定
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//使用するフォント
	const char* FontPath = "02_data/99_font/GenShinGothic-Bold.ttf";

	//フォントサイズ
	const float FontSize = FONTSIZE;

	//文字の設定
	ImGuiIO & io = ImGui::GetIO();

	//io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF(FontPath, FontSize, nullptr, io.Fonts->GetGlyphRangesJapanese());

	//ImGuiスタイルの設定
	ImGui::StyleColorsLight();

	//プラットフォーム/レンダラーとバックエンドの設定
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	//変数の初期化
	InitVariable();
}

//==========================================
//  変数の初期化
//==========================================
void InitVariable()
{
	g_bMap = false;
	g_bCollider = false;
	g_b2DUI = false;
	g_bLight = false;
	g_bDraw = false;
	g_bParticle = false;
}

//==========================================
//  終了処理
//==========================================
void UninitUi()
{
	//ImGuiを終了
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//==========================================
//  更新処理
//==========================================
void UpdateUi()
{
	//ImGuiの実行
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//システムウィンドウを生成
	CreateSystemWindow();

	//更新の終了
	ImGui::EndFrame();
}

//==========================================
//  描画処理
//==========================================
void DrawUi()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライティングをオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//描画内容
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	//ライティングをオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================
//  システムウィンドウ
//==========================================
void CreateSystemWindow()
{
	//システムウィンドウの生成
	ImGui::Begin(u8"SystemWindow");

	//ウィンドウ表示
	if (ImGui::CollapsingHeader(u8"SubWindowSystem"))
	{
		SubWindowSystem();
	}

	//セーブボタンを表示
	if (ImGui::CollapsingHeader(u8"SaveSystem"))
	{
		SaveSystem();
	}

	//システムウィンドウの終了
	ImGui::End();
}

//==========================================
//  各ウィンドウを生成する
//==========================================
void SubWindowSystem()
{
	//マップエディタウィンドウの表示
	ImGui::Checkbox(u8"マップ配置", &g_bMap);
	if (g_bMap)
	{
		CreateMapWindow();
	}

	//当たり判定ウィンドウの表示
	ImGui::Checkbox(u8"当たり判定作成", &g_bCollider);
	if (g_bCollider)
	{
		CreateCollisionWindow();
	}

	//2DUIウィンドウの表示
	ImGui::Checkbox(u8"2DUI配置", &g_b2DUI);
	if (g_b2DUI)
	{
		Create2DUInWindow();
	}

	//ライトウィンドウの表示
	ImGui::Checkbox(u8"ライト編集", &g_bLight);
	if (g_bLight)
	{
		CreateLightWindow();
	}

	//描画関連ウィンドウの表示
	ImGui::Checkbox(u8"描画編集", &g_bDraw);
	if (g_bDraw)
	{
		CreateDrawWindow();
	}

	//パーティクルウィンドウの表示
	ImGui::Checkbox(u8"パーティクル編集", &g_bParticle);
	if (g_bParticle)
	{
		CreateParticleWindow();
	}
}

//==========================================
//  セーブシステム
//==========================================
void SaveSystem()
{
	//セーブボタンの生成
	if (ImGui::Button(u8"全データセーブ"))
	{
		//ローカル変数宣言
		FILE *pFile; //ファイルポインタ
		int save = 1; //セーブ専用

		//書き出し先ファイル
		pFile = fopen(c_apSaveFilePass[SAVE_ALL], "wb");

		//書き出し
		if (pFile != NULL)
		{
			fwrite(&save, sizeof(int), 1, pFile);	
			fclose(pFile);
		}
		else
		{
			MessageBox(NULL, ERRORMESSAGE, "Error", MB_ICONWARNING);
		}
	}
}

//==========================================
//  マップエディタウィンドウ
//==========================================
void CreateMapWindow()
{
	//マップエディタウィンドウの生成
	ImGui::Begin(u8"MapEditter ( 小原 / 委員長 )");

	//タブを生成
	if (ImGui::BeginTabBar(u8"ウィンドウ状態", 0))
	{
		//パーティクル設定モード
		if (ImGui::BeginTabItem(u8"アイテム配置 ( 小原 )"))
		{
			MapEditterTab();
			ImGui::EndTabItem();
		}

		//パーティクル編集モード
		if (ImGui::BeginTabItem(u8"メッシュ編集 ( 委員長 )"))
		{
			//パーティクルウィンドウベースの設定
			MeshEditterTab();
			ImGui::EndTabItem();
		}

		ImGui::Separator();
		ImGui::TreePop();
	}

	//マップエディタウィンドウの終了
	ImGui::End();
}

//==========================================
//  マップエディタタブ
//==========================================
void MapEditterTab(void)
{
	ImGui::Text(u8"小原が基本のマップエディタを作成");
}

//==========================================
//  メッシュエディタタブ
//==========================================
void MeshEditterTab()
{
	ImGui::Text(u8"委員長がメッシュマップエディタを作成");
}

//==========================================
//  当たり判定ウィンドウ
//==========================================
void CreateCollisionWindow()
{
	//当たり判定エディタウィンドウの生成
	ImGui::Begin(u8"ColliderWindow ( 中村 )");

	ImGui::Text(u8"中村が当たり判定エディタを作成");

	//当たり判定エディタウィンドウの終了
	ImGui::End();
}

//==========================================
//  2DUIウィンドウ
//==========================================
void Create2DUInWindow()
{
	//2DUIウィンドウの生成
	ImGui::Begin(u8"2DUIWindow ( 委員長 )");

	ImGui::Text(u8"委員長が2DUIエディタを作成");

	//2DUIウィンドウの終了
	ImGui::End();
}

//==========================================
//  ライトエディタウィンドウ
//==========================================
void CreateLightWindow()
{
	//ライトウィンドウの生成
	ImGui::Begin(u8"LightWindow ( 坂本 )");

	ImGui::Text(u8"坂本がライトエディタを作成");

	//ライトウィンドウの終了
	ImGui::End();
}

//==========================================
//  描画関連ウィンドウ
//==========================================
void CreateDrawWindow()
{
	//描画関連ウィンドウの生成
	ImGui::Begin(u8"DrawWindow ( 奥定 )");

	ImGui::Text(u8"奥定が描画関連を作成");

	//描画関連ウィンドウの終了
	ImGui::End();
}

//==========================================
//  パーティクルウィンドウ
//==========================================
void CreateParticleWindow()
{
	//描画関連ウィンドウの生成
	ImGui::Begin(u8"ParticleWindow ( 金崎 )");

	ImGui::Text(u8"金崎がパーティクルエディタを作成");

	//描画関連ウィンドウの終了
	ImGui::End();
}
