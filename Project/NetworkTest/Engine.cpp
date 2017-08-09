#include "all.h"

int IgnoreEscapeKey;

// 他ファイルからは read only {
__int64 FrameStartTime;
int ProcFrame;
int ActFrame;
int FreezeInputFrame;
int WindowIsActive;
int DropCount;
int NoDropCount;
// }

static void CheckHz(void)
{
	__int64 currTime = GetCurrTime();
	__int64 diffTime = currTime - FrameStartTime;

	if(diffTime < 15 || 18 < diffTime) // ? frame rate drop
		DropCount++;
	else
		NoDropCount++;

	FrameStartTime = currTime;
}
int InnerDrawScrHdl = -1;

void EachFrame(void)
{
	doThreads();

	if(!SEEachFrame())
	{
		MusicEachFrame();
	}
	Gnd.EL->ExecuteAllTask();
	CurtainEachFrame();

	{
		int &scrHdl = InnerDrawScrHdl;

		if(scrHdl != -1)
		{
			SetDrawScreen(DX_SCREEN_BACK);
			errorCase(DrawExtendGraph(0, 0, Gnd.RealScreen_W, Gnd.RealScreen_H, scrHdl, 0)); // ? 失敗
		}
	}

	// DxLib >

	ScreenFlip();

	if(!IgnoreEscapeKey && CheckHitKey(KEY_INPUT_ESCAPE) == 1 || ProcessMessage() == -1)
	{
		EndProc();
	}

	// < DxLib

	CheckHz();

	ProcFrame++;
	errorCase(IMAX < ProcFrame); // およそ 192.9 日後にカンスト
	ActFrame++;
	m_countDown(FreezeInputFrame);
	WindowIsActive = IsWindowActive();

	PadEachFrame();
	KeyEachFrame();
	MouseEachFrame();

	if(Gnd.RealScreen_W != SCREEN_W || Gnd.RealScreen_H != SCREEN_H)
	{
		int &scrHdl = InnerDrawScrHdl;

		if(scrHdl == -1)
		{
			scrHdl = MakeScreen(SCREEN_W, SCREEN_H);
			errorCase(scrHdl == -1); // ? 失敗
		}
		SetDrawScreen(scrHdl);
	}
}
void FreezeInput(int frame) // frame: 1 == このフレームのみ, 2 == このフレームと次のフレーム ...
{
	errorCase(frame < 1 || IMAX < frame);
	FreezeInputFrame = frame;
}
