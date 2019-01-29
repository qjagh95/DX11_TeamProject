#pragma once

#include "Engine.h"

PUN_BEGIN

class PUN_DLL CStageManager
{
private:
	class CStage2D*	m_pStage;

public:
	void SetStage2D(class CStage2D* pStage);

public:
	class CStage2D*	GetStage2D()	const;

	DECLARE_SINGLE(CStageManager)
};

PUN_END