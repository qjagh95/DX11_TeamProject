#include "ST3_SlientTrace.h"

void ST3_SlientTrace::FS_IDLE(float DeltaTime)
{
}

void ST3_SlientTrace::FS_TRACE(float DeltaTime)
{
}

void ST3_SlientTrace::FS_BED_FIND(float DeltaTime)
{
	if (m_Animation->IsCurAnimEnd() == true)
	{
		Vector3 GetRot = m_Renderer->GetDiffrentRot();
		m_pTransform->RotationY(GetRot.x);
		//ChangeState(STS_IDLE, m_AniName);
	}
}

void ST3_SlientTrace::FS_DESK_FIND(float DeltaTime)
{

}

void ST3_SlientTrace::FS_LOCKER_FIND(float DeltaTime)
{
}

void ST3_SlientTrace::FS_HOOK(float DeltaTime)
{
}

void ST3_SlientTrace::FS_JAP(float DeltaTime)
{
}

void ST3_SlientTrace::FS_HEAD_ATTACK(float DeltaTime)
{
}

void ST3_SlientTrace::FS_BASH_DOOR(float DeltaTime)
{
}

void ST3_SlientTrace::FS_BASH_DOOR_OPEN(float DeltaTime)
{
	if (m_Animation->IsCurAnimEnd() == true)
	{
		float GetPosZ = m_Renderer->GetDiffrentPosZ();

		m_pTransform->Move(AXIS_Z, GetPosZ);
		//ChangeState(STS_IDLE, m_AniName);
	}
}

void ST3_SlientTrace::FS_NORMAL_DOOR_OPEN_LEFT(float DeltaTime)
{
}

void ST3_SlientTrace::FS_NORMAL_DOOR_OPEN_RIGHT(float DeltaTime)
{
}