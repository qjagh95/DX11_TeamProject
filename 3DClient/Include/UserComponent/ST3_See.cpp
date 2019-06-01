#include "../ClientHeader.h"
#include "ST3_See.h"

ST3_See::ST3_See()
{
	m_SeeDist = 8.0f;
}

ST3_See::ST3_See(const ST3_See & CopyData)
	:NPCBase(CopyData)
{
}

ST3_See::~ST3_See()
{
}

bool ST3_See::Init()
{
	NPCBase::Init();

	m_AniName[SES_BASH] = "";
	m_AniName[SES_SEE] = "";

	m_Renderer->SetMesh("", TEXT(""));
	m_Animation->LoadBone("");
	m_Animation->Load("");

	ChangeState(SES_BASH, m_AniName);

	return true;
}

int ST3_See::Input(float fTime)
{
	NPCBase::Input(fTime);

	return 0;
}

int ST3_See::Update(float fTime)
{
	NPCBase::Update(fTime);

	switch (m_State)
	{
	case SES_SEE:
		FS_SEE(fTime);
		break;
	case SES_BASH:
		FS_BASH(fTime);
		break;
	}

	return 0;
}

int ST3_See::LateUpdate(float fTime)
{
	return 0;
}

void ST3_See::Collision(float fTime)
{
}

void ST3_See::Render(float fTime)
{
}

ST3_See * ST3_See::Clone()
{
	return new ST3_See(*this);
}

void ST3_See::FS_SEE(float DeltaTime)
{
	float Angle = m_pTransform->GetWorldPos().GetAngle(m_TargetTransform->GetWorldPos());
	m_pTransform->SetWorldRotY(Angle);

	if (m_TargetDistance >= m_SeeDist)
		ChangeState(SES_BASH, m_AniName);
}

void ST3_See::FS_BASH(float DeltaTime)
{
	if (m_TargetDistance <= m_SeeDist)
		ChangeState(SES_SEE, m_AniName);
}
