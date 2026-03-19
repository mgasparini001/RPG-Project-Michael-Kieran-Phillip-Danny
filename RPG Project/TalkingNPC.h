#pragma once
#include <string>

class talkingNPC
{
private:
	std::string name;
	//vector<diaglougeNode> dialouges
public:
	talkingNPC(int size);
	void printDialouge(int currentDialouge);
	void setName(std::string n);
};