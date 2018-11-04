#include "all.h"

static oneObject(autoList<void (*)(void)>, new autoList<void (*)(void)>(), GetFinalizers);

void addFinalizer(void (*func)(void))
{
	errorCase(!func);
	GetFinalizers()->AddElement(func);
}
void removeFinalizer(void (*func)(void))
{
	for(int index = GetFinalizers()->GetCount() - 1; 0 <= index; index--) // LIFO
	{
		if(GetFinalizers()->GetElement(index) == func)
		{
			GetFinalizers()->DesertElement(index);
			break;
		}
	}
}
void termination(int errorLevel)
{
	while(GetFinalizers()->GetCount()) // LIFO
	{
		GetFinalizers()->UnaddElement()();
	}
	exit(errorLevel);
}
void error2(char *file, int lineno, char *function)
{
	{
		static int called;

		if(called)
			exit(2);

		called = 1;
	}

	LOG("[ERROR] %s %d %s\n", file, lineno, function);

	if(DxLibInited)
	{
		printfDx("[ERROR] %s %d %s\n", file, lineno, function);

		while((IgnoreEscapeKey || CheckHitKey(KEY_INPUT_ESCAPE) == 0) && ProcessMessage() != -1)
		{
			ScreenFlip();
		}
	}
	termination(1);
}
FILE *GetLogFp(void)
{
	static FILE *fp;

	if(!fp)
		fp = fileOpen("C:\\tmp\\Game.log", "wt");

	return fp;
}

// arg >

static int ArgIndex = 1;

int hasArgs(int count)
{
	return count <= __argc - ArgIndex;
}
int argIs(char *spell)
{
	if(ArgIndex < __argc)
	{
		if(!_stricmp(__argv[ArgIndex], spell))
		{
			ArgIndex++;
			return 1;
		}
	}
	return 0;
}
char *getArg(int index)
{
	errorCase(index < 0 || __argc - ArgIndex <= index);
	return __argv[ArgIndex + index];
}
char *nextArg(void)
{
	char *arg = getArg(0);
	ArgIndex++;
	return arg;
}

// < arg

int IsWindowActive(void) // ret: ? このウィンドウはアクティブ
{
	return GetActiveFlag() ? 1 : 0;
}
static int GetNowCount_TEST(void)
{
	static uint64 baseCount;
	static int inited;

	if(!inited)
	{
		baseCount = ((uint64)0xffffffff - 20000) - (uint64)(uint)GetNowCount();
		inited = 1;
	}
	return (int)(uint)(baseCount + (uint64)(uint)GetNowCount());
}
__int64 GetCurrTime(void)
{
	static __int64 lastTime = -1;
	static __int64 baseTime;
	static uint lastCount;
	uint currCount = (uint)GetNowCount_TEST();

	if(currCount < lastCount)
	{
		baseTime += (__int64)_UI32_MAX + 1;
	}
	lastCount = currCount;
	__int64 currTime = baseTime + currCount;

	errorCase(currTime < 0); // ? カンスト(オーバーフロー)
	errorCase(currTime < lastTime); // ? 時間が戻った || カンスト(オーバーフロー)
	errorCase(lastTime != -1 && lastTime + 60000 < currTime); // ? 1分以上経過 <- 飛び過ぎ

	lastTime = currTime;
	return currTime;
}
