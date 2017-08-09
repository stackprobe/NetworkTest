#include "all.h"

int DxLibInited;

void EndProc(void)
{
	ExportSaveData();
	Gnd_FNLZ();

	if(DxLibInited)
	{
		DxLibInited = 0;
		errorCase(DxLib_End()); // ? 失敗
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
	SetOutApplicationLogValidFlag(LOG_ENABLED); // DxLib のログを出力 1: する 0: しない

	SetAlwaysRunFlag(1); // ? 非アクティブ時に 1: 動く 0: 止まる

	SetMainWindowText(
#if LOG_ENABLED
		"(LOG_ENABLED) "
#endif
		"NetworkTest"
//		" / " __DATE__ " " __TIME__
		);

//	SetGraphMode(SCREEN_W, SCREEN_H, 32);
	SetGraphMode(Gnd.RealScreen_W, Gnd.RealScreen_H, 32);
	ChangeWindowMode(1); // 1: ウィンドウ 0: フルスクリーン

//	SetFullSceneAntiAliasingMode(4, 2); // 適当な値が分からん。フルスクリーン廃止したので不要

	errorCase(DxLib_Init()); // ? 失敗

	DxLibInited = 1;

	SetMouseDispFlag(0); // ? マウスを表示 1: する 0: しない
	SetWindowSizeChangeEnableFlag(0); // ウィンドウの右下をドラッグで伸縮 1: する 0: しない

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR); // BILINEARがデフォ！

	// < DxLib

	// ここで Reset と付く関数をすべて呼んでおくこと。

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
		errorCase(DeleteGraph(hdl)); // ? 失敗
		hdl = -1;
	}
}
static void DxPrv_SetScreenSize(int w, int h)
{
	int mdm = GetMouseDispMode();

	UnloadAllPicResHandle();
	SetDrawScreen(DX_SCREEN_BACK); // InnerDrawScrHdl を開放するため。
	UnloadGraph(InnerDrawScrHdl); // MakeScreen -> DeleteGraph で良いらしい。

	errorCase(SetGraphMode(w, h, 32) != DX_CHANGESCREEN_OK); // ? 失敗

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
