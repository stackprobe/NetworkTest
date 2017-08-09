#include "all.h"

#define MULTIVAL 157
#define POWERVAL 31

static void AddTrailZero(autoList<uchar> *block, int tznum)
{
	for(int count = 0; count < tznum; count++)
	{
		block->AddElement(0);
	}
}
static int UnaddTrailZero(autoList<uchar> *block)
{
	int tznum = 0;

	while(block->GetCount() && block->GetElement(block->GetCount() - 1) == 0)
	{
		block->UnaddElement();
		tznum++;
	}
	return tznum;
}
static int SingleJammer(autoList<uchar> *block, int encodeFlag)
{
	uint tznum = UnaddTrailZero(block);

	if(block->GetCount() == 0)
	{
		if(encodeFlag)
		{
			tznum++;
		}
		else
		{
			if(!tznum)
				return 0;

			tznum--;
		}
	}
	else if(encodeFlag)
	{
		int value = 0;

		for(int index = 0; index < block->GetCount() || value; index++)
		{
			value += block->RefElement(index, 0) * MULTIVAL;
			block->PutElement(index, value & 0xff, 0);
			value >>= 8;
		}
	}
	else
	{
		int value = 0;

		for(int index = block->GetCount() - 1; 0 <= index; index--)
		{
			value |= block->GetElement(index);
			block->SetElement(index, value / MULTIVAL);
			value %= MULTIVAL;
			value <<= 8;
		}
		if(value)
			return 0;

		UnaddTrailZero(block);
	}
	AddTrailZero(block, tznum);
	return 1;
}
int Jammer(autoList<uchar> *block, int encodeFlag) // ret: ? ê¨å˜
{
	for(int count = 0; count < POWERVAL; count++)
		if(!SingleJammer(block, encodeFlag))
			return 0;

	return 1;
}
