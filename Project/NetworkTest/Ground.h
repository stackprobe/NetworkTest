typedef struct Gnd_st
{
	taskList *EL; // EffectList
	int PrimaryPadId; // -1 == –¢Ý’è

	// SaveData -->

	int RealScreen_W;
	int RealScreen_H;

	/*
		‰¹—Ê, 0.0 - 1.0, def: 0.5
	*/
	double MusicVolume;
	double SEVolume;

	struct // 0 - (PAD_BUTTON_MAX - 1), def: SNWPB_*
	{
		int Dir_2;
		int Dir_4;
		int Dir_6;
		int Dir_8;
		int A;
		int B;
		int C;
		int D;
		int E;
		int F;
		int L;
		int R;
		int Pause;
	}
	PadBtnno;

	// app -->
}
Gnd_t;

extern Gnd_t Gnd;

void Gnd_INIT(void);
void Gnd_FNLZ(void);

void ImportSaveData(void);
void ExportSaveData(void);
