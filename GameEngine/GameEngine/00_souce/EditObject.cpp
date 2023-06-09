#ifdef _DEBUG	// デバッグ処理
//===========================================
//
//オブジェクトのメイン処理[EditObject.cpp]
//Author 小原立暉
//
//===========================================
#include "main.h"
#include "model.h"
#include "EditObject.h"
#include "EditCollision.h"
#include "Editmain.h"
#include "input.h"
#include "camera.h"
#include "object.h"
#include "Shadow.h"
#include "EditBillboard.h"
#include "player.h"

//マクロ定義
#define EDITOBJ_SELECT_MATERIAL_ALPHA	(1.0f)		// 選択中のマテリアルの透明度
#define EDITOBJ_NORMAL_MATERIAL_ALPHA	(0.5f)		// 選択していないマテリアルの透明度
#define EDITOBJ_DELETE_OBJECT_RANGE		(70.0f)		// オブジェクト消去の範囲
#define EDITOBJ_ADJUSTMENT_MOVE			(4.0f)		// 調整用の移動量
#define EDITOBJ_MOVE					(16.0f)		// 通常の移動量
#define EDITOBJ_ROT_MOVE				(0.02f)		// 向きの移動量
#define EDITOBJ_SCALING					(0.02f)		// 拡大縮小率
#define EDITOBJ_COL_CHANGE_CNT			(5)			// 色が変わるカウント
#define EDITOBJ_COL_CONVERSION			(0.01f)		// マテリアルの変化量
#define EDITOBJ_UPDOWN_MOVE				(6.0f)		// 上下移動の移動量
#define EDITOBJ_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// 調整用の上下移動の移動量
#define EDITOBJ_ADJUSTMENT_ROT_MOVE		(15)		// 調整用の向きの移動量

//プロトタイプ宣言
void SaveCurrentEdit(void);			//エディット状況の一時保存処理
void TypeChangeEdit(void);			//種類変更処理
void MoveEdit(void);				//移動処理
void RotationEdit(void);			//回転処理
void SetEdit(void);					//オブジェクトの設定処理
void DeleteEditObject(void);		//オブジェクトの消去
void ScaleObjectX(void);			//オブジェクトの拡大縮小処理(X軸)
void ScaleObjectY(void);			//オブジェクトの拡大縮小処理(Y軸)
void ScaleObjectZ(void);			//オブジェクトの拡大縮小処理(Z軸)
void ScaleObject(void);				//オブジェクトの拡大縮小処理
void ResetEdit(void);				//オブジェクトの情報リセット処理
void EditMaterialCustom(void);		//マテリアルのエディット処理
void BreakEdit(void);				//オブジェクトの破壊エディット処理
void ShadowEdit(void);				//オブジェクトの影のエディット処理
void CollisionEdit(void);			//オブジェクトの当たり判定のエディット処理
void UpDownEditObject(void);		//オブジェクトの上下移動処理
void RightAngleEditObject(void);	//オブジェクトの直角処理
void CollisionRotationEdit(void);	//当たり判定の回転処理
void PlayerPosSetEditObject(void);	//オブジェクトのプレイヤー位置移動
void JudgeEditObject(void);			//オブジェクトの善悪処理

//破壊モードの表記
const char *c_apBreakmodename[BREAKTYPE_MAX] =
{
	"破壊不可",
	"一撃破壊",
	"吹き飛ぶ",
};

//影モードの表記
const char *c_apShadowmodename[SHADOWTYPE_MAX] =
{
	"影無し",
	"丸影",
	"リアル影",
};

//当たり判定モードの表記
const char *c_apCollisionmodename[COLLISIONTYPE_MAX] =
{
	"当たり判定無し",
	"モデル頂点の当たり判定",
	"作成した汎用の当たり判定",
	//"作成したそれぞれの当たり判定",
};

//善悪状態の表記
const char *c_apJudgemodename[JUDGESTATE_MAX] = 
{
	"良い建物",
	"悪い建物",
};

//グローバル変数
EditObject  g_EditObject;								//オブジェクトの情報
D3DXVECTOR3 g_aRotObject[MODEL_OBJ_MAX];				//オブジェクトの向きの情報
D3DXVECTOR3 g_aScaleObject[MODEL_OBJ_MAX];				//オブジェクトの拡大率の情報
int g_nStyleObject;										//スタイルの変数

