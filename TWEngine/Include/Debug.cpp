#include "stdafx.h"
#include "Debug.h"
#include "Core.h"

PUN_USING

void CDebug::OutputConsole(char * pText)
{
#ifdef _DEBUG
	_cprintf(pText);
#endif // DEBUG
}

void CDebug::OutputVisual(char * pText)
{
	OutputDebugStringA(pText);
}

void CDebug::OutputTitle(char * pTitle)
{
	//#ifdef _DEBUG
	SetWindowTextA(WINDOWHANDLE, pTitle);
	//#endif
}