#pragma once

#include <string>
#include <vector>

using namespace std;

class dialogueNode
{
private:
	string NPCDialogue;
	string playerChoices;
	vector<int> nextDialogues;

public:
	dialogueNode(string npcTalk, string playerChoiceList, vector<int> nextList);
};
