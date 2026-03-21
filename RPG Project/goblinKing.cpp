#include "GoblinKing.h"

void refreshScreen();

goblinKing::goblinKing(int treeSize) : talkingNPC(treeSize)
{
	name = "The Goblin King";
	vector<int> choices1 = { 2,6,7,9 };
	vector<int> choices2 = { 3,6,7,9 };
	vector<int> choices3 = { 4,6,7,9 };
	vector<int> choices4 = { 5,7,9 };
	vector<int> choices5;
	vector<int> choices6;
	vector<int> choices7 = { 8,9 };
	vector<int> choices8;
	vector<int> choices9 = { 10,1 };
	vector<int> choices10;

		
		
	dialogueNode* n1 = new dialogueNode("Ello, wut do we ave ere ?", "0) ...... (Say Nothing)\n1) You stupid goblin!Free me at once and I'll show you what I'm made of!\n2) WAHHH(Start crying)\n3) I am definetly not trying to break out and grab my sword to kill you!(30 % chance of success WIP))", choices1);
	dialogueTree.at(0) = n1;
	dialogueNode* n2 = new dialogueNode("A umie eh? I ain't ever seen one uv dem so scrawny.", "0) ...... (Say Nothing)\n1) The only reason you think I am skinny is because you're so fat!\n2) WAHHH (Start crying)\n3) I am definetly not trying to break out and grab my sword to kill you! (30% chance of success WIP)", choices2);
	dialogueTree.at(1) = n2;
	dialogueNode* n3 = new dialogueNode("Doesn't talk much does e? Do you talk umie?", "0) ...... (Say Nothing)\n1) Only to those worth talking to you blob of slime\n2) WAHHH (Start crying)\n3) I am definetly not trying to break out and grab my sword to kill you! (30% chance of success WIP)", choices3);
	dialogueTree.at(2) = n3;
	dialogueNode* n4 = new dialogueNode("Guess not. Oh well, cook im for dinner I spose. Put some extra salt on im too. I don't fink'ed taste like much otherwise", "0) ...... (Say Nothing)\n1) WAHH (Start crying)\n2) I am definetly not trying to break out and grab my sword to kill you! (30% chance of success WIP)", choices4);
	dialogueTree.at(3) = n4;
	dialogueNode* n5 = new dialogueNode("(Says to the other goblins) nah, e really didn't taste datgood, not even wiv da salt", "Press enter to continue", choices5);
	dialogueTree.at(4) = n5;
	dialogueNode* n6 = new dialogueNode("Ur gonna regret dat umie. You want a foight, you got un. Here is your sword, just so you may have a chance", "Press enter to continue", choices6);
	dialogueTree.at(5) = n6;
	dialogueNode* n7 = new dialogueNode("Awww, iz it gonna cry? Dat's da most umie fing Io eva did see. Frow it in da cage and we can watch it cry fur fun.", "0) WAHHHHHHHHHHH (Keep crying)\n1) I am definetly not trying to break out and grab my sword to kill you! (30% chance of success WIP)", choices7);
	dialogueTree.at(6) = n7;
	dialogueNode* n8 = new dialogueNode("(Says to the other goblins) Look at this loser cry, its so funny. Oh he dyed of dehydration, oh well", "Press enter to continue", choices8);
	dialogueTree.at(7) = n8;
	dialogueNode* n9 = new dialogueNode("WUT!! 'ow did u escape!? Iom gonna krump u reel good 'umie", "0) I'm going to kill you!\n1) Bye-Bye! (Run away)", choices9);
	dialogueTree.at(8) = n9;
	dialogueNode* n10 = new dialogueNode("Alroight den, oi guess dis iz it. u may 'ave 'urt me a bit, but ull neva win cuz oi 'as moi 'uge choppa!", "Press enter to continue", choices10);
	dialogueTree.at(9) = n10;
}

void goblinKing::printDialogueG(int currentDialogue, player& p1, boss& b1, ItemRegistry& registry, Inventory Inv)
{
	refreshScreen();
	dialogueNode* current = dialogueTree.at(currentDialogue);
	int choice;
	string dialogue = R"(
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                                                                                                                                         
                                                                                                                                                          
   )" + name + R"(:                                                                                                                                        
   )" + current->getNPCDialogue() + R"(                                                                                                                    
   )" + +R"(                                                                                                                                             
                                                                                                                                                           
                                                                                                               
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
)";
	string choices = R"(
----------------------------------------------------------------------------------------------------------|
                                                                              
)" + current->getPlayerChoices() + R"(  
                                                                              
                                                                              
                                                                              
                                                                              
----------------------------------------------------------------------------------------------------------|
)";
	cout << dialogue;
	cout << choices;
	if (current->getNextDialogues().size() == 0)
	{
		if (currentDialogue == 5 || currentDialogue == 9)
		{
			std::cin.ignore();
			std::cin.get();
			enterBattle(b1, p1, registry, Inv);
		}
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

		printDialogueG(current->getNextDialogues().at(choice) - 1, p1, b1, registry, Inv);
	}
}