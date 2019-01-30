#include "EngineHeader.h"
#include "StageManager.h"
#include "Component/Stage2D.h"

PUN_USING

DEFINITION_SINGLE(CStageManager)

CStageManager::CStageManager()
{
}

CStageManager::~CStageManager()
{
}

void CStageManager::SetStage2D(CStage2D * pStage)
{
	if (pStage != nullptr)
	{
		pStage->AddRef();
		m_pStage = pStage;
	}
}

CStage2D * CStageManager::GetStage2D() const
{
	if (m_pStage != nullptr)
	{
		//m_pPlayer->AddRef();
		return m_pStage;
	}
	return nullptr;
}
