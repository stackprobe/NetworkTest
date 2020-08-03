#include "all.h"

Gnd_t Gnd;

void Gnd_INIT(void)
{
	memset(&Gnd, 0x00, sizeof(Gnd_t));

	Gnd.EL = new taskList();
	Gnd.PrimaryPadId = -1;

	// SaveData -->

	Gnd.RealScreen_W = SCREEN_W;
	Gnd.RealScreen_H = SCREEN_H;

	Gnd.MusicVolume = 0.5;
	Gnd.SEVolume = 0.5;

	Gnd.PadBtnno.Dir_2 = SNWPB_DIR_2;
	Gnd.PadBtnno.Dir_4 = SNWPB_DIR_4;
	Gnd.PadBtnno.Dir_6 = SNWPB_DIR_6;
	Gnd.PadBtnno.Dir_8 = SNWPB_DIR_8;
	Gnd.PadBtnno.A = SNWPB_A1;
	Gnd.PadBtnno.B = SNWPB_B1;
	Gnd.PadBtnno.C = SNWPB_A2;
	Gnd.PadBtnno.D = SNWPB_B2;
	Gnd.PadBtnno.E = SNWPB_A3;
	Gnd.PadBtnno.F = SNWPB_B3;
	Gnd.PadBtnno.L = SNWPB_L;
	Gnd.PadBtnno.R = SNWPB_R;
	Gnd.PadBtnno.Pause = SNWPB_DSTART;

	// app -->
}
void Gnd_FNLZ(void)
{
	delete Gnd.EL;

	// app -->
}

// ---- SaveData ----

#define SAVE_FILE "SaveData.dat"
#define SAVEDATA_SIGNATURE "NetworkTest_SaveData"

static autoList<uchar> *SaveData;
static int SDIndex;

static char *SD_ReadLine(void) // ret: bind
{
	static char *line;

	memFree(line);
	line = readLine(SaveData, SDIndex);

	if(!line)
		line = strx("");

	return line;
}
static int SD_ReadBoolean(void)
{
	return atoi(SD_ReadLine()) ? 1 : 0;
}
static int SD_ReadInt(int minval, int maxval)
{
	int value = atoi(SD_ReadLine());
	m_range(value, minval, maxval);
	return value;
}
static double SD_ReadDouble(double minval, double maxval, int denom)
{
	errorCase(denom < 1);

	double value = (double)atoi(SD_ReadLine()) / denom;
	m_range(value, minval, maxval);
	return value;
}
static void SD_ReadBitList(bitList *dest)
{
	dest->Clear();

	char *line = SD_ReadLine();

	for(int index = 0; line[index]; index++)
	{
		switch(line[index])
		{
		case '0':
//			dest->PutBit(index, 0); // 不要
			break;

		case '1':
			dest->PutBit(index, 1);
			break;

		default:
			error();
		}
	}
}

static void SD_WriteLine(char *line)
{
	writeLine(SaveData, line);
}
static void SD_WriteLine_x(char *line)
{
	SD_WriteLine(line);
	memFree(line);
}
static void SD_WriteInt(int value)
{
	SD_WriteLine_x(xcout("%d", value));
}
static void SD_WriteDouble(double value, int denom)
{
	SD_WriteInt(m_d2i(value * denom));
}
static void SD_WriteBitList(bitList *src)
{
	autoList<char> *buff = new autoList<char>();

	for(int index = 0; index < src->GetList_DIRECT()->GetCount() * (int)sizeof(uint) * CHAR_BIT; index++) // todo: 最終ビット位置を返すメソッド
	{
		buff->AddElement(src->RefBit(index) ? '1' : '0');
	}
	buff->AddElement('\0');
	SD_WriteLine_x(buff->UnbindBuffer());
	delete buff;
}

void ImportSaveData(void)
{
	if(!accessible(SAVE_FILE))
		return;

	SaveData = readFile(SAVE_FILE);

#if LOG_ENABLED == 0
	if(!Jammer(SaveData, 0)) // ? セーブデータ破損
	{
		delete SaveData;
		return;
	}
#endif
	SDIndex = 0;

	if(strcmp(SD_ReadLine(), SAVEDATA_SIGNATURE) != 0) // ? セーブデータ破損
	{
		delete SaveData;
		return;
	}

	// Item {

	Gnd.RealScreen_W = SD_ReadInt(SCREEN_W, SCREEN_W_MAX);
	Gnd.RealScreen_H = SD_ReadInt(SCREEN_H, SCREEN_H_MAX);

	Gnd.MusicVolume = SD_ReadDouble(0.0, 1.0, IMAX);
	Gnd.SEVolume = SD_ReadDouble(0.0, 1.0, IMAX);

	Gnd.PadBtnno.Dir_2 = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.Dir_4 = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.Dir_6 = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.Dir_8 = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.A = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.B = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.C = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.D = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.E = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.F = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.L = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.R = SD_ReadInt(0, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnno.Pause = SD_ReadInt(0, PAD_BUTTON_MAX - 1);

	// app -->

	// }

	delete SaveData;
}
void ExportSaveData(void)
{
	SaveData = new autoList<uchar>();

	SD_WriteLine(SAVEDATA_SIGNATURE);

	// Item {

	SD_WriteInt(Gnd.RealScreen_W);
	SD_WriteInt(Gnd.RealScreen_H);

	SD_WriteDouble(Gnd.MusicVolume, IMAX);
	SD_WriteDouble(Gnd.SEVolume, IMAX);

	SD_WriteInt(Gnd.PadBtnno.Dir_2);
	SD_WriteInt(Gnd.PadBtnno.Dir_4);
	SD_WriteInt(Gnd.PadBtnno.Dir_6);
	SD_WriteInt(Gnd.PadBtnno.Dir_8);
	SD_WriteInt(Gnd.PadBtnno.A);
	SD_WriteInt(Gnd.PadBtnno.B);
	SD_WriteInt(Gnd.PadBtnno.C);
	SD_WriteInt(Gnd.PadBtnno.D);
	SD_WriteInt(Gnd.PadBtnno.E);
	SD_WriteInt(Gnd.PadBtnno.F);
	SD_WriteInt(Gnd.PadBtnno.L);
	SD_WriteInt(Gnd.PadBtnno.R);
	SD_WriteInt(Gnd.PadBtnno.Pause);

	// app -->

	// }

#if LOG_ENABLED == 0
	errorCase(!Jammer(SaveData, 1));
#endif

	writeFile_cx(SAVE_FILE, SaveData);
}
