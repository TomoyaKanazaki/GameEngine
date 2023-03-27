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

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
bool g_bMap; //�}�b�v�G�f�B�^�̗L��
bool g_bCollider; //�����蔻��G�f�B�^�̗L��
bool g_b2DUI; //2DUI�G�f�B�^�̗L��
bool g_bLight; //���C�g�G�f�B�^�̗L��
bool g_bDraw; //�`��֘A�̗L��
bool g_bParticle; //�p�[�e�B�N���̗L��

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
	//�V�X�e���E�B���h�E�̐���
	ImGui::Begin(u8"SystemWindow");

	//�E�B���h�E�\��
	if (ImGui::CollapsingHeader(u8"SubWindowSystem"))
	{
		SubWindowSystem();
	}

	//�Z�[�u�{�^����\��
	if (ImGui::CollapsingHeader(u8"SaveSystem"))
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
			MessageBox(NULL, ERRORMESSAGE, "Error", MB_ICONWARNING);
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
	ImGui::Text(u8"�ψ��������b�V���}�b�v�G�f�B�^���쐬");
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
	//�`��֘A�E�B���h�E�̐���
	ImGui::Begin(u8"ParticleWindow ( ���� )");

	ImGui::Text(u8"���肪�p�[�e�B�N���G�f�B�^���쐬");

	//�`��֘A�E�B���h�E�̏I��
	ImGui::End();
}
