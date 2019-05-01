#include "../EngineHeader.h"
#include "NPCBase.h"

#include "Renderer.h"
#include "ColliderOBB3D.h"
#include "ColliderSphere.h"
#include "Animation.h"
#include "../NavigationMesh.h"
#include "../NavigationManager3D.h"
#include "../BehaviorTree.h"

PUN_USING

NPCBase::NPCBase()
{
	m_Renderer = NULLPTR;
	m_Animation = NULLPTR;
	m_BT = NULLPTR;
	m_NaviMesh = NULLPTR;
	m_HeadCollider = NULLPTR;
	m_BodyCollider = NULLPTR;
	m_LegCollider = NULLPTR;

	m_State = 0;
	m_PrevState = 0;
	m_Hp = 1;
	m_TargetDistance = 0.0f;
	m_MoveSpeed = 5.0f;
	m_Y = 0.0f;
	m_CurName = "None";
}

NPCBase::NPCBase(const NPCBase & CopyData)
	:CUserComponent(CopyData)
{
}

NPCBase::~NPCBase()
{
	SAFE_RELEASE(m_HeadCollider);
	SAFE_RELEASE(m_BodyCollider);
	SAFE_RELEASE(m_LegCollider);
	SAFE_RELEASE(m_Renderer);
	SAFE_RELEASE(m_Animation);
	SAFE_DELETE(m_BT);
}

bool NPCBase::Init()
{
	m_pObject->SetFrustrumCullUse(false);

	m_Renderer = m_pObject->AddComponent<CRenderer>("MonsterRenderer");
	m_Animation = m_pObject->AddComponent<CAnimation>("MonsterAnimation");

	m_HeadCollider = m_pObject->AddComponent<CColliderSphere>("MonsterHead");
	m_BodyCollider = m_pObject->AddComponent<CColliderSphere>("MonsterBody");
	m_LegCollider = m_pObject->AddComponent<CColliderSphere>("MonsterLeg");

	return true;
}

int NPCBase::Input(float fTime)
{
	m_NaviMesh = GET_SINGLE(CNavigationManager3D)->FindNavMesh(m_pScene, m_pTransform->GetWorldPos());

	if (m_NaviMesh != NULLPTR)
		m_Y = m_NaviMesh->GetY(m_pTransform->GetWorldPos());

	return 0;
}

int NPCBase::Update(float fTime)
{
	if (m_Target != NULLPTR)
		m_TargetDistance = m_pObject->GetTransformNonCount()->GetWorldPos().Distance(m_TargetTransform->GetWorldPos());

	if (m_BT != NULLPTR)
		m_BT->Update(fTime);

	if (m_Hp > 0)
		m_pObject->Die();

	return 0;
}

int NPCBase::LateUpdate(float fTime)
{
	return 0;
}

void NPCBase::Collision(float fTime)
{
}

void NPCBase::Render(float fTime)
{
}

NPCBase * NPCBase::Clone()
{
	return new NPCBase(*this);
}

void NPCBase::CreateBT(const string & TreeName, BT_ROOT_CHILD_TYPE Type)
{
	m_BT = BTManager::Get()->CreateBehaviorTree(TreeName, m_pObject, Type);
	m_BT->AddBoardData("m_Animation", m_Animation);
	m_BT->AddBoardData("m_Target", m_Target);
	m_BT->AddBoardData("m_TargetTransform", m_TargetTransform);
	m_BT->AddBoardData("m_NaviMesh", m_NaviMesh);
	m_BT->AddBoardData("m_PathList", &m_PathList);
	m_BT->AddBoardData("m_PrevState", &m_PrevState);
	m_BT->AddBoardData("m_State", &m_State);
	m_BT->AddBoardData("m_Hp", &m_Hp);
	m_BT->AddBoardData("m_MoveSpeed", &m_MoveSpeed);
	m_BT->AddBoardData("m_TargetDistance", &m_TargetDistance);
	m_BT->AddBoardData("m_Y", &m_Y);
}

void NPCBase::ChangeState(int State)
{
	m_PrevState = m_State;
	m_State = State;
	m_CurName = m_AniName[m_State];
	m_Animation->ChangeClip(m_CurName);
}

void NPCBase::ChangeState(int* myStateVar, int ChangeState, string * AniName, CAnimation * Animation)
{
	*myStateVar = ChangeState;
	Animation->ChangeClip(AniName[ChangeState]);
}
