#pragma once
#include <iostream>
#include "TalkingNPC.h"
#include "player.h"
#include "boss.h"
#include "battle.h"
#include "ItemRegistry.h"

class goblinKing : public talkingNPC
{
public:
	//consttructor that hardcodes in the bartender's dialogue
	goblinKing(int treeSize);
	void printDialogueG(int currentDialogue, player& p1, boss& b1, ItemRegistry& registry, Inventory Inv);
};