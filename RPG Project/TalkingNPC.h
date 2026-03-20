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
	//constructor
	talkingNPC(int treeSize);
	//recursive function thats takes the player through a conversation
	void printDialouge(int currentDialogue);

};