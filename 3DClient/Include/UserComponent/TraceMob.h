#pragma once
#include <Component/UserComponent.h>
#include <BehaviorTree.h>
#include <BTManager.h>

PUN_USING

class TraceMob : public CUserComponent
{
public:
	bool Init() override;
	int Input(float fTime) override;
	int Update(float fTime) override;
	int LateUpdate(float fTime) override;
	void Collision(float fTime) override;
	void Render(float fTime) override;
	TraceMob* Clone() override;

	void ChangeState(int State);
	void SetTarget(CGameObject* Target);
	void BTInit();

private:
	TRACE_STATE m_State;
	string m_StateName[TS_MAX];
	string m_CurState;

	CAnimation*	m_Animation;
	CTransform*	m_TargetTr;
	Vector3		m_TargetPos;
	bool		m_PathFind;
	list<Vector3>	m_PathList;
	bool		m_Move;
	Vector3		m_MovePos;

	BehaviorTree* m_BT;

protected:
	TraceMob();
	TraceMob(const TraceMob& CopyData);
	~TraceMob();

public:
	friend class CGameObject;
};