//==========================================
//モデルの初期化処理
//==========================================
void InitEditObject(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	{//エディットオブジェクトの初期化
		//エディットオブジェクトの位置を初期化する
		g_EditObject.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//エディットオブジェクトの向きを初期化する
		g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//エディットオブジェクトの拡大率を初期化する
		g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		//エディットオブジェクトの番号を初期化する
		g_EditObject.nSetNumber = -1;

		//使用しない
		g_EditObject.bUse = false;

		//エディットオブジェクトの種類を初期化する
		g_EditObject.nType = MODELTYPE_OBJECT_TREE;

		//モデルの基本情報
		g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

		//選択中のマテリアルの番号を初期化する
		g_EditObject.nCntMaterial = 0;

		//色を変えるカウントを初期化する
		g_EditObject.nColorCount = 0;

		//壊れない
		g_EditObject.Break.Breaktype = BREAKTYPE_NONE;

		//影無し
		g_EditObject.Shadowtype.Shadowtype = SHADOWTYPE_NONE;

		//当たり判定あり
		g_EditObject.Collisiontype.Collisiontype = COLLISIONTYPE_MODEL;

		//良い建物
		g_EditObject.Judge.Judgetype = JUDGESTATE_JUSTICE;
	}

	for (int nCntBreak = 0; nCntBreak < BREAKTYPE_MAX; nCntBreak++)
	{
		//壊れ方のデバッグ表記を設定
		g_EditObject.Break.pBreakMode[nCntBreak] = (char*)c_apBreakmodename[nCntBreak];
	}

	for (int nCntShadow = 0; nCntShadow < SHADOWTYPE_MAX; nCntShadow++)
	{
		//影のデバッグ表記を設定
		g_EditObject.Shadowtype.pShadowMode[nCntShadow] = (char*)c_apShadowmodename[nCntShadow];
	}

	for (int nCntCollision = 0; nCntCollision < COLLISIONTYPE_MAX; nCntCollision++)
	{
		//当たり判定のデバッグ表記を設定
		g_EditObject.Collisiontype.pCollisionMode[nCntCollision] = (char*)c_apCollisionmodename[nCntCollision];
	}

	for (int nCntJudge = 0; nCntJudge < JUDGESTATE_MAX; nCntJudge++)
	{
		//善悪のデバッグ正気を設定
		g_EditObject.Judge.pJudgeMode[nCntJudge] = (char*)c_apJudgemodename[nCntJudge];
	}

	//当たり判定の向き変数
	g_EditObject.CollInfo.rot      = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	g_EditObject.CollInfo.stateRot = ROTSTATE_0;					//向き状態

	//スタイルを設定する
	g_nStyleObject = EDITSTYLE_OBJECT;

	//カスタム用のマテリアル情報
	for (int nCntModel = 0; nCntModel < MODEL_OBJ_MAX; nCntModel++)
	{ // モデルの初期化
		//マテリアルの色を取得する
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)GetModelData(nCntModel + FROM_OBJECT).pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)GetModelData(nCntModel + FROM_OBJECT).dwNumMat; nCntMat++)
		{
			//マテリアルをコピーする
			g_EditObject.MatCopy[nCntModel][nCntMat] = pMat[nCntMat];
			g_EditObject.EditMaterial[nCntModel][nCntMat] = pMat[nCntMat];
		}
	}

	// オブジェクトの拡大率の情報の初期化
	for (int nCntObject = 0; nCntObject < MODEL_OBJ_MAX; nCntObject++)
	{ // オブジェクトの種類の総数分繰り返す

		g_aRotObject[nCntObject]   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//オブジェクトの向きの情報
		g_aScaleObject[nCntObject] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//オブジェクトの拡大率情報
	}
}

//========================================
//モデルの終了処理
//========================================
void UninitEditObject(void)
{

}

