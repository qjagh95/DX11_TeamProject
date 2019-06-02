#include "../ClientHeader.h"
#include "NPCBase.h"

int NPCBase::m_Count = 0;

NPCBase::NPCBase()
{
	m_MyCount = 0;
	m_Count++;
	m_Renderer = NULLPTR;
	m_TargetPlayer = NULLPTR;
	m_Animation = NULLPTR;
	m_BT = NULLPTR;
	m_NaviMesh = NULLPTR;
	m_HeadCollider = NULLPTR;
	m_BodyCollider = NULLPTR;
	m_LegCollider = NULLPTR;
	m_BodyOBB = NULLPTR;
	m_Target = NULLPTR;

	m_State = 0;
	m_PrevState = 0;
	m_Hp = 1;
	m_TargetDistance = 0.0f;
	m_MoveSpeed = 5.0f;
	m_Y = 0.0f;
	m_PathFind = false;
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
	SAFE_RELEASE(m_BodyOBB);
	SAFE_RELEASE(m_Renderer);
	SAFE_RELEASE(m_Animation);
	SAFE_DELETE(m_BT);
}

bool NPCBase::Init()
{
	m_pObject->SetFrustrumCullUse(true);

	m_Animation = m_pObject->AddComponent<CAnimation>("MonsterAnimation");
	m_Renderer = m_pObject->AddComponent<CRenderer>("MonsterRenderer");
	m_Renderer->SetAnimation(m_Animation);

	m_HeadCollider = m_pObject->AddComponent<CColliderSphere>("MonsterHead");
	m_HeadCollider->SetInfo(Vector3(0.0f, 1.0f, 0.0f), 1.0f);
	m_HeadCollider->SetMyTypeName("MonsterHead");
	m_HeadCollider->SetContinueTypeName("MonsterBodyOBB");
	m_HeadCollider->SetContinueTypeName("MouseCollider");

	m_BodyCollider = m_pObject->AddComponent<CColliderSphere>("MonsterBody");
	m_BodyCollider->SetMyTypeName("MonsterBody");
	m_BodyCollider->SetContinueTypeName("MonsterBodyOBB");
	m_BodyCollider->SetContinueTypeName("MouseCollider");

	m_LegCollider = m_pObject->AddComponent<CColliderSphere>("MonsterLeg");
	m_LegCollider->SetInfo(Vector3(0.0f, -1.0f, 0.0f), 5.0f);
	m_LegCollider->SetMyTypeName("MonsterLeg");
	m_LegCollider->SetContinueTypeName("MonsterBodyOBB");
	m_LegCollider->SetContinueTypeName("MouseCollider");

	m_BodyOBB = m_pObject->AddComponent<CColliderOBB3D>("MonsterBodyOBB");
	m_BodyOBB->SetMyTypeName("MonsterBodyOBB");
	m_BodyOBB->SetContinueTypeName("MouseCollider");

	m_pTransform->SetWorldScale(Vector3(0.04f, 0.04f, 0.04f));
	m_MyCount = m_Count;

	return true;
}

int NPCBase::Input(float fTime)
{
	m_NaviMesh = CNavigationManager3D::GetInst()->FindNavMesh(m_pScene, m_pTransform->GetWorldPos());

	if (m_NaviMesh != NULLPTR)
		m_Y = m_NaviMesh->GetY(m_pTransform->GetWorldPos());

	return 0;
}

int NPCBase::Update(float DeltaTime)
{
	if (m_Target != NULLPTR)
		m_TargetDistance = m_pObject->GetTransformNonCount()->GetWorldPos().Distance(m_TargetTransform->GetWorldPos());

	if (m_BT != NULLPTR)
		m_BT->Update(DeltaTime);

	if (m_Hp <= 0)
		m_pObject->Die();

	return 0;
}

int NPCBase::LateUpdate(float DeltaTime)
{
	return 0;
}

void NPCBase::Collision(float DeltaTime)
{
}

void NPCBase::Render(float DeltaTime)
{
}

NPCBase * NPCBase::Clone()
{
	return new NPCBase(*this);
}

void NPCBase::CreateBT(const string & TreeName, BT_ROOT_CHILD_TYPE Type)
{
	char temp[255] = {};
	_itoa_s(m_Count, temp, 10);

	string Name = TreeName;
	Name += temp;

	m_BT = BTManager::Get()->CreateBehaviorTree(Name, m_pObject, Type);
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

void NPCBase::ChangeState(int State, string* AniName)
{
	m_PrevState = m_State;
	m_State = State;
	m_CurName = AniName[m_State];
	m_Animation->ChangeClip(m_CurName);
}

void NPCBase::ChangeState(int* myStateVar, int ChangeState, string * AniName, CAnimation * Animation)
{
	*myStateVar = ChangeState;
	Animation->ChangeClip(AniName[ChangeState]);
}

void NPCBase::SetTarget(CGameObject * Target)
{
	m_Target = Target;
	m_TargetTransform = m_Target->GetTransformNonCount();
	m_TargetPlayer = m_Target->FindComponentFromTagNonCount<CHuman_Player>("Player");
}

void NPCBase::SetLegCollider(float Radius, const Vector3 & Center)
{
	m_LegCollider->SetInfo(Center, Radius);
}

void NPCBase::SetBodyCollider(float Radius, const Vector3 & Center)
{
	m_BodyCollider->SetInfo(Center, Radius);
}

void NPCBase::SetHeadCollider(float Radius, const Vector3 & Center)
{
	m_HeadCollider->SetInfo(Center, Radius);
}

void NPCBase::SetOBBCollider(const Vector3 & Length, const Vector3 & Center)
{
	m_BodyOBB->SetInfo(Center, Vector3::Axis, Length);
}