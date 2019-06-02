#pragma once
#include <Component/UserComponent.h>

#include <BehaviorTree.h>
#include <NavigationMesh.h>

#include <Component/ColliderOBB3D.h>
#include <Component/ColliderSphere.h>
#include <Component/Renderer.h>

PUN_USING

class CHuman_Player;
class NPCBase : public CUserComponent
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
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

	void SetTarget(CGameObject* Target);
	CGameObject* GetTarget() const { return m_Target; }
	CTransform* GetTargetTransform() const { return m_TargetTransform; }
	void SetMoveingPos(const Vector3& Pos) { m_MovingPos = Pos; }
	Vector3 GetMovingPos() const { return m_MovingPos; }

	void SetLegCollider(float Radius, const Vector3& Center = Vector3::Zero);
	void SetBodyCollider(float Radius,const Vector3& Center = Vector3::Zero);
	void SetHeadCollider(float Radius,const Vector3& Center = Vector3::Zero);
	void SetOBBCollider(const Vector3& Length, const Vector3& Center = Vector3::Zero);

	int GetState() const { return m_State; }
	void SetState(int State) { m_State = State; }

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
	CHuman_Player* m_TargetPlayer;
	CNavigationMesh* m_NaviMesh;
	list<Vector3> m_PathList;
	Vector3 m_MovingPos;

	static int m_Count;
	float m_Y;
	int m_MyCount;
	int m_State;
	int m_PrevState;
	int m_Hp;
	float m_TargetDistance;
	float m_MoveSpeed;
	bool m_PathFind;

protected:
	NPCBase();
	NPCBase(const NPCBase& CopyData);
	virtual ~NPCBase();
};