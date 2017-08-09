template <class Element_t>
class sortedList
{
private:
	autoList<Element_t> *List;
	int (*Comp)(Element_t, Element_t);
	int Sorted;

	void TrySort()
	{
		if(!this->Sorted)
		{
			this->DoSort();
			this->Sorted = 1;
		}
	}
	void DoSort()
	{
		combSort(this->List, this->Comp);
	}

public:
	sortedList(int (*comp)(Element_t, Element_t))
	{
		this->List = new autoList<Element_t>();
		this->Comp = comp;
		this->Sorted = 0;
	}
	sortedList(int (*comp)(Element_t, Element_t), autoList<Element_t> bind_list, int sorted = 0)
	{
		this->List = bind_list;
		this->Comp = comp;
		this->Sorted = sorted;
	}
	sortedList(const sortedList &source)
	{
		error();
	}
	~sortedList()
	{
		delete this->List;
	}

	autoList<Element_t> unbindList()
	{
		autoList<Element_t> unbind_list = this->List;
		this->List = new autoList<Element_t>();
		return unbind_list;
	}

	void AddElement(Element_t element)
	{
		this->List->AddElement(element);
		this->Sorted = 0;
	}
	Element_t GetElement(int index)
	{
		this->TrySort();
		return this->List->GetElement(index);
	}
	Element_t GetElement(Element_t ferret, Element_t defval)
	{
		int index = this->GetIndex(ferret);

		if(index == -1)
			return defval;

		return this->List->GetElement(index);
	}
	int GetIndex(Element_t ferret)
	{
		this->TrySort();

		int lidx = -1;
		int ridx = this->List->GetCount();

		while(lidx + 1 < ridx)
		{
			int midx = (lidx + ridx) / 2;
			int ret = this->Comp(ferret, this->List->GetElement(midx));

			if(ret == 0)
				return midx;

			if(ret < 0)
				ridx = midx;
			else
				lidx = midx;
		}
		return -1;
	}
};
