typedef struct CommonEffectExtra_st
{
	int LastPicId; // PicId Ç©ÇÁ LastPicId Ç‹Ç≈ï\é¶ÇµÇƒèIóπÇ∑ÇÈ, 0 == ñ≥å¯
	double SlideX;
	double SlideY;
	double SlideX_B;
	double SlideY_B;
	double R_B;
	double Z_B;
	double SlideX_F;
	double SlideY_F;
	double R_F;
	double Z_F;
}
CommonEffectExtra_t;

extern CommonEffectExtra_t CEE;

extern double CameraX;
extern double CameraY;

void AddCommonEffect(
	taskList *tl,
	int top_mode,
	int picId,
	double x, double y, double r = 0.0, double z = 1.0, double a = 1.0,
	double x_add = 0.0, double y_add = 0.0, double r_add = 0.0, double z_add = 0.0, double a_add = 0.0,
	double x_add2 = 0.0, double y_add2 = 0.0, double r_add2 = 0.0, double z_add2 = 0.0, double a_add2 = 0.0
	);

void CEE_Reset(void);