//========================================
//モデルの更新処理
//========================================
void UpdateEditObject(void)
{
	Object *pObject = GetObjectData();				//オブジェクトを取得する

	g_nStyleObject = GetStyle();					//スタイルを取得する

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++, pObject++)
		{
			if (pObject->bUse == false && g_EditObject.bUse == false)
			{//手が加えられていないオブジェクトだった場合
				//オブジェクトを設定状態にする
				g_EditObject.bUse = true;

				//オブジェクトの番号を保存する
				g_EditObject.nSetNumber = nCntObject;

				//モデル情報を読み込む
				g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

				break;							//抜け出す
			}
		}

		//エディット状況の一時保存処理
		SaveCurrentEdit();
	}
	else
	{//オブジェクト設置モードじゃなかった場合
		//使用しない
		g_EditObject.bUse = false;
	}

	if (GetCollisionStyle() == COLLISIONSTYLE_OBJECT)
	{ // 当たり判定スタイルがオブジェクト変更状態の場合

		//種類変更処理
		TypeChangeEdit();

		//移動処理
		MoveEdit();

		//回転処理
		RotationEdit();

		//オブジェクトの直角処理
		RightAngleEditObject();

		if (g_EditObject.rot.y > D3DX_PI)
		{//3.14fより大きくなった場合
			//-3.14fに補正する
			g_EditObject.rot.y -= D3DX_PI * 2;
		}
		else if (g_EditObject.rot.y < -D3DX_PI)
		{//-3.14fより小さくなった場合
			//3.14fに補正する
			g_EditObject.rot.y += D3DX_PI * 2;
		}

		//オブジェクトの消去
		DeleteEditObject();

		//オブジェクトの設定処理
		SetEdit();

		//オブジェクトの拡大縮小処理(X軸)
		ScaleObjectX();

		//オブジェクトの拡大縮小処理(Y軸)
		ScaleObjectY();

		//オブジェクトの拡大縮小処理(Z軸)
		ScaleObjectZ();

		//オブジェクトの拡大縮小処理
		ScaleObject();

		//オブジェクトの情報リセット処理
		ResetEdit();

		//マテリアルのエディット処理
		EditMaterialCustom();

		//オブジェクトの破壊エディット処理
		BreakEdit();

		//オブジェクトの影のエディット処理
		ShadowEdit();

		//オブジェクトの当たり判定のエディット処理
		CollisionEdit();

		//オブジェクトの上下移動処理
		UpDownEditObject();

		//回転処理
		CollisionRotationEdit();

		//オブジェクトのプレイヤー位置移動
		PlayerPosSetEditObject();

		//オブジェクトの善悪処理
		JudgeEditObject();			
	}
}

//=====================================
//モデルの描画処理
//=====================================
void DrawEditObject(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pEditMat;						//エディットマテリアルのデータ

	if (g_EditObject.bUse == true)
	{//設定中か使用している場合
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_EditObject.mtx);

		//拡大率を反映
		D3DXMatrixScaling(&mtxScale, g_EditObject.scale.x, g_EditObject.scale.y, g_EditObject.scale.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxScale);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_EditObject.rot.y, g_EditObject.rot.x, g_EditObject.rot.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_EditObject.pos.x, g_EditObject.pos.y, g_EditObject.pos.z);
		D3DXMatrixMultiply(&g_EditObject.mtx, &g_EditObject.mtx, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_EditObject.mtx);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.modelData.dwNumMat; nCntMat++)
		{
			pEditMat = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat];

			if (nCntMat == g_EditObject.nCntMaterial)
			{//選択中のマテリアルだった場合
				//透明度を設定する
				pEditMat->MatD3D.Ambient.a = EDITOBJ_SELECT_MATERIAL_ALPHA;
				pEditMat->MatD3D.Diffuse.a = EDITOBJ_SELECT_MATERIAL_ALPHA;
			}
			else
			{//選択されていないマテリアルだった場合
				//透明度を設定する
				pEditMat->MatD3D.Ambient.a = EDITOBJ_NORMAL_MATERIAL_ALPHA;
				pEditMat->MatD3D.Diffuse.a = EDITOBJ_NORMAL_MATERIAL_ALPHA;
			}

			//マテリアルの設定
			pDevice->SetMaterial(&g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_EditObject.modelData.pTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_EditObject.modelData.pMesh->DrawSubset(nCntMat);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//エディットオブジェクトの取得処理
//=======================================
EditObject *GetEditObject(void)
{
	//エディットオブジェクトの情報を返す
	return &g_EditObject;
}

//=======================================
//オブジェクトの消去
//=======================================
void DeleteEditObject(void)
{
	Object *pObject = GetObjectData();				//オブジェクトの情報を取得する

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++, pObject++)
	{
		if (pObject->bUse == true)
		{//オブジェクトが使用されていた場合
			if (g_EditObject.pos.x >= pObject->pos.x - EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.pos.x <= pObject->pos.x + EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.pos.z >= pObject->pos.z - EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.pos.z <= pObject->pos.z + EDITOBJ_DELETE_OBJECT_RANGE &&
				g_EditObject.bUse == true)
			{//範囲内に入った場合
				//削除対象状態にする
				pObject->editState = OBJECTSTATE_DELETETARGET;

				if (GetKeyboardTrigger(DIK_9) == true)
				{//9キーを押した場合

					//使用していない
					pObject->bUse = false;
				}
			}
			else
			{//範囲外だった場合
				//使用状態にする
				pObject->editState = OBJECTSTATE_USE;
			}
		}
	}
}

