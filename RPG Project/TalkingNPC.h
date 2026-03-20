#pragma once
#include "dialogueNode.h"
#include "errorChecking.h"
#include <iostream>
#include <string>
#include <vector>

class talkingNPC
{
protected:
	std::string name;
	vector<dialogueNode*> dialogueTree;
public:
	talkingNPC(int treeSize);
	void printDialouge(int currentDialogue);

};