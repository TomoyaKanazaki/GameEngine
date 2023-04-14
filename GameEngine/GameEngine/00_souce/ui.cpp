//==========================================
//
//  UI�\���̐���(ui.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <string.h>
#include "ui.h"
#include "camera.h"
#include "texture.h"
#include "meshfield.h"

//==========================================
//  �}�N����`
//==========================================
#define FONTSIZE (15.0f) //UI�Ɏg�p����t�H���g�̃T�C�Y
#define ERRORMESSAGE "Failed to save." //�G���[���b�Z�[�W

//==========================================
//  �Z�[�u�f�[�^�ۑ���̗񋓌^��`
//==========================================
typedef enum
{
	SAVE_ALL = 0, //�S�f�[�^
	SAVE_MAP, //�}�b�v
	SAVE_COLLISION, //�����蔻��
	SAVE_2DUI, //2DUI
	SAVE_LIGHT, //���C�g
	SAVE_DRAW, //�`��
	SAVE_PARTICLE, //�p�[�e�B�N��
	SAVE_MAX
}WHAT_SAVE;

//==========================================
//  �Z�[�u��t�@�C���ꗗ
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
//  �Z�[�u�p�\���̒�`
//==========================================
typedef struct
{

}SAVE_DATA;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
//���C���V�X�e��--------------------------
void InitVariable(void); //�ϐ��̏�����
void CreateSystemWindow(void); //�V�X�e���E�B���h�E�̐���
void SubWindowSystem(void); //�e�E�B���h�E�𐶐�����
void SaveSystem(void); //�S�V�X�e���̃Z�[�u
//------------------------------------------

//�}�b�v------------------------------------
void CreateMapWindow(void); //�}�b�v�G�f�B�^�E�B���h�E�̐���
void MapEditterTab(void); //�}�b�v�z�u�^�u
void MeshEditterTab(void); //���b�V���ҏW�^�u
//------------------------------------------

//�����蔻��--------------------------------
void CreateCollisionWindow(void); //�����蔻��E�B���h�E�̐���
//------------------------------------------

//2DUI--------------------------------------
void Create2DUInWindow(void); //2DUI�E�B���h�E�̐���
//------------------------------------------

//���C�g------------------------------------
void CreateLightWindow(void); //���C�g�ҏW�E�B���h�E�̐���
//------------------------------------------

//�`��֘A----------------------------------
void CreateDrawWindow(void); //�`��֘A�E�B���h�E�̐���
//------------------------------------------

//�p�[�e�B�N��------------------------------
void CreateParticleWindow(void); //�p�[�e�B�N���E�B���h�E�̐���
//------------------------------------------

//�J����---------------------------------------
void CreateCameraWIndow(void); //�J�����E�B���h�E�̐���
//------------------------------------------

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
bool g_bMap; //�}�b�v�G�f�B�^�̗L��
bool g_bCollider; //�����蔻��G�f�B�^�̗L��
bool g_b2DUI; //2DUI�G�f�B�^�̗L��
bool g_bLight; //���C�g�G�f�B�^�̗L��
bool g_bDraw; //�`��֘A�̗L��
bool g_bParticle; //�p�[�e�B�N���̗L��
bool g_bCamera; //�J�����̗L��
bool g_bBackGround; //�w�i�\���̗L��
bool g_bWire; //���C���[�t���[���̐؂�ւ�
bool g_bCull; //�J�����O���[�h�̐؂�ւ�

int g_nNum; //�ݒ蒸�_�w��p
D3DXCOLOR g_col; //���_�J���[�ݒ�p

//==========================================
//  ����������
//==========================================
void InitUi(HWND hWnd)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ImGui���̐ݒ�
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//�g�p����t�H���g
	const char* FontPath = "02_data/99_font/GenShinGothic-Bold.ttf";

	//�t�H���g�T�C�Y
	const float FontSize = FONTSIZE;

	//�����̐ݒ�
	ImGuiIO & io = ImGui::GetIO();

	//io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF(FontPath, FontSize, nullptr, io.Fonts->GetGlyphRangesJapanese());

	//ImGui�X�^�C���̐ݒ�
	ImGui::StyleColorsLight();

	//�v���b�g�t�H�[��/�����_���[�ƃo�b�N�G���h�̐ݒ�
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	//�ϐ��̏�����
	InitVariable();
}