//=======================================
//エディット状況の一時保存処理
//=======================================
void SaveCurrentEdit(void)
{
	//ポインタを宣言
	Collision     *pCollision     = GetCollision();
	EditCollision *pEditCollision = GetEditCollision();

	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // 当たり判定の最大数分繰り返す

		// 横幅と縦幅を計算
		pEditCollision->pCollision->fWidth[nCntColl] = (GetModelData(MODELTYPE_EDIT_COLLISION).size.x * pEditCollision->pCollision->scale[nCntColl].x) * 0.5f;
		pEditCollision->pCollision->fDepth[nCntColl] = (GetModelData(MODELTYPE_EDIT_COLLISION).size.z * pEditCollision->pCollision->scale[nCntColl].z) * 0.5f;
		
		// 横幅と縦幅を代入
		pCollision[g_EditObject.nType].fWidth[nCntColl] = pEditCollision->pCollision->fWidth[nCntColl];
		pCollision[g_EditObject.nType].fDepth[nCntColl] = pEditCollision->pCollision->fDepth[nCntColl];
	}

	//現在のモデル向きをセーブ
	g_aRotObject[g_EditObject.nType] = g_EditObject.rot;

	//現在のモデル拡大率をセーブ
	g_aScaleObject[g_EditObject.nType] = g_EditObject.scale;
}

//=======================================
//種類変更処理
//=======================================
void TypeChangeEdit(void)
{
	//ポインタを宣言
	Collision     *pCollision     = GetCollision();
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_1) == true &&
			GetKeyboardPress(DIK_LSHIFT) == true)
		{
			//設定オブジェクトの種類を選択する
			g_EditObject.nType = (g_EditObject.nType + (MODEL_OBJ_MAX - 1)) % MODEL_OBJ_MAX;

			//設定する
			g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

			//現在の向きを初期化
			g_EditObject.CollInfo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EditObject.CollInfo.stateRot = ROTSTATE_0;

			//現在の当たり判定をロード
			pEditCollision->pCollision = &pCollision[g_EditObject.nType];

			// 位置情報の初期化
			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // 当たり判定の最大数分繰り返す

				pEditCollision->pos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
				pEditCollision->vecPos[nCntColl] = pEditCollision->pCollision->vecPos[nCntColl];	// 位置ベクトル
			}

			//現在のモデル向きをロード
			g_EditObject.rot = g_aRotObject[g_EditObject.nType];

			//現在のモデル拡大率をロード
			g_EditObject.scale = g_aScaleObject[g_EditObject.nType];
		}
		else if (GetKeyboardTrigger(DIK_1) == true)
		{//1キーを押した場合
			//設定オブジェクトの種類を選択する
			g_EditObject.nType = (g_EditObject.nType + 1) % MODEL_OBJ_MAX;

			//設定する
			g_EditObject.modelData = GetModelData(g_EditObject.nType + FROM_OBJECT);

			//現在の向きを初期化
			g_EditObject.CollInfo.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_EditObject.CollInfo.stateRot = ROTSTATE_0;

			//現在の当たり判定をロード
			pEditCollision->pCollision = &pCollision[g_EditObject.nType];

			// 位置情報の初期化
			for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
			{ // 当たり判定の最大数分繰り返す

				pEditCollision->pos[nCntColl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 位置
				pEditCollision->vecPos[nCntColl] = pEditCollision->pCollision->vecPos[nCntColl];	// 位置ベクトル
			}

			//現在のモデル向きをロード
			g_EditObject.rot = g_aRotObject[g_EditObject.nType];

			//現在のモデル拡大率をロード
			g_EditObject.scale = g_aScaleObject[g_EditObject.nType];
		}
	}
}

