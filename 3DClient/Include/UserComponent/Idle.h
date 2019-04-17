#pragma once
#include <BehaviorTree.h>

PUN_USING

class Idle : public BehaviorTree::Action
{
public:
	void Init() override;
	int Update(float DeltaTime) override;
	void Ending(float DeltaTime) override;

	bool DistanceCheck(float DeltaTime);

public:
	Idle();
	~Idle();

private:
	CTransform*	m_TargetTr;
	TRACE_STATE m_State;
};

