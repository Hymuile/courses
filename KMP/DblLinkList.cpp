#include<iostream>
#include<string>

enum class statusCode {
	RANGE_ERROR, SUCCESS,ENTRY_FOUND
};

template<typename T>
struct DblNode
{
	struct DblNode<T>* back;
	std::string data;
	struct DblNode<T>* next;

	DblNode() {};
	DblNode(T, DblNode<T>* linkback = NULL, DblNode<T>* linknext = NULL);
};

template<typename T>
DblNode<T>::DblNode(T elem, DblNode<T>* linkback, DblNode<T>* linknext)
{
	data = elem;
	back = linkback;
	next = linknext;
}

template<typename T>
class DblLinkList {
protected:
	DblNode<T>* head;
	mutable int curPosition;
	mutable DblNode<T>* curPtr;
	int count;

	DblNode<T>* GetElemPtr(int position) const;
	void Init();

public:
	DblLinkList();
	virtual ~DblLinkList();
	int Length() const;
	bool Empty() const;
	void Clear();

	statusCode GetElem(int position, T& e) const;
	statusCode SetElem(int position, T& e) const;
	statusCode Delete(int position, T& e);
	statusCode Insert(int position, const T& e);
	void Traverse() const;
	DblLinkList(const DblLinkList<T>& copy);
	DblLinkList<T>& operator=(const DblLinkList<T>& copy);
};

template<typename T>
DblNode<T>* DblLinkList<T>::GetElemPtr(int position) const
{
	if (curPosition < position)
		for (; curPosition < position; curPosition++)
			curPtr = curPtr->next;
	else if (curPosition > position)
		for (; curPosition > position; curPosition--)
			curPtr = curPtr->back;
	return curPtr;
}

template<typename T>
void DblLinkList<T>::Init()
{
	head = new DblNode<T>;
	head->next = head;
	curPtr = head; curPosition = 0;
	count = 0;
}

template<typename T>
DblLinkList<T>::DblLinkList() { Init(); }

template<typename T>
DblLinkList<T>::~DblLinkList()
{
	Clear();
	delete head;
}

template<typename T>
int DblLinkList<T>::Length() const 
{ 
	return count;
}

template<typename T>
bool DblLinkList<T>::Empty() const 
{
	return head->next == NULL;
}

template<typename T>
void DblLinkList<T>::Clear()
{
	T tmpElem;
	while (Length() > 0)
		Delete(1, tmpElem);
}

template<typename T>
statusCode DblLinkList<T>::GetElem(int position, T& e) const
{
	if (position<1 || position>Length())
		return statusCode::RANGE_ERROR;
	else {
		DblNode<T>* tmpPtr;
		tmpPtr = GetElemPtr(position);
		e = tmpPtr->data;
		return statusCode::ENTRY_FOUND;
	}
}

template<typename T>
statusCode DblLinkList<T>::SetElem(int position, T& e) const
{
	if (position<1 || position>Length())
		return statusCode::RANGE_ERROR;
	else {
		DblNode<T>* tmpPtr;
		tmpPtr = GetElemPtr(position);
		tmpPtr->data = e;
		return statusCode::SUCCESS;
	}
}

template<typename T>
statusCode DblLinkList<T>::Delete(int position, T& e)
{
	if (position<1 || position>Length())
		return statusCode::RANGE_ERROR;
	else {
		DblNode<T>* tmpPtr = new DblNode<T>;
		tmpPtr = GetElemPtr(position - 1);
		tmpPtr = tmpPtr->next;
		tmpPtr->back->next = tmpPtr->next;
		tmpPtr->next->back = tmpPtr->back;
		e = tmpPtr->data;
		if (position == Length()) {
			curPosition = 0;
			curPtr = head;
		}
		else {
			curPosition = position;
			curPtr = tmpPtr->next;
		}
		count--;
		delete tmpPtr;
		return statusCode::SUCCESS;
	}
}

template<typename T>
statusCode DblLinkList<T>::Insert(int position, const T& e)
{
	if (position<1 || position>Length() + 1)
		return statusCode::RANGE_ERROR;
	else {
		DblNode<T>* tmpPtr, * nextPtr, * newPtr;
		tmpPtr = GetElemPtr(position - 1);
		nextPtr = tmpPtr->next;
		newPtr = new DblNode<T>(e, tmpPtr, nextPtr);
		tmpPtr->next = newPtr;
		nextPtr->back = newPtr;
		curPosition = position;
		curPtr = newPtr;
		count++;
		return statusCode::SUCCESS;
	}
}

template<typename T>
void DblLinkList<T>::Traverse() const
{
	std::string tmpElem;
	for (size_t i = 1; i <= Length(); i++){
		GetElem(i, tmpElem);
		std::cout << tmpElem<<std::endl;
	}
}

template<typename T>
DblLinkList<T>::DblLinkList(const DblLinkList<T>& copy)
{
	int copyLength = copy.Length();
	T e;
	head = NULL;
	Init();
	for (int i = 1; i <= copyLength; i++){
		copy.GetElem(i, e);
		Insert(Length() + 1, e);
	}
}

template<typename T>
DblLinkList<T>& DblLinkList<T>::operator=(const DblLinkList<T>& copy)
{
	if (&copy != this){
		int copyLength = copy.Length();
		T e;
		Init();
		for (int i = 1; i <= copyLength; i++) {
			copy.GetElem(i, e);
			Insert(Length() + 1, e);
		}
	}
	return *this;
}
