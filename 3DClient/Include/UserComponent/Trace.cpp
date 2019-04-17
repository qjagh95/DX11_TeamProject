#include "../ClientHeader.h"
#include "Trace.h"

#include <BlackBoard.h>
#include <NavigationMesh.h>
#include <NavigationManager3D.h>
#include <BlackBoard.h>

Trace::Trace()
{

}

Trace::~Trace()
{
}

void Trace::Init()
{
	m_TargetTr = m_BlackBoard->GetData<CTransform>("m_TargetTr");
	m_bMove = false;
	m_bPathFind = false;
}

int Trace::Update(float DeltaTime)
{
	if (m_bPathFind == false)
	{
		CScene* getScene = CSceneManager::GetInst()->GetSceneNonCount();
		CNavigationMesh* pMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(getScene, m_pObject->GetTransformNonCount()->GetWorldPos());

		pMesh->FindPath(m_pObject->GetTransformNonCount()->GetWorldPos(), m_TargetTr->GetWorldPos());
		m_PathList.clear();
		m_PathList = pMesh->GetPathList();

		m_MovePos = m_PathList.front();

		m_bPathFind = true;
	}

	Vector3 cPos = m_MovePos;
	Vector3	vDir = cPos - m_pObject->GetTransformNonCount()->GetWorldPos();
	vDir.Normalize();

	float Distance = cPos.Distance(m_pObject->GetTransformNonCount()->GetWorldPos());

	if (Distance >= 0.5f)
	{
		m_pObject->GetTransformNonCount()->LookAtY(m_MovePos);
		m_pObject->GetTransformNonCount()->Move(vDir, 5.0f, DeltaTime);
	}

	if (Distance < 0.5f)
	{
		if (m_PathList.empty())
		{
			m_bPathFind = false;
			m_bMove = false;
		}
		else
		{
			m_MovePos = m_PathList.front();
			m_PathList.pop_front();
		}
	}

	if (Distance >= 35.0f)
		return ACTION_FALSE;

	return ACTION_RUNNING;
}

void Trace::Ending(float DeltaTime)
{
}
