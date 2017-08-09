#include "all.h"

char *addChar(char *str, int chr) // ret: strr(str)
{
	char *ret = xcout("%s%c", str, chr);
	memFree(str);
	return ret;
}
char *unbindToString(autoList<uchar> *buff_bind)
{
	buff_bind->AddElement('\0');
	char *str = (char *)buff_bind->UnbindBuffer();
	delete buff_bind;
	return str;
}
void toAsciiString(char *str)
{
	for(char *p = str; *p; p++)
		if(!m_isRange(*p, '\x20', '\x7e'))
			*p = '?';
}
autoList<uchar> *stringToBlock(char *str)
{
	return bindString(strx(str));
}
autoList<uchar> *bindString(char *str_bind)
{
	return new autoList<uchar>((uchar *)str_bind, strlen(str_bind));
}
int c2i(int chr)
{
	if('a' <= chr) return chr - 'a' + 10;
	if('A' <= chr) return chr - 'A' + 10;
	if('0' <= chr) return chr - '0';
	return -1;
}
void hexStringToBlock(char *str, void *dest)
{
	errorCase(!str);
	errorCase(strlen(str) % 2 == 1);
	errorCase(!dest);

	autoList<uchar> *buff = new autoList<uchar>();

	for(int index = 0; str[index * 2]; index++)
	{
		((uchar *)dest)[index] = c2i(str[index * 2]) * 16 + c2i(str[index * 2 + 1]);
	}
}