//=======================================
//移動処理
//=======================================
void MoveEdit(void)
{
	//ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左シフトキーが押されていた場合
		//処理を抜ける
		return;
	}

	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//左コントロールキーを押していた場合
		if (g_nStyleObject == EDITSTYLE_OBJECT)
		{//オブジェクト設置モードだった場合
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキーを押した場合
				//位置を奥に進める
				g_EditObject.pos.z += EDITOBJ_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキーを押した場合
				//位置を手前に進める
				g_EditObject.pos.z -= EDITOBJ_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_A) == true)
			{//Aキーを押した場合
				//位置を左に進める
				g_EditObject.pos.x -= EDITOBJ_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_D) == true)
			{//Dキーを押した場合
				//位置を右に進める
				g_EditObject.pos.x += EDITOBJ_ADJUSTMENT_MOVE;
			}
		}
	}
	else
	{//左コントロールを押していない場合
		if (g_nStyleObject == EDITSTYLE_OBJECT)
		{//オブジェクト設置モードだった場合
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーを押した場合
			 //位置を奥に進める
				g_EditObject.pos.z += EDITOBJ_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーを押した場合
			 //位置を手前に進める
				g_EditObject.pos.z -= EDITOBJ_MOVE;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{//Aキーを押した場合
			 //位置を左に進める
				g_EditObject.pos.x -= EDITOBJ_MOVE;
			}

			if (GetKeyboardPress(DIK_D) == true)
			{//Dキーを押した場合
			 //位置を右に進める
				g_EditObject.pos.x += EDITOBJ_MOVE;
			}
		}
	}

	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // 当たり判定の最大数分繰り返す

		//当たり判定の位置を反映 (ベクトルの逆方向)
		pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
	}
}

//=======================================
//回転処理
//=======================================
void RotationEdit(void)
{
	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//左CTRLキーを押している場合
		//処理を抜ける
		return;
	}

	if (GetKeyboardPress(DIK_Q) == true)
	{//Qキーを押した場合
		//向きを変える
		g_EditObject.rot.y += EDITOBJ_ROT_MOVE;
	}
	else if (GetKeyboardPress(DIK_E) == true)
	{//Eキーを押した場合
		//向きを変える
		g_EditObject.rot.y -= EDITOBJ_ROT_MOVE;
	}
}

//=======================================
//オブジェクトの設定処理
//=======================================
void SetEdit(void)
{
	// 変数を宣言
	ROTSTATE stateRot;

	// ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardTrigger(DIK_0) == true)
		{//0キーを押した場合
			for (int nCount = 0; nCount < MAX_MATERIAL; nCount++)
			{
				// 透明度を元に戻す
				g_EditObject.EditMaterial[g_EditObject.nType][nCount].MatD3D.Ambient.a = g_EditObject.MatCopy[g_EditObject.nType][nCount].MatD3D.Ambient.a;
				g_EditObject.EditMaterial[g_EditObject.nType][nCount].MatD3D.Diffuse.a = g_EditObject.MatCopy[g_EditObject.nType][nCount].MatD3D.Diffuse.a;
			}

			//代入する向き状態を設定
			stateRot = (g_EditObject.Collisiontype.Collisiontype == COLLISIONTYPE_CREATE) ? g_EditObject.CollInfo.stateRot : ROTSTATE_0;

			//オブジェクトの設定処理
			SetObject
			(
				g_EditObject.pos,									// 位置
				g_EditObject.rot,									// 向き
				g_EditObject.scale,									// 拡大率
				&g_EditObject.EditMaterial[g_EditObject.nType][0],	// マテリアルのデータ
				g_EditObject.nType,									// 種類
				g_EditObject.Break.Breaktype,						// 破壊の種類
				g_EditObject.Shadowtype.Shadowtype,					// 影の種類
				g_EditObject.Collisiontype.Collisiontype,			// 当たり判定の種類
				stateRot,											// 向き状態
				APPEARSTATE_COMPLETE,								// 出現方法
				g_EditObject.Judge.Judgetype						// 善悪の種類
			);

			//エディットオブジェクトの番号を初期化する
			g_EditObject.nSetNumber = -1;
		}
	}
}

