double toDouble(char *line);
char *xcout(char *format, ...);
char *strx(char *line);
int isPound(int counter);
int atoi_x(char *line);
autoList<char *> *tokenize(char *line, char *delimiters);

template <class Var_t>
void swapVar(Var_t &a, Var_t &b)
{
	Var_t tmp = a;
	a = b;
	b = tmp;
}

int getZero(void);
uint getUIZero(void);
__int64 getI64Zero(void);
void noop(void);
void noop_i(int dummy);
void noop_ui(uint dummy);
void noop_i64(__int64 dummy);

i2D_t makeI2D(int x, int y);
d2D_t makeD2D(double x, double y);

bitList *createBitList(void);
void releaseBitList(bitList *bl);

void my_memset(void *block, int fillValue, int size);

template<class Element_t>
void gnomeSort(autoList<Element_t> *list, int (*comp)(Element_t, Element_t))
{
	for(int rndx = 1; rndx < list->GetCount(); rndx++)
		for(int lndx = rndx; lndx && 0 < comp(list->GetElement(lndx - 1), list->GetElement(lndx)); lndx--)
			list->Swap(lndx - 1, lndx);
}
template<class Element_t>
void combSort(autoList<Element_t> *list, int (*comp)(Element_t, Element_t))
{
	int span = list->GetCount();

	for(; ; )
	{
		span *= 10;
		span /= 13;

		if(span < 2)
			break;

		if(span == 9 || span == 10)
			span = 11;

		for(int lidx = 0, ridx = span; ridx < list->GetCount(); lidx++, ridx++)
			if(0 < comp(list->GetElement(lidx), list->GetElement(ridx)))
				list->Swap(lidx, ridx);
	}
	gnomeSort(list, comp);
}
