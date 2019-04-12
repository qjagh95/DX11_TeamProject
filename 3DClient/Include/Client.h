#pragma once
#include "EngineHeader.h"
#include "Flag.h"

enum USERCOMPONENT_TYPE
{
	UT_NONE = CT_END,
	UT_PLAYER,
	UT_MONSTER,
	UT_BULLET,
	UT_EFFECT,
	UT_ITEM,
	UT_ICON,
	UT_INVEN
};

// Inventory State
enum INVENTORY_STATE
{
	INS_NORMAL,
	INS_CLICK,
	INS_MOUSEON,
	INS_DISABLE,
	INS_END
};