//=======================================
//オブジェクトの拡大縮小処理(X軸)
//=======================================
void ScaleObjectX(void)
{
	//ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_U) == true)
			{//Uキーを押した場合
				//X軸を拡大する
				g_EditObject.scale.x += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_J) == true)
			{//Jキーを押した場合
				//X軸を縮小する
				g_EditObject.scale.x -= EDITOBJ_SCALING;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_U) == true)
			{//Uキーを押した場合
				//X軸を拡大する
				g_EditObject.scale.x += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_J) == true)
			{//Jキーを押した場合
				//X軸を縮小する
				g_EditObject.scale.x -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//オブジェクトの拡大縮小処理(Y軸)
//=======================================
void ScaleObjectY(void)
{
	//ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_I) == true)
			{//Iキーを押した場合
				//Y軸を拡大する
				g_EditObject.scale.y += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_K) == true)
			{//Kキーを押した場合
				//Y軸を縮小する
				g_EditObject.scale.y -= EDITOBJ_SCALING;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_I) == true)
			{//Iキーを押した場合
				//Y軸を拡大する
				g_EditObject.scale.y += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_K) == true)
			{//Kキーを押した場合
				//Y軸を縮小する
				g_EditObject.scale.y -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//オブジェクトの拡大縮小処理(Z軸)
//=======================================
void ScaleObjectZ(void)
{
	//ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_O) == true)
			{//Oキーを押した場合
				//Z軸を拡大する
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_L) == true)
			{//Lキーを押した場合
				//Z軸を縮小する
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_O) == true)
			{//Oキーを押した場合
				//Z軸を拡大する
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_L) == true)
			{//Lキーを押した場合
				//Z軸を縮小する
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//オブジェクトの拡大縮小処理
//=======================================
void ScaleObject(void)
{
	//ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_4) == true)
			{//4キーを押した場合
				//拡大する
				g_EditObject.scale.x += EDITOBJ_SCALING;
				g_EditObject.scale.y += EDITOBJ_SCALING;
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardTrigger(DIK_5) == true)
			{//5キーを押した場合
				//縮小する
				g_EditObject.scale.x -= EDITOBJ_SCALING;
				g_EditObject.scale.y -= EDITOBJ_SCALING;
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_4) == true)
			{//4キーを押した場合
				//拡大する
				g_EditObject.scale.x += EDITOBJ_SCALING;
				g_EditObject.scale.y += EDITOBJ_SCALING;
				g_EditObject.scale.z += EDITOBJ_SCALING;
			}
			else if (GetKeyboardPress(DIK_5) == true)
			{//5キーを押した場合
				//縮小する
				g_EditObject.scale.x -= EDITOBJ_SCALING;
				g_EditObject.scale.y -= EDITOBJ_SCALING;
				g_EditObject.scale.z -= EDITOBJ_SCALING;
			}
		}
	}
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void ResetEdit(void)
{
	if (g_nStyleObject == EDITSTYLE_OBJECT)
	{//オブジェクト設置モードだった場合

		if (GetKeyboardTrigger(DIK_2) == true)
		{//2キーを押した場合

			//角度を初期化する
			g_EditObject.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		if (GetKeyboardTrigger(DIK_3) == true)
		{//3キーを押した場合

			//拡大率を初期化する
			g_EditObject.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
	}
}

//=======================================
//マテリアルのエディット処理
//=======================================
void EditMaterialCustom(void)
{
	D3DMATERIAL9 *pMatEdit;		//マテリアルデータへのポインタ

	if (g_EditObject.bUse == true)
	{//設定中か使用している場合

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACEキーを押した場合

			//マテリアルを変更する
			g_EditObject.nCntMaterial = (g_EditObject.nCntMaterial + 1) % (int)(g_EditObject.modelData.dwNumMat);
		}

		for (int nCntMat = 0; nCntMat < (int)g_EditObject.modelData.dwNumMat; nCntMat++)
		{
			//マテリアルデータへのポインタを取得
			pMatEdit = &g_EditObject.EditMaterial[g_EditObject.nType][nCntMat].MatD3D;

			if (nCntMat == g_EditObject.nCntMaterial)
			{//選択中のマテリアルだった場合

				if (GetKeyboardPress(DIK_LSHIFT) == true)
				{//左シフトキーを押した場合

					if (GetKeyboardPress(DIK_R) == true)
					{//Rキーを押した場合

						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20Fごとに

							//R値を増やす
							pMatEdit->Ambient.r += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.r > 1.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Ambient.r = 0.0f;
							}

							pMatEdit->Diffuse.r += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.r > 1.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Diffuse.r = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_F) == true)
					{//Fキーを押した場合

					 //カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20Fごとに

							//R値を増やす
							pMatEdit->Ambient.r -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.r < 0.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Ambient.r = 1.0f;
							}

							pMatEdit->Diffuse.r -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.r < 0.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Diffuse.r = 1.0f;
							}
						}
					}

					if (GetKeyboardPress(DIK_T) == true)
					{//Tキーを押した場合

						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20Fごとに

							//G値を増やす
							pMatEdit->Ambient.g += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.g > 1.0f)
							{//G値が限界を超えたら

								//G値を0に戻す
								pMatEdit->Ambient.g = 0.0f;
							}

							//G値を増やす
							pMatEdit->Diffuse.g += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.g > 1.0f)
							{//G値が限界を超えたら

								//G値を0に戻す
								pMatEdit->Diffuse.g = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_G) == true)
					{//Gキーを押した場合

						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20Fごとに

							//R値を増やす
							pMatEdit->Ambient.g -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.g < 0.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Ambient.g = 1.0f;
							}

							//G値を増やす
							pMatEdit->Diffuse.g -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.g < 0.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Diffuse.g = 1.0f;
							}
						}
					}

					if (GetKeyboardPress(DIK_Y) == true)
					{//Yキーを押した場合

						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20Fごとに

							//B値を増やす
							pMatEdit->Ambient.b += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.b > 1.0f)
							{//B値が限界を超えたら

								//B値を0に戻す
								pMatEdit->Ambient.b = 0.0f;
							}

							//B値を増やす
							pMatEdit->Diffuse.b += EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.b > 1.0f)
							{//B値が限界を超えたら

								//B値を0に戻す
								pMatEdit->Diffuse.b = 0.0f;
							}
						}
					}
					else if (GetKeyboardPress(DIK_H) == true)
					{//Hキーを押した場合

						//カウントを加算する
						g_EditObject.nColorCount++;

						if (g_EditObject.nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
						{//20Fごとに

							//R値を増やす
							pMatEdit->Ambient.b -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Ambient.b < 0.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Ambient.b = 1.0f;
							}

							pMatEdit->Diffuse.b -= EDITOBJ_COL_CONVERSION;

							if (pMatEdit->Diffuse.b < 0.0f)
							{//R値が限界を超えたら

								//R値を0に戻す
								pMatEdit->Diffuse.b = 1.0f;
							}
						}
					}

					//アドレスを入れる
					D3DMATERIAL9 *pMatOriginal = &g_EditObject.MatCopy[g_EditObject.nType][nCntMat].MatD3D;

					if (GetKeyboardPress(DIK_V) == true)
					{//Vキーを押した場合

						//R値のリセット
						pMatEdit->Ambient.r = pMatOriginal->Ambient.r;
						pMatEdit->Diffuse.r = pMatOriginal->Diffuse.r;
					}

					if (GetKeyboardPress(DIK_B) == true)
					{//Bキーを押した場合

						//G値のリセット
						pMatEdit->Ambient.g = pMatOriginal->Ambient.g;
						pMatEdit->Diffuse.g = pMatOriginal->Diffuse.g;
					}

					if (GetKeyboardPress(DIK_N) == true)
					{//Nキーを押した場合

						//B値のリセット
						pMatEdit->Ambient.b = pMatOriginal->Ambient.b;
						pMatEdit->Diffuse.b = pMatOriginal->Diffuse.b;
					}
				}
			}
		}
	}
}

