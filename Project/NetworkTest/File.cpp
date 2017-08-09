#include "all.h"

int accessible(char *path)
{
	return !_access(path, 0);
}
