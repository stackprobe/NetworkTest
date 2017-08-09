#include "all.h"

int GetInput(int inpId)
{
	int padId = Gnd.PrimaryPadId;

	m_range(padId, 0, PAD_MAX); // Gnd.PrimaryPadId 未定義のとき -1 -> 0 になる。

	switch(inpId)
	{
	case INP_DIR_2:
		return GetKeyInput(KEY_INPUT_DOWN) | GetPadInput(padId, Gnd.PadBtnno.Dir_2);

	case INP_DIR_4:
		return GetKeyInput(KEY_INPUT_LEFT) | GetPadInput(padId, Gnd.PadBtnno.Dir_4);

	case INP_DIR_6:
		return GetKeyInput(KEY_INPUT_RIGHT) | GetPadInput(padId, Gnd.PadBtnno.Dir_6);

	case INP_DIR_8:
		return GetKeyInput(KEY_INPUT_UP) | GetPadInput(padId, Gnd.PadBtnno.Dir_8);

	case INP_A:
		return GetKeyInput(KEY_INPUT_Z) | GetPadInput(padId, Gnd.PadBtnno.A) /*| GetMouInput(MOUBTN_L)*/; // 決定

	case INP_B:
		return GetKeyInput(KEY_INPUT_X) | GetPadInput(padId, Gnd.PadBtnno.B) /*| GetMouInput(MOUBTN_R)*/; // キャンセル

	case INP_C:
		return GetKeyInput(KEY_INPUT_C) | GetPadInput(padId, Gnd.PadBtnno.C);

	case INP_D:
		return GetKeyInput(KEY_INPUT_V) | GetPadInput(padId, Gnd.PadBtnno.D);

	case INP_E:
		return GetKeyInput(KEY_INPUT_A) | GetPadInput(padId, Gnd.PadBtnno.E);

	case INP_F:
		return GetKeyInput(KEY_INPUT_S) | GetPadInput(padId, Gnd.PadBtnno.F);

	case INP_L:
		return GetKeyInput(KEY_INPUT_D) | GetPadInput(padId, Gnd.PadBtnno.L);

	case INP_R:
		return GetKeyInput(KEY_INPUT_LSHIFT) | GetPadInput(padId, Gnd.PadBtnno.R); // app
//		return GetKeyInput(KEY_INPUT_F) | GetPadInput(padId, Gnd.PadBtnno.R);

	case INP_PAUSE:
		return GetKeyInput(KEY_INPUT_SPACE) | GetPadInput(padId, Gnd.PadBtnno.Pause) | GetMouInput(MOUBTN_M);

	default:
		error();
	}
	error();
	return -1; // dummy
}
int GetPound(int inpId)
{
	return isPound(GetInput(inpId));
}
