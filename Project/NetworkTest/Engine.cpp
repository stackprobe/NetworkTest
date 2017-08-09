#include "all.h"

int IgnoreEscapeKey;

// ���t�@�C������� read only {
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
			errorCase(DrawExtendGraph(0, 0, Gnd.RealScreen_W, Gnd.RealScreen_H, scrHdl, 0)); // ? ���s
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
	errorCase(IMAX < ProcFrame); // ���悻 192.9 ����ɃJ���X�g
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
			errorCase(scrHdl == -1); // ? ���s
		}
		SetDrawScreen(scrHdl);
	}
}
void FreezeInput(int frame) // frame: 1 == ���̃t���[���̂�, 2 == ���̃t���[���Ǝ��̃t���[�� ...
{
	errorCase(frame < 1 || IMAX < frame);
	FreezeInputFrame = frame;
}
