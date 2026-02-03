#pragma once
#include <iostream>

template<class T>
class itemNode
{
private:
	T item;
	itemNode* prevItem;
	itemNode* nextItem;
public:
	itemNode(T i)
	{
		item = i;
		prevItem = nullptr;
		nextItem = nullptr;
	}
	T getItem()
	{
		return item;
	}
	void setPrev(itemNode<T>* prevI)
	{
		prevItem = prevI;
	}
	void setNext(itemNode<T>* setNext)
	{
		nextItem = setNext;
	}
	itemNode<T>* getNext()
	{
		return nextItem;
	}
	itemNode<T>* getPrev()
	{
		return prevItem;
	}
};

template<class T>
class DLL
{
private:
	int size;
	itemNode<T>* index;
	itemNode<T>* beg;
	itemNode<T>* end;
public:
	DLL(itemNode<T>* first)
	{
		index = first;
		beg = first;
		end = first;
		size = 1;
	}
	itemNode<T> getCurrent()
	{
		return *index;
	}
	int getSize()
	{
		return size;
	}
	void goToNext()
	{
		index = (*index).getNext();
	}
	void goToPrev()
	{
		index = (*index).getPrev();
	}
	void insertAtLocation(int i, itemNode<T>* s)
	{
		for (int c = 1; c < i - 1; c++)
		{
			goToNext();
		}
		(*s).setPrev(index);
		(*s).setNext((*index).getNext());
		(*(*index).getNext()).setPrev(s);
		(*index).setNext(s);
		size++;
		index = beg;
	}
	void insertAtBeg(itemNode<T>* s)
	{
		if (size == 0)
		{
			index = s;
			beg = s;
			end = s;
			size++;
		}
		else
		{
			(*s).setNext(beg);
			(*beg).setPrev(s);
			beg = s;
			index = s;
			size++;
		}
	}
	void insertAtEnd(itemNode<T>* s)
	{
		(*s).setPrev(end);
		(*end).setNext(s);
		end = s;
		index = beg;
		size++;
	}
	void deleteAtBeg()
	{
		if (size == 1)
		{
			index = nullptr;
			beg = nullptr;
			end = nullptr;
			size--;
		}
		else
		{
			goToNext();
			(*index).setPrev(nullptr);
			beg = index;
			index = beg;
			size--;
		}
	}
	void deleteAtEnd()
	{

		index = end;
		goToPrev();
		(*index).setNext(nullptr);
		end = index;
		index = beg;
		size--;
	}

	void deleteAtLocation(int i)
	{
		for (int c = 1; c < i - 1; c++)
		{
			goToNext();
		}
		itemNode* s = index->getNext();
		(*(*s).getNext()).setPrev(index);
		(*index).setNext((*s).getNext());
		index = beg;
		size--;
	}
};