//=======================================
//オブジェクトの破壊可能処理
//=======================================
void BreakEdit(void)
{
	if (GetKeyboardTrigger(DIK_7) == true)
	{//7キーを押した場合
		//破壊の変数を切り替える
		g_EditObject.Break.Breaktype = (g_EditObject.Break.Breaktype + 1) % BREAKTYPE_MAX;
	}
}

//=======================================
//オブジェクトの影のエディット処理
//=======================================
void ShadowEdit(void)
{
	if (GetKeyboardTrigger(DIK_8) == true)
	{//8キーを押した場合
		//影の種類を切り替える
		g_EditObject.Shadowtype.Shadowtype = (g_EditObject.Shadowtype.Shadowtype + 1) % SHADOWTYPE_MAX;
	}
}

//=======================================
//オブジェクトの当たり判定のエディット処理
//=======================================
void CollisionEdit(void)
{
	if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
	{//DIK_BACKSPACEキーを押した場合
		//当たり判定の種類を切り替える
		g_EditObject.Collisiontype.Collisiontype = (g_EditObject.Collisiontype.Collisiontype + 1) % COLLISIONTYPE_MAX;
	}
}

//=======================================
//オブジェクトの上下移動処理
//=======================================
void UpDownEditObject(void)
{
	//ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//左SHIFTキーを押している場合
		if (GetKeyboardPress(DIK_LCONTROL) == true)
		{//左コントロールキーを押していた場合
			if (GetKeyboardTrigger(DIK_W) == true)
			{//Wキーを押している場合
				//位置を奥に進める
				g_EditObject.pos.y += EDITOBJ_UPDOWN_ADJUSTMENT_MOVE;
			}

			if (GetKeyboardTrigger(DIK_S) == true)
			{//Sキーを押している場合
				//位置を手前に進める
				g_EditObject.pos.y -= EDITOBJ_UPDOWN_ADJUSTMENT_MOVE;
			}
		}
		else
		{//左コントロールキーを押していない場合
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーを押している場合
			 //位置を奥に進める
				g_EditObject.pos.y += EDITOBJ_UPDOWN_MOVE;
			}

			if (GetKeyboardPress(DIK_S) == true)
			{//Sキーを押している場合
			 //位置を手前に進める
				g_EditObject.pos.y -= EDITOBJ_UPDOWN_MOVE;
			}
		}

		if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_D) == true)
		{//AキーかDキーを押した場合
			//地面に戻す
			g_EditObject.pos.y = 0.0f;
		}
	}

	for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
	{ // 当たり判定の最大数分繰り返す

		//当たり判定の位置を反映 (ベクトルの逆方向)
		pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
	}
}

