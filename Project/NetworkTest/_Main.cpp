/*
	float, double をゼロクリアした値および広域の初期値は 0.0 とする。
	float, double の計算結果は環境によって誤差があるものとする。

	----

	フレームループの基本形

//	ActFrame = 0; // 必要ならば
	FreezeInput();

	for(; ; )
	{
		1. 処理
			ループの脱出
			割り込み処理
				離脱処理・再入処理はここで行う？
				FreezeInput();
		2. 描画
		3. EachFrame();
	}
//	FreezeInput(); // 念のため
*/

#include "all.h"

void ProcMain(void)
{
#if !LOG_ENABLED
	EchoTestMain();
#elif 1 // test
	EchoTestMain();
#else // dummy
	for(; ; )
	{
		if(GetInput(INP_A) == 1)
			break;

		EachFrame();
	}
#endif
}
