#include "../ClientHeader.h"
#include "TraceMob.h"

#include <Component/Animation.h>
#include <Component/ColliderSphere.h>

#include "Idle.h"
#include "Trace.h"

TraceMob::TraceMob()
{
	m_State = TS_IDLE;
	m_BT = NULLPTR;
}

TraceMob::TraceMob(const TraceMob & CopyData)
	:CUserComponent(CopyData)
{
}

TraceMob::~TraceMob()
{
	SAFE_RELEASE(m_Animation);
	SAFE_DELETE(m_BT);
}

bool TraceMob::Init()
{
	CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>("Render");
	pRenderer->SetMesh("BlackCow", TEXT("Monster4.msh"));

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);
	pMaterial->SetEmissive(0.f);
	SAFE_RELEASE(pMaterial);

	m_Animation = m_pObject->AddComponent<CAnimation>("Animation");

	m_Animation->LoadBone("Monster4.bne");
	m_Animation->Load("Monster4.anm");

	m_pTransform->SetLocalRot(0.f, -90.f, 0.f);
	m_pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	m_pTransform->SetWorldPos(38.f, 0.f, 81.f);

	CColliderSphere* pBody = m_pObject->AddComponent<CColliderSphere>("PlayerBody");
	pBody->SetInfo(Vector3::Zero, 3.f);

	SAFE_RELEASE(pBody);

	m_StateName[TS_IDLE] = "MobIdle";
	m_StateName[TS_TRACE] = "MobTrace";

	m_CurState = m_StateName[m_State];

	return true;
}

void TraceMob::BTInit()
{
	m_BT = BTManager::Get()->CreateBehaviorTree("TraceBT", m_pObject);

	m_BT->AddBoardData("m_TargetTr", &m_TargetTr);

	m_BT->AddRootSelectorInSelector("TraceAndIdle");
	Idle* myIdle = m_BT->AddSelectorInAction<Idle>("TraceAndIdle", "Idle");
	Trace* myTrace = m_BT->AddSelectorInAction<Trace>("TraceAndIdle", "Trace");

	m_BT->AddActionInDecorator<Idle>("Idle", myIdle, &Idle::DistanceCheck);
}

int TraceMob::Input(float fTime)
{
	return 0;
}

int TraceMob::Update(float fTime)
{
	m_BT->Update(fTime);

	return 0;
}

int TraceMob::LateUpdate(float fTime)
{
	return 0;
}

void TraceMob::Collision(float fTime)
{
}

void TraceMob::Render(float fTime)
{
}

TraceMob * TraceMob::Clone()
{
	return new TraceMob(*this);
}

void TraceMob::ChangeState(int State)
{
	m_State = (TRACE_STATE)State;
	m_CurState = m_StateName[m_State];
}

void TraceMob::SetTarget(CGameObject * Target)
{
	m_TargetTr = Target->GetTransformNonCount();
}

