#pragma once
#include <iostream>
#include "TalkingNPC.h"

class bartender : public talkingNPC
{
public:
	bartender(int treeSize);
	void talk();
};
