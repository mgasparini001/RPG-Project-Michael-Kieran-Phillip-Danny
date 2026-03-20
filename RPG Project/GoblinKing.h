#pragma once
#include <iostream>
#include "TalkingNPC.h"

class goblinKing : public talkingNPC
{
public:
	//consttructor that hardcodes in the bartender's dialogue
	goblinKing(int treeSize);
};