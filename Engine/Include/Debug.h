#pragma once

#include "Engine.h"
#include "Macro.h"

PUN_BEGIN

class PUN_DLL CDebug
{
public:
	static void OutputConsole(char* pText);
	static void OutputVisual(char* pText);
	static void OutputTitle(char* pTitle);
};

PUN_END