//=======================================
//オブジェクトの直角処理
//=======================================
void RightAngleEditObject(void)
{
	if (GetKeyboardPress(DIK_LCONTROL) == true)
	{//左CTRLキーを押している場合
		if (GetKeyboardTrigger(DIK_Q) == true)
		{//Qキーを押した場合
			//向きを変える
			g_EditObject.rot.y += D3DXToRadian(EDITOBJ_ADJUSTMENT_ROT_MOVE);
		}
		else if (GetKeyboardTrigger(DIK_E) == true)
		{//Eキーを押した場合
			//向きを変える
			g_EditObject.rot.y -= D3DXToRadian(EDITOBJ_ADJUSTMENT_ROT_MOVE);
		}
	}
}

//=======================================
//当たり判定回転処理
//=======================================
void CollisionRotationEdit(void)
{
	// ポインタを宣言
	EditCollision *pEditCollision = GetEditCollision();

	if (GetKeyboardTrigger(DIK_C) == true)
	{//Cキーを押した場合
		//向きを変更
		g_EditObject.CollInfo.rot.y += D3DX_PI * 0.5f;

		//向き状態を変更
		g_EditObject.CollInfo.stateRot = (ROTSTATE)((g_EditObject.CollInfo.stateRot + 1) % ROTSTATE_MAX);

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			//位置ベクトルを90度回転
			pEditCollision->vecPos[nCntColl] = D3DXVECTOR3(-pEditCollision->vecPos[nCntColl].z, pEditCollision->vecPos[nCntColl].y, pEditCollision->vecPos[nCntColl].x);

			//位置を反映 (ベクトルの方向)
			pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
		}
	}
	else if (GetKeyboardTrigger(DIK_Z) == true)
	{//Zキーを押した場合
		//向きを変更
		g_EditObject.CollInfo.rot.y -= D3DX_PI * 0.5f;

		//向き状態を変更
		g_EditObject.CollInfo.stateRot = (ROTSTATE)((g_EditObject.CollInfo.stateRot + (ROTSTATE_MAX - 1)) % ROTSTATE_MAX);

		for (int nCntColl = 0; nCntColl < MAX_COLLISION; nCntColl++)
		{ // 当たり判定の最大数分繰り返す

			//位置ベクトルを90度回転
			pEditCollision->vecPos[nCntColl] = D3DXVECTOR3(pEditCollision->vecPos[nCntColl].z, pEditCollision->vecPos[nCntColl].y, -pEditCollision->vecPos[nCntColl].x);

			//位置を反映 (ベクトルの方向)
			pEditCollision->pos[nCntColl] = g_EditObject.pos - pEditCollision->vecPos[nCntColl];
		}
	}

	if (g_EditObject.CollInfo.rot.y > D3DX_PI)
	{//3.14fより大きくなった場合
		//-3.14fに補正する
		g_EditObject.CollInfo.rot.y -= D3DX_PI * 2;
	}
	else if (g_EditObject.CollInfo.rot.y < -D3DX_PI)
	{//-3.14fより小さくなった場合
		//3.14fに補正する
		g_EditObject.CollInfo.rot.y += D3DX_PI * 2;
	}
}

//=======================================
//オブジェクトのプレイヤー位置移動
//=======================================
void PlayerPosSetEditObject(void)
{
	// ポインタを宣言
	Player *pPlayer = GetPlayer();	// プレイヤーの情報

	if (GetKeyboardTrigger(DIK_LMENU) == true)
	{ // 左の ALTキーが押された場合

		// プレイヤーの位置に位置を移動
		g_EditObject.pos = pPlayer->pos;
	}
}

//=======================================
//オブジェクトの善悪処理
//=======================================
void JudgeEditObject(void)
{
	if (GetKeyboardTrigger(DIK_RCONTROL) == true)
	{//右コントロールキーを押した場合

		//善悪の種類を切り替える
		g_EditObject.Judge.Judgetype = (g_EditObject.Judge.Judgetype + 1) % JUDGESTATE_MAX;
	}
}

#endif