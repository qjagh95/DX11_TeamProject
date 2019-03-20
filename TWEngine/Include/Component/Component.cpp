#include "EngineHeader.h"
#include "Component.h"

PUN_USING

CComponent::CComponent()
{
}

CComponent::CComponent(const CComponent & com)
{
	*this = com;

	m_iRefCount = 1;
}

CComponent::~CComponent()
{
}

CScene * CComponent::GetScene() const
{
	return m_pScene;
}

CLayer * CComponent::GetLayer() const
{
	return m_pLayer;
}

CGameObject * CComponent::GetGameObject() const
{
	m_pObject->AddRef();
	return m_pObject;
}

CGameObject * CComponent::GetGameObjectNonCount() const
{
	return m_pObject;
}

CTransform * CComponent::GetTransform() const
{
	m_pTransform->AddRef();
	return m_pTransform;
}

CTransform * CComponent::GetTransformNonCount() const
{
	return m_pTransform;
}

COMPONENT_TYPE CComponent::GetComponentType() const
{
	return m_eComType;
}

void CComponent::Start()
{
}

void CComponent::AfterClone()
{
}

int CComponent::Input(float fTime)
{
	return 0;
}

int CComponent::Update(float fTime)
{
	return 0;
}

int CComponent::LateUpdate(float fTime)
{
	return 0;
}

void CComponent::Collision(float fTime)
{
}

void CComponent::Render(float fTime)
{
}

void CComponent::Save(FILE * pFile)
{
	// Tag를 저장한다.
	size_t iLength = m_strTag.length();

	fwrite(&iLength, sizeof(size_t), 1, pFile);

	fwrite(m_strTag.c_str(), 1, iLength, pFile);
}

void CComponent::Load(FILE * pFile)
{
	size_t	iLength = 0;
	fread(&iLength, sizeof(size_t), 1, pFile);

	char	strTag[256] = {};
	fread(strTag, 1, iLength, pFile);

	m_strTag = strTag;
}

void CComponent::Save(BinaryWrite* _pInstBW)
{
}

void CComponent::Load(BinaryRead* _pInstBR)
{
}

void CComponent::SaveFromPath(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	FILE*	pFile = nullptr;

	fopen_s(&pFile, strPath.c_str(), "wb");

	if (!pFile)
		return;

	Save(pFile);

	fclose(pFile);
}

void CComponent::LoadFromPath(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathFromMultibyte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	FILE*	pFile = nullptr;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
		return;

	Load(pFile);

	fclose(pFile);
}

const list<CComponent*>* CComponent::FindComponentsFromTag(const string & strTag)
{
	return m_pObject->FindComponentsFromTag(strTag);
}

const list<CComponent*>* CComponent::FindComponentsFromType(COMPONENT_TYPE eType)
{
	return m_pObject->FindComponentsFromType(eType);
}

bool CComponent::CheckComponentFromType(COMPONENT_TYPE eType)
{
	return m_pObject->CheckComponentFromType(eType);
}
