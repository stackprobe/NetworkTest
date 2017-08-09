#include "all.h"

static uint GetSizeImage(uint xSize, uint ySize)
{
	return ((xSize * 3 + 3) / 4) * 4 * ySize;
}
void writeBmpFile(char *file, autoTable<uint> *bmp)
{
	autoList<uchar> *fileData = new autoList<uchar>();

	int w = bmp->GetWidth();
	int h = bmp->GetHeight();

	// Bfh
	fileData->AddElement('B');
	fileData->AddElement('M');
	writeUI32(fileData, GetSizeImage(w, h) + 0x36);
	writeUI32(fileData, 0); // Reserved_01 + Reserved_02
	writeUI32(fileData, 0x36);

	// Bfi
	writeUI32(fileData, 0x28);
	writeUI32(fileData, w);
	writeUI32(fileData, h);
	writeUI32(fileData, 0x00180001); // Planes + BitCount
	writeUI32(fileData, 0);
	writeUI32(fileData, GetSizeImage(w, h));
	writeUI32(fileData, 0);
	writeUI32(fileData, 0);
	writeUI32(fileData, 0);
	writeUI32(fileData, 0);

	for(int y = h - 1; 0 <= y; y--)
	{
		for(int x = 0; x < w; x++)
		{
			uint color = bmp->GetCell(x, y);
			uchar cR;
			uchar cG;
			uchar cB;

			cR = color >> 16;
			cG = color >> 8;
			cB = color;

			// BGR ’ˆÓ
			fileData->AddElement(cB);
			fileData->AddElement(cG);
			fileData->AddElement(cR);
		}
		for(int x = w % 4; x; x--)
		{
			fileData->AddElement(0x00);
		}
	}
	writeFile_cx(file, fileData);
}
void writeBmpFile_cx(char *file, autoTable<uint> *bmp)
{
	writeBmpFile(file, bmp);
	delete bmp;
}
void writeBmpFile_xx(char *file, autoTable<uint> *bmp)
{
	writeBmpFile_cx(file, bmp);
	memFree(file);
}
