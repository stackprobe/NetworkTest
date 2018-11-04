#include "C:\Factory\Common\Options\SockServer.h"

static int Perform(char *prmFile, char *ansFile)
{
	LOGPOS();
	copyFile(prmFile, ansFile);
copyFile(prmFile, "C:\\temp\\1.tmp");
	LOGPOS();
	return 1;
}
static int Idle(void)
{
	while(hasKey())
	{
		if(getKey() == 0x1b)
		{
			cout("Exit the server.\n");
			return 0;
		}
		cout("Press ESCAPE to exit the server.\n");
	}
	return 1;
}
int main(int argc, char **argv)
{
	sockServer(Perform, 80, 1, IMAX, Idle);
}
