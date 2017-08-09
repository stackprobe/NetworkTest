#include "all.h"

static void DrawWall(void)
{
	DrawCurtain();
//	DrawRect(P_WHITEBOX, 100, 100, SCREEN_W - 200, SCREEN_H - 200);
}

// ---- Block ----

#define BLOCK_MAX 500

static double BlockPosCube[BLOCK_MAX][2][2]; // [ブロック][位置, 速度][X, Y]

static void DrawBlocksInit(void)
{
	for(int b = 0; b < BLOCK_MAX; b++)
	for(int c = 0; c < 2; c++)
	for(int d = 0; d < 2; d++)
	{
#if 1
		BlockPosCube[b][c][d] = c == 0 ? 300.0 + eRnd() * eRnd() * eRnd() * 5.0 : eRnd() * eRnd() * eRnd() * 5.0;
#else
		BlockPosCube[b][c][d] = 0.0;
#endif
	}
}
static void DrawBlocks(void)
{
	for(int b = 0; b < BLOCK_MAX; b++)
	{
		double b_rate = (double)b / BLOCK_MAX;

		BlockPosCube[b][0][0] += BlockPosCube[b][1][0];
		BlockPosCube[b][0][1] += BlockPosCube[b][1][1];

		{
			const double MARGIN = 50.0;
			const double X_MIN = -MARGIN;
			const double Y_MIN = -MARGIN;
			const double X_MAX = SCREEN_W + MARGIN;
			const double Y_MAX = SCREEN_H + MARGIN;
			int warped = 0;

			if(BlockPosCube[b][0][0] < X_MIN) { BlockPosCube[b][0][0] = X_MAX; warped = 1; }
			if(BlockPosCube[b][0][1] < Y_MIN) { BlockPosCube[b][0][1] = Y_MAX; warped = 1; }
			if(X_MAX < BlockPosCube[b][0][0]) { BlockPosCube[b][0][0] = X_MIN; warped = 1; }
			if(Y_MAX < BlockPosCube[b][0][1]) { BlockPosCube[b][0][1] = Y_MIN; warped = 1; }

			if(warped)
			{
				BlockPosCube[b][1][0] *= 0.9;
				BlockPosCube[b][1][1] *= 0.9;
			}
		}

		BlockPosCube[b][1][0] += eRnd() * 0.1;
		BlockPosCube[b][1][1] += eRnd() * 0.1;

		BlockPosCube[b][1][0] += cos(ActFrame / 503.0) * 0.01;
		BlockPosCube[b][1][1] += sin(ActFrame / 607.0) * 0.01;

		const double SPEED_MAX = 10.0;

		m_range(BlockPosCube[b][1][0], -SPEED_MAX, SPEED_MAX);
		m_range(BlockPosCube[b][1][1], -SPEED_MAX, SPEED_MAX);

#if 1
#if 0
		DPE_SetAlpha(1.0 - b_rate * 0.9);
#else
		DPE_SetAlpha(0.1 + b_rate * 0.9);
#endif
#if 0
		DrawBegin(P_BLOCK_00 + (6 + b % 6), BlockPosCube[b][0][0], BlockPosCube[b][0][1]);
#else
		DrawBegin(P_BLOCK_00 + b % 12, BlockPosCube[b][0][0], BlockPosCube[b][0][1]);
#endif
		DrawZoom(1.1 + b_rate * 0.9);
		DrawRotate((BlockPosCube[b][0][0] + BlockPosCube[b][0][1]) * 0.01);
		DrawEnd();
		DPE_Reset();
#else // old
		DrawBegin(P_BLOCK_00 + b % 12, BlockPosCube[b][0][0], BlockPosCube[b][0][1]);
		DrawZoom(1.1 + b_rate * 0.9);
		DrawRotate((BlockPosCube[b][0][0] + BlockPosCube[b][0][1]) * 0.01);
		DrawEnd();
#endif
	}
}

// ----

void EchoTestMain(void)
{
	char *lastRecvMessage = strx("NONE");
	int recvCount = 0;
	int sendCount = 0;

	DrawBlocksInit();

	SetCurtain(120, 0.0, -1.0);
	FreezeInput();
	ActFrame = 0;

	MusicPlay(MUS_BGM);

	for(; ; )
	{
		if(GetInput(INP_PAUSE) == 1)
			break;

		if(GetInput(INP_A) == 1)
		{
			SockSend(stringToBlock("HELLO"));
			sendCount++;
		}

		if(GetInput(INP_B) == 1)
		{
			autoList<uchar> *data = new autoList<uchar>();

			data->AddElements((uchar *)"[a x 10000]:", 12);

			for(int c = 0; c < 10000; c++)
				data->AddElement('a');

			SockSend(data);
			sendCount++;
		}

		// 受信
		{
			autoList<uchar> *recvData = SockRecv();

			if(recvData)
			{
				memFree(lastRecvMessage);
				lastRecvMessage = unbindToString(recvData);
				toAsciiString(lastRecvMessage);
				recvCount++;

				{
					const int LEN_MAX = 30;

					if(LEN_MAX < strlen(lastRecvMessage))
						strcpy(lastRecvMessage + LEN_MAX - 4, " ...");
				}
			}
		}

		DrawWall();
		DrawBlocks();
		
		DPE_SetAlpha(0.7);
		DPE_SetBright(0, 0, 0);
		DrawRect(P_WHITEBOX, 0, 0, SCREEN_W * 0.4, SCREEN_H);
		DPE_Reset();

		SetPrint();
		PE_Border();

		Print("INFO_MESSAGE: ");
		Print(GetSockInfoMessage());
		PrintRet();

		Print("ERROR_MESSAGE: ");
		Print(GetSockErrorMessage());
		PrintRet();

		Print_x(xcout("SEND_COUNT: %d", sendCount));
		PrintRet();

		Print("LAST_RECV_MESSAGE: [");
		Print(lastRecvMessage);
		Print("]");
		PrintRet();

		Print_x(xcout("RECV_COUNT: %d", recvCount));
		PrintRet();

		PE_Reset();

		EachFrame();
	}
	FreezeInput();
	MusicFade();
	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawWall();
		EachFrame();
	}
	sceneLeave();

	memFree(lastRecvMessage);
}
