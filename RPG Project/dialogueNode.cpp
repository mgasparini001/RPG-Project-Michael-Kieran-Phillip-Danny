#include "dialogueNode.h"

dialogueNode::dialogueNode(string npcTalk, string playerChoiceList, vector<int> nextList)
	: NPCDialogue(npcTalk), playerChoices(playerChoiceList), nextDialogues(nextList)
{
}
