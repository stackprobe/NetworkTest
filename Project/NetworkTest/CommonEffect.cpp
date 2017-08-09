#include "all.h"

typedef struct CommonEffect_st
{
	int PicId;
	CommonEffectExtra_t Extra;
	double X;
	double Y;
	double R;
	double Z;
	double A;
	double XAdd;
	double YAdd;
	double RAdd;
	double ZAdd;
	double AAdd;
	double XAdd2;
	double YAdd2;
	double RAdd2;
	double ZAdd2;
	double AAdd2;
}
CommonEffect_t;

double CameraX;
double CameraY;

static int CommonEffectFunc(CommonEffect_t *i)
{
	double drawX = i->X - CameraX;
	double drawY = i->Y - CameraY;

	DPE_SetAlpha(i->A);
	DrawBegin(i->PicId, drawX, drawY);

	DrawSlide(i->Extra.SlideX_B, i->Extra.SlideY_B);
	DrawRotate(i->Extra.R_B);
	DrawZoom(i->Extra.Z_B);

	DrawSlide(i->Extra.SlideX, i->Extra.SlideY);
	DrawRotate(i->R);
	DrawZoom(i->Z);

	DrawSlide(i->Extra.SlideX_F, i->Extra.SlideY_F);
	DrawRotate(i->Extra.R_F);
	DrawZoom(i->Extra.Z_F);

	DrawEnd();
	DPE_Reset();

	i->X += i->XAdd;
	i->Y += i->YAdd;
	i->R += i->RAdd;
	i->Z += i->ZAdd;
	i->A += i->AAdd;

	i->XAdd += i->XAdd2;
	i->YAdd += i->YAdd2;
	i->RAdd += i->RAdd2;
	i->ZAdd += i->ZAdd2;
	i->AAdd += i->AAdd2;

	if(i->Extra.LastPicId)
	{
		i->PicId++;

		if(i->Extra.LastPicId < i->PicId)
			return 0;
	}

	// ? out of screen
	if(
		drawX < -(double)SCREEN_W || SCREEN_W * 2.0 < drawX ||
		drawY < -(double)SCREEN_H || SCREEN_H * 2.0 < drawY
		)
		return 0;

	return 0.0 < i->A;
}
static void CommonEffectReleaseParam(CommonEffect_t *i)
{
	memFree(i);
}
CommonEffectExtra_t CEE;

void AddCommonEffect(
	taskList *tl,
	int top_mode,
	int picId,
	double x, double y, double r, double z, double a,
	double x_add, double y_add, double r_add, double z_add, double a_add,
	double x_add2, double y_add2, double r_add2, double z_add2, double a_add2
	)
{
	CommonEffect_t *i = (CommonEffect_t *)memAlloc_NC(sizeof(CommonEffect_t));

	i->PicId = picId;
	i->Extra = CEE;

	i->X = x;
	i->Y = y;
	i->R = r;
	i->Z = z;
	i->A = a;

	i->XAdd = x_add;
	i->YAdd = y_add;
	i->RAdd = r_add;
	i->ZAdd = z_add;
	i->AAdd = a_add;

	i->XAdd2 = x_add2;
	i->YAdd2 = y_add2;
	i->RAdd2 = r_add2;
	i->ZAdd2 = z_add2;
	i->AAdd2 = a_add2;

	AddTask(tl, top_mode, CommonEffectFunc, i, CommonEffectReleaseParam);
}
void CEE_Reset(void)
{
	memset(&CEE, 0x00, sizeof(CommonEffectExtra_t));

	CEE.Z_B = 1.0;
	CEE.Z_F = 1.0;
}
