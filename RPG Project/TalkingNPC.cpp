#include "TalkingNPC.h"

void refreshScreen();


talkingNPC::talkingNPC(int treeSize)
{
	for (int i = 1; i <= treeSize; i++)
	{
		dialogueTree.push_back(NULL);
	}
}

void talkingNPC::printDialouge(int currentDialogue)
{
	refreshScreen();
	dialogueNode* current = dialogueTree.at(currentDialogue);
	int choice;
	std::cout << name << ":" << std::endl;
	std::cout << current->getNPCDialogue() << std::endl;
	std::cout << current->getPlayerChoices() << std::endl;
	if (current->getPlayerChoices() == "Press enter to continue")
	{
		std::cin.ignore();
		std::cin.get();
		refreshScreen();
	}
	else
	{
		bool valid = false;
		while (!valid)
		{
			string test = getValidInput();
			if (test != "fail")
			{
				choice = stoi(test);
				if (choice < 0 || choice > current->getNextDialogues().size() - 1)
				{
					std::cout << "Could you repeat that?" << std::endl;
				}
				else
				{
					valid = true;
				}
			}
		}
		printDialouge(current->getNextDialogues().at(choice) - 1);
	}
	
}