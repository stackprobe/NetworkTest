#include "all.h"

static PicInfo_t *LoadPic(autoList<uchar> *fileData)
{
	return Pic_GraphicHandle2PicInfo(Pic_SoftImage2GraphicHandle(Pic_FileData2SoftImage(fileData)));
}
static void UnloadPic(PicInfo_t *i)
{
	Pic_ReleasePicInfo(i);
}
oneObject(resCluster<PicInfo_t *>, CreatePicRes(LoadPic, UnloadPic), GetStdPicRes);
