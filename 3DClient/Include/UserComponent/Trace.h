#pragma once
#include <BehaviorTree.h>

PUN_USING
class Trace : public BehaviorTree::Action
{
public:
	void Init() override;
	int Update(float DeltaTime) override;
	void Ending(float DeltaTime) override;

private:
	CTransform*	m_TargetTr;
	bool m_PathFind;
	list<Vector3> m_PathList;
	Vector3 m_MovePos;
	TRACE_STATE m_State;
	Vector3 m_vTargetPos;

	bool m_bMove;
	bool m_bPathFind;

public:
	Trace();
	~Trace();
};

