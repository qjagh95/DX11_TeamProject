#include "../ClientHeader.h"
#include "Idle.h"
#include <NavigationMesh.h>
#include <NavigationManager3D.h>
#include <BlackBoard.h>

Idle::Idle()
{
}

Idle::~Idle()
{
}

void Idle::Init()
{
	m_TargetTr = m_BlackBoard->GetData<CTransform>("m_TargetTr");
	m_State = *m_BlackBoard->GetData<TRACE_STATE>("m_State");
}

int Idle::Update(float DeltaTime)
{
	return ACTION_RUNNING;
}

void Idle::Ending(float DeltaTime)
{

}

bool Idle::DistanceCheck(float DeltaTime)
{
	float Distance = m_pObject->GetTransformNonCount()->GetWorldPos().Distance(m_TargetTr->GetWorldPos());
	CScene* getScene = CSceneManager::GetInst()->GetSceneNonCount();

	if (Distance <= 30.0f)
		return false;
	else if (Distance <= 0.5f)
		return true;

	return true;
}
