#pragma once

#include "../EngineHeader.h"

PUN_BEGIN

class PUN_DLL CViewManager
{
private:
	unordered_map<string, CRenderState*> m_mapRenderState;
	unordered_map<string, CRenderTarget*> m_mapRenderTarget;
	unordered_map<string, CMultiRenderTarget*> m_mapMultiTarget;

public:
	bool Init();

	DECLARE_SINGLE(CViewManager)
};

PUN_END