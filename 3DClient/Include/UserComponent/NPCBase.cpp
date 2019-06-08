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
	m_BodyOBB = NULLPTR;
	m_Target = NULLPTR;

	m_MeshLenght = 0.0f;
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

	m_BodyOBB = m_pObject->AddComponent<CColliderOBB3D>("MonsterBodyOBB");
	m_BodyOBB->SetMyTypeName("MonsterBodyOBB");
	m_BodyOBB->SetContinueTypeName("MouseCollider");
	m_BodyOBB->SetContinueTypeName("JapBody");
	m_BodyOBB->SetContinueTypeName("HookBody");
	m_BodyOBB->SetContinueTypeName("HeadBody");

	m_pTransform->SetWorldScale(Vector3(0.04f, 0.04f, 0.04f));
	m_MyCount = m_Count;

	return true;
}

int NPCBase::Input(float fTime)
{
	if (m_NaviMesh != NULLPTR)
		m_Y = m_NaviMesh->GetY(m_pTransform->GetWorldPos());

	return 0;
}

int NPCBase::Update(float DeltaTime)
{
	m_MeshLenght = m_Renderer->GetMeshLength() * m_pTransform->GetWorldScale();
	m_WorldPos = m_pTransform->GetWorldPos();

	Vector3 LenghtHelf = m_MeshLenght * 0.5f;

	m_FrontCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontCenterRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontUpCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontUpLeft = Vector3(m_WorldPos.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontUpRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontDownCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y, m_WorldPos.z - LenghtHelf.z);
	m_FrontDownLeft = m_WorldPos;
	m_FrontDownRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y, m_WorldPos.z - LenghtHelf.z);

	m_CenterCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterCenterRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterUpCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterUpLeft = Vector3(m_WorldPos.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterUpRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterDownCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterDownLeft = Vector3(m_WorldPos.x, m_WorldPos.y, m_WorldPos.z + LenghtHelf.z);
	m_CenterDownRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y, m_WorldPos.z + LenghtHelf.z);

	m_BackCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z + LenghtHelf.z);
	m_BackCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z + LenghtHelf.z);
	m_BackCenterRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y + LenghtHelf.y, m_WorldPos.z + LenghtHelf.z);
	m_BackUpCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z + m_MeshLenght.z);
	m_BackUpCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z + m_MeshLenght.z);
	m_BackUpCenterRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y + m_MeshLenght.y, m_WorldPos.z + m_MeshLenght.z);
	m_BackDownCenter = Vector3(m_WorldPos.x + LenghtHelf.x, m_WorldPos.y, m_WorldPos.z + m_MeshLenght.z);
	m_BackDownCenterLeft = Vector3(m_WorldPos.x, m_WorldPos.y, m_WorldPos.z + m_MeshLenght.z);
	m_BackDownCenterRight = Vector3(m_WorldPos.x + m_MeshLenght.x, m_WorldPos.y, m_WorldPos.z + m_MeshLenght.z);

	m_NaviMesh = CNavigationManager3D::GetInst()->FindNavMesh(m_pScene, m_CenterDownCenter);

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

void NPCBase::SetOBBCollider(const Vector3 & Length, const Vector3 & Center)
{
	m_BodyOBB->SetInfo(Center, Vector3::Axis, Length);
}

float NPCBase::GetYAngle(const Vector3 & V1, const Vector3 & V2) const
{
	float Atan = atan2(V1.x - V2.x, V1.z - V2.z);
	float Angle = RadianToDegree(Atan);
	Angle += 180.0f;
	
	return Angle;
}
