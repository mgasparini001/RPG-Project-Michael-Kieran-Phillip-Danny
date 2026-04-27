#include "Bartender.h"

bartender::bartender(int treeSize) : talkingNPC(treeSize)
{
	name = "The Bartender";
	vector<int> choices1 = { 2,6,6 };
	vector<int> choices2 = { 3,5,6,6 };
	vector<int> choices3 = { 4,6,4 };
	vector<int> choices4;
	vector<int> choices5 = { 3,6,6 };
	vector<int> choices6 = { 8,9,8,7 };
	vector<int> choices7;
	vector<int> choices8;
	vector<int> choices9;
	dialogueNode* n1 = new dialogueNode("Hello adventurer! Care for some ale to cure a dry throat?", "0) Yes Please! (Pay 1 gold)\n1) No thanks\n2) How dare you ask payment!? (Punch him in the face and take it, pay 0 gold", choices1);
	dialogueTree.at(0) = n1;
	
	dialogueNode* n2 = new dialogueNode("Where are you off to?", "0) Nowhere in particular\n1) I'm on a quest!\n2) None of your business\n3) NONE OF YOUR BUSINESS (Punch him in the face)", choices2);
	dialogueTree.at(1) = n2;
	
	dialogueNode* n3 = new dialogueNode("Then let me give you some advice: those mountains up ahead are treacherous. They have goblins that travel in their dark places and at night. If you take the mountain pass you'll probably meet some at nightfall, but if you go into the caves you'll definetly meet some.", "0) Tell me more about the goblins\n1) Thanks (leave)\n2) Which way do you recommend?", choices3);
	dialogueTree.at(2) = n3;
	
	dialogueNode* n4 = new dialogueNode("It is said that the Goblin King has a great treasure horde that would make a man rich, but he keeps it hidden away in\nthose caves so that no one can find it. There's also supposed to be an anicent armory near the entrance of the caves. If I was you, I would go see if I could get myself some weapons and fight the goblins good and proper. Who knows, you may\neven find some treasure!", "Press enter to continue", choices4);
	dialogueTree.at(3) = n4;
	
	dialogueNode* n5 = new dialogueNode("That's great! Where to?", "0) Just over those mountains\n1) I'd tell you but then I'd have to kill you\n2) STOP TALKING (Punch him in the face)", choices5);
	dialogueTree.at(4) = n5;
	
	dialogueNode* n6 = new dialogueNode("Oh no! Not this band of ruffians again! Their leader does not seem to like you\nThe leader: I don't like the looks of you traveler! (Punches you)\nThat punch from him seemed to hurt, are you just going to take that?", "0) I could do this all day (Start fight WIP)\n1) You're on your own! (Run away)\n2) You are all beneath me! (Start Fight WIP)\n3) Whatever he can punch me again (turn the other cheek)", choices6);
	dialogueTree.at(5) = n6;
	
	dialogueNode* n7 = new dialogueNode("The leader ruffed you up pretty good, but this gold coin should help you get back on you feet.", "Press enter to continue", choices7);
	dialogueTree.at(6) = n7;
	
	dialogueNode* n8 = new dialogueNode("Please defeat this guy! (Battle starts here, WIP)", "Press enter to continue", choices8);
	dialogueTree.at(7) = n8;
	dialogueNode* n9 = new dialogueNode("Coward", "Press enter to continue", choices9);
	dialogueTree.at(8) = n9;
}
