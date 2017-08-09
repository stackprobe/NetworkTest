#include "all.h"

int DxLibInited;

void EndProc(void)
{
	ExportSaveData();
	Gnd_FNLZ();

	if(DxLibInited)
	{
		DxLibInited = 0;
		errorCase(DxLib_End()); // ? ���s
	}
	termination();
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	memAlloc_INIT();
	initRnd((int)time(NULL));

	Gnd_INIT();
	ImportSaveData();

	// DxLib >

#if LOG_ENABLED
	SetApplicationLogSaveDirectory("C:\\temp");
#endif
	SetOutApplicationLogValidFlag(LOG_ENABLED); // DxLib �̃��O���o�� 1: ���� 0: ���Ȃ�

	SetAlwaysRunFlag(1); // ? ��A�N�e�B�u���� 1: ���� 0: �~�܂�

	SetMainWindowText(
#if LOG_ENABLED
		"(LOG_ENABLED) "
#endif
		"NetworkTest"
//		" / " __DATE__ " " __TIME__
		);

//	SetGraphMode(SCREEN_W, SCREEN_H, 32);
	SetGraphMode(Gnd.RealScreen_W, Gnd.RealScreen_H, 32);
	ChangeWindowMode(1); // 1: �E�B���h�E 0: �t���X�N���[��

//	SetFullSceneAntiAliasingMode(4, 2); // �K���Ȓl���������B�t���X�N���[���p�~�����̂ŕs�v

	errorCase(DxLib_Init()); // ? ���s

	DxLibInited = 1;

	SetMouseDispFlag(0); // ? �}�E�X��\�� 1: ���� 0: ���Ȃ�
	SetWindowSizeChangeEnableFlag(0); // �E�B���h�E�̉E�����h���b�O�ŐL�k 1: ���� 0: ���Ȃ�

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR); // BILINEAR���f�t�H�I

	// < DxLib

	// ������ Reset �ƕt���֐������ׂČĂ�ł������ƁB

	DPE_Reset();
	CEE_Reset();
	PE_Reset();

	LOGPOS();
	ProcMain();
	LOGPOS();

	EndProc();
	return 0; // dummy
}

// DxPrv_ >

static int DxPrv_GetMouseDispMode(void)
{
	return GetMouseDispFlag() ? 1 : 0;
}
static void DxPrv_SetMouseDispMode(int mode)
{
	SetMouseDispFlag(mode ? 1 : 0);
}
static void UnloadGraph(int &hdl)
{
	if(hdl != -1)
	{
		errorCase(DeleteGraph(hdl)); // ? ���s
		hdl = -1;
	}
}
static void DxPrv_SetScreenSize(int w, int h)
{
	int mdm = GetMouseDispMode();

	UnloadAllPicResHandle();
	SetDrawScreen(DX_SCREEN_BACK); // InnerDrawScrHdl ���J�����邽�߁B
	UnloadGraph(InnerDrawScrHdl); // MakeScreen -> DeleteGraph �ŗǂ��炵���B

	errorCase(SetGraphMode(w, h, 32) != DX_CHANGESCREEN_OK); // ? ���s

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	SetMouseDispMode(mdm);
}

// < DxPrv_

int GetMouseDispMode(void)
{
	return DxPrv_GetMouseDispMode();
}
void SetMouseDispMode(int mode)
{
	DxPrv_SetMouseDispMode(mode);
}
void ApplyScreenSize(void)
{
	DxPrv_SetScreenSize(Gnd.RealScreen_W, Gnd.RealScreen_H);
}
void SetScreenSize(int w, int h)
{
	m_range(w, SCREEN_W, SCREEN_W_MAX);
	m_range(h, SCREEN_H, SCREEN_H_MAX);

	if(Gnd.RealScreen_W != w || Gnd.RealScreen_H != h)
	{
		Gnd.RealScreen_W = w;
		Gnd.RealScreen_H = h;

		ApplyScreenSize();
	}
}
