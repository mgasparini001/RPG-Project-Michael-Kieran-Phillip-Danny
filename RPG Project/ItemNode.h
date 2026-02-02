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
	itemNode(T i);
	T getItem();
	void setPrev(T prevItem);
	void setNext(T setNext);
	itemNode* getNext();
	itemNode* getPrev();
};