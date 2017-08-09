#include "all.h"

int KeepThread;
static autoList<int> *ThreadHdls;
static CRITICAL_SECTION CSec;
static thread_tls int CSecIn;

void critical(void)
{
	if(!CSecIn)
	{
		EnterCriticalSection(&CSec);
		CSecIn = 1;
	}
}
void uncritical(void)
{
	if(CSecIn)
	{
		LeaveCriticalSection(&CSec);
		CSecIn = 0;
	}
}
static void DoSleep(int millis) // millis: 0 == �������Ȃ�
{
	while(KeepThread && 0 < millis)
	{
		int m = m_min(millis, 15);

		Sleep(m);
		millis -= m;
	}
}
void doThreads(int millis)
{
	if(CSecIn)
	{
		uncritical();
		DoSleep(millis); // Sleep(0);���ƁA�ʃX���b�h�� critical() �ɐ��䂪�n��Ȃ��BSleep(1);�Ȃ�ok
		critical();
	}
}

static void Fnlz(void)
{
	uncritical();

	KeepThread = 0;

	while(ThreadHdls->GetCount())
	{
		HANDLE hdl = (HANDLE)ThreadHdls->UnaddElement();

		WaitForSingleObject(hdl, INFINITE);
		CloseHandle(hdl);
	}
}
static void Init(void)
{
	// once
	{
		static int inited;

		if(inited)
			return;

		inited = 1;
	}

	KeepThread = 1;
	ThreadHdls = new autoList<int>();
	InitializeCriticalSection(&CSec);

	critical();
	addFinalizer(Fnlz);
}
static uint __stdcall Perform(void *prm)
{
	critical();
	((void (*)(void))prm)();
	uncritical();

	return 0;
}
void StartThread(void (*func)(void))
{
	errorCase(!func);

	Init();

	int hdl = (int)_beginthreadex(0, 0, Perform, (void *)func, 0, 0);
	errorCase(!hdl);

	ThreadHdls->AddElement(hdl);
}
