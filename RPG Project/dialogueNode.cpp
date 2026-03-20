#include "dialogueNode.h"

dialogueNode::dialogueNode(string npcTalk, string playerChoiceList, vector<int> nextList)
	: NPCDialogue(npcTalk), playerChoices(playerChoiceList), nextDialogues(nextList)
{
}

string dialogueNode::getNPCDialogue() const
{
	return NPCDialogue;
}

string dialogueNode::getPlayerChoices() const
{
	return playerChoices;
}

vector<int> dialogueNode::getNextDialogues() const
{
	return nextDialogues;
}