//==========================================
//  �ϐ��̏�����
//==========================================
void InitVariable()
{
	g_bMap = false;
	g_bCollider = false;
	g_b2DUI = false;
	g_bLight = false;
	g_bDraw = false;
	g_bParticle = false;
	g_bCamera = false;
	g_bBackGround = false;
	g_bWire = false;
	g_bCull = true;

	g_nNum = 0;
	g_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================
//  �I������
//==========================================
void UninitUi()
{
	//ImGui���I��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//==========================================
//  �X�V����
//==========================================
void UpdateUi()
{
	//ImGui�̎��s
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//�V�X�e���E�B���h�E�𐶐�
	CreateSystemWindow();

	//�X�V�̏I��
	ImGui::EndFrame();
}

//==========================================
//  �`�揈��
//==========================================
void DrawUi()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C���[�t���[����ݒ�
	if (g_bWire)
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//�J�����O���[�h��ݒ�
	if (g_bCull)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	else
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//���C�e�B���O���I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�`����e
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	//���C�e�B���O���I��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================
//  �V�X�e���E�B���h�E
//==========================================
void CreateSystemWindow()
{
	//�E�B���h�E�I�v�V�����̐ݒ�
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove; //�ړ��𐧌�
	window_flags |= ImGuiWindowFlags_NoResize; //�T�C�Y�ύX�𐧌�
	window_flags |= ImGuiWindowFlags_NoCollapse; //�E�B���h�E�̕\���𐧌�
	if (g_bBackGround) //�w�i�\����ݒ�
	{
		window_flags |= ImGuiWindowFlags_NoBackground;
	}

	//�V�X�e���E�B���h�E�̐���
	ImGui::Begin(u8"SystemWindow", 0, window_flags);

	//�w�i�̕\��
	ImGui::Checkbox(u8"�w�i��\��", &g_bBackGround);

	//���C���[�t���[���̐؂�ւ�
	ImGui::Checkbox(u8"���C���[�t���[��", &g_bWire);

	//�J�����O���[�h�̐؂�ւ�
	ImGui::Checkbox(u8"�J�����O���[�h", &g_bCull);

	//�E�B���h�E�\��
	if (ImGui::CollapsingHeader(u8"SubWindowSystem"))
	{
		SubWindowSystem();
	}

	//�Z�[�u�{�^����\��
	if (ImGui::CollapsingHeader(u8"SaveSystem ( ���c )"))
	{
		SaveSystem();
	}

	//�V�X�e���E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  �e�E�B���h�E�𐶐�����
//==========================================
void SubWindowSystem()
{
	//�}�b�v�G�f�B�^�E�B���h�E�̕\��
	ImGui::Checkbox(u8"�}�b�v�z�u", &g_bMap);
	if (g_bMap)
	{
		CreateMapWindow();
	}

	//�����蔻��E�B���h�E�̕\��
	ImGui::Checkbox(u8"�����蔻��쐬", &g_bCollider);
	if (g_bCollider)
	{
		CreateCollisionWindow();
	}

	//2DUI�E�B���h�E�̕\��
	ImGui::Checkbox(u8"2DUI�z�u", &g_b2DUI);
	if (g_b2DUI)
	{
		Create2DUInWindow();
	}

	//���C�g�E�B���h�E�̕\��
	ImGui::Checkbox(u8"���C�g�ҏW", &g_bLight);
	if (g_bLight)
	{
		CreateLightWindow();
	}

	//�`��֘A�E�B���h�E�̕\��
	ImGui::Checkbox(u8"�`��ҏW", &g_bDraw);
	if (g_bDraw)
	{
		CreateDrawWindow();
	}

	//�p�[�e�B�N���E�B���h�E�̕\��
	ImGui::Checkbox(u8"�p�[�e�B�N���ҏW", &g_bParticle);
	if (g_bParticle)
	{
		CreateParticleWindow();
	}

	//�J�����E�B���h�E�̕\��
	ImGui::Checkbox(u8"�J�����ҏW", &g_bCamera);
	if (g_bCamera)
	{
		CreateCameraWIndow();
	}
}

//==========================================
//  �Z�[�u�V�X�e��
//==========================================
void SaveSystem()
{
	//�Z�[�u�{�^���̐���
	if (ImGui::Button(u8"�S�f�[�^�Z�[�u"))
	{
		//���[�J���ϐ��錾
		FILE *pFile; //�t�@�C���|�C���^
		int save = 1; //�Z�[�u��p

		//�����o����t�@�C��
		pFile = fopen(c_apSaveFilePass[SAVE_ALL], "wb");

		//�����o��
		if (pFile != NULL)
		{
			fwrite(&save, sizeof(int), 1, pFile);	
			fclose(pFile);
		}
		else
		{

		}
	}
}

//==========================================
//  �}�b�v�G�f�B�^�E�B���h�E
//==========================================
void CreateMapWindow()
{
	//�}�b�v�G�f�B�^�E�B���h�E�̐���
	ImGui::Begin(u8"MapEditter ( ���� / �ψ��� )");

	//�^�u�𐶐�
	if (ImGui::BeginTabBar(u8"�E�B���h�E���", 0))
	{
		//�p�[�e�B�N���ݒ胂�[�h
		if (ImGui::BeginTabItem(u8"�A�C�e���z�u ( ���� )"))
		{
			MapEditterTab();
			ImGui::EndTabItem();
		}

		//�p�[�e�B�N���ҏW���[�h
		if (ImGui::BeginTabItem(u8"���b�V���ҏW ( �ψ��� )"))
		{
			//�p�[�e�B�N���E�B���h�E�x�[�X�̐ݒ�
			MeshEditterTab();
			ImGui::EndTabItem();
		}

		ImGui::Separator();
		ImGui::TreePop();
	}

	//�}�b�v�G�f�B�^�E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  �}�b�v�G�f�B�^�^�u
//==========================================
void MapEditterTab(void)
{
	ImGui::Text(u8"��������{�̃}�b�v�G�f�B�^���쐬");
}

//==========================================
//  ���b�V���G�f�B�^�^�u
//==========================================
void MeshEditterTab()
{
	//���̎擾
	Mesh *meshfield = GetMeshField();
	LPDIRECT3DVERTEXBUFFER9 pMeshFieldBuff = GetVtxData();

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	pMeshFieldBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���S���W�̕ҏW
	ImGui::DragFloat3(u8"���̒��S���W", meshfield->pos);

	//�����̕ҏW
	ImGui::SliderFloat3(u8"���̌���", meshfield->rot, -D3DX_PI, D3DX_PI);

	//�ҏW���钸�_�̔ԍ����w��
	if (ImGui::ArrowButton("##left", 0) && g_nNum > 0) //�f�N�������g
	{
		g_nNum--;
	} ImGui::SameLine();

	//�X���C�_�[�Ŏw��
	ImGui::SliderInt(u8"���_�ԍ�", &g_nNum, 0, meshfield->data.nNumVtx - 1); ImGui::SameLine();

	if (ImGui::ArrowButton("##right", 1) && g_nNum < meshfield->data.nNumVtx - 1) //�C���N�������g
	{
		g_nNum++;
	}

	//���_���W�̕ҏW
	ImGui::DragFloat3(u8"���_���W", pVtx[g_nNum].pos);

	//�@���x�N�g���̕ҏW
	ImGui::DragFloat3(u8"�@���x�N�g��", pVtx[g_nNum].nor);

	//���_�J���[�̕ҏW
	g_col = pVtx[g_nNum].col;
	ImGui::ColorEdit4(u8"���_�J���[", g_col);
	pVtx[g_nNum].col = g_col;

	//�e�N�X�`�����W�̕ҏW
	ImGui::DragFloat2(u8"�e�N�X�`�����W", pVtx[g_nNum].tex);

	//���_�o�b�t�@���A�����b�N
	pMeshFieldBuff->Unlock();

	//���b�V�����̕ۑ�
	if (ImGui::Button(u8"���b�V������ۑ�"))
	{
		SaveMeshField();
	}

	//�������̕ҏW
	ImGui::Separator();
	ImGui::Text(u8"�������ҏW");

	//�������̕ύX
	ImGui::DragInt(u8"���������̕ύX", &meshfield->data.nNumMesh_U);
	ImGui::DragInt(u8"�c�������̕ύX", &meshfield->data.nNumMesh_V);

	//���̃T�C�Y�̕ҏW
	ImGui::DragFloat2(u8"���̃T�C�Y��ύX", meshfield->data.size);

	//���������X�V
	if (ImGui::Button(u8"�������ύX"))
	{
		ResetMeshField();
	}

	//GetOpenFileName
}

//==========================================
//  �����蔻��E�B���h�E
//==========================================
void CreateCollisionWindow()
{
	//�����蔻��G�f�B�^�E�B���h�E�̐���
	ImGui::Begin(u8"ColliderWindow ( ���� )");

	ImGui::Text(u8"�����������蔻��G�f�B�^���쐬");

	//�����蔻��G�f�B�^�E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  2DUI�E�B���h�E
//==========================================
void Create2DUInWindow()
{
	//2DUI�E�B���h�E�̐���
	ImGui::Begin(u8"2DUIWindow ( �ψ��� )");

	ImGui::Text(u8"�ψ�����2DUI�G�f�B�^���쐬");

	//2DUI�E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  ���C�g�G�f�B�^�E�B���h�E
//==========================================
void CreateLightWindow()
{
	//���C�g�E�B���h�E�̐���
	ImGui::Begin(u8"LightWindow ( ��{ )");

	ImGui::Text(u8"��{�����C�g�G�f�B�^���쐬");

	//���C�g�E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  �`��֘A�E�B���h�E
//==========================================
void CreateDrawWindow()
{
	//�`��֘A�E�B���h�E�̐���
	ImGui::Begin(u8"DrawWindow ( ���� )");

	ImGui::Text(u8"���肪�`��֘A���쐬");

	//�`��֘A�E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  �p�[�e�B�N���E�B���h�E
//==========================================
void CreateParticleWindow()
{
	//�p�[�e�B�N���E�B���h�E�̐���
	ImGui::Begin(u8"ParticleWindow ( ���� )");

	ImGui::Text(u8"���肪�p�[�e�B�N���G�f�B�^���쐬");

	//�p�[�e�B�N���E�B���h�E�̏I��
	ImGui::End();
}

//==========================================
//  �J�����E�B���h�E
//==========================================
void CreateCameraWIndow()
{
	//�J�����E�B���h�E�̐���
	ImGui::Begin(u8"CameraWindow ( ���c )");

	ImGui::Text(u8"���c���J�����G�f�B�^���쐬");

	//�J�����E�B���h�E�̏I��
	ImGui::End();
}
