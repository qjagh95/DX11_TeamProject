#pragma once
#include "UserComponent.h"

PUN_BEGIN

class CColliderSphere;
class CRenderer;
class BehaviorTree;
class CNavigationMesh;
class CColliderOBB3D;
class PUN_DLL NPCBase : public CUserComponent
{
public:
	virtual bool Init() override;
	virtual int Input(float fTime) override;
	virtual int Update(float fTime) override;
	virtual int LateUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void Render(float fTime) override;
	virtual NPCBase* Clone() override;
	
	void CreateBT(const string& TreeName, BT_ROOT_CHILD_TYPE Type = BT_SELECTOR);
	void ChangeState(int State, string* AniName);
	static void ChangeState(int* myStateVar, int ChangeState, string* AniName, CAnimation* Animation);

	int GetHP() const { return m_Hp; }
	void SetHP(int HP) { m_Hp = HP; }
	void AddHp(int Value) { m_Hp += Value; };

	float GetTargetDistance() const { return m_TargetDistance; }

	void SetMoveSpeed(float Value) { m_MoveSpeed = Value; }
	float GetMoveSpeed() const { return m_MoveSpeed; }

	void SetTarget(CGameObject* Target) { m_Target = Target, m_TargetTransform = m_Target->GetTransformNonCount(); }
	CGameObject* GetTarget() const { return m_Target; }
	CTransform* GetTargetTransform() const { return m_TargetTransform; }

	void SetColliderInfo(float HeadRadius, float BodyRadius, float LegRadius, Vector3 OBBLenth);

protected:
	string m_CurName;
	CColliderSphere* m_HeadCollider;
	CColliderSphere* m_BodyCollider;
	CColliderSphere* m_LegCollider;
	CColliderOBB3D* m_BodyOBB;
	CRenderer* m_Renderer;
	CAnimation* m_Animation;
	BehaviorTree* m_BT;
	CGameObject* m_Target;
	CTransform* m_TargetTransform;
	CNavigationMesh* m_NaviMesh;
	list<Vector3> m_PathList;
	float m_Y;

	int m_State;
	int m_PrevState;
	int m_Hp;
	float m_TargetDistance;
	float m_MoveSpeed;

protected:
	NPCBase();
	NPCBase(const NPCBase& CopyData);
	virtual ~NPCBase();
};

PUN_END