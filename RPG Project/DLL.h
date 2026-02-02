#pragma once
#include <iostream>
#include "ItemNode.h"
template<class T>
class DLL
{
private:
	int size;
	itemNode* index;
	itemNode* beg;
	itemNode* end;
public:
	DLL(itemNode* first);
	itemNode getCurrent();
	int getSize();
	void goToNext();
	void goToPrev();
	void insertAtLocation(int i, itemNode* s);
	void insertAtBeg(itemNode* s);
	void insertAtEnd(itemNode* s);
	void deleteAtBeg();
	void deleteAtEnd();
	void deleteAtLocation(int i);
};