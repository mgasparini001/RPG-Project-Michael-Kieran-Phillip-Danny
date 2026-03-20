#pragma once
#include <iostream>
#include "TalkingNPC.h"


class bartender : public talkingNPC
{
public:
	//constructor that hardcodes in the bartender's dialogue
	bartender(int treeSize);
};
