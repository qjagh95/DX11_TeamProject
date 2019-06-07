#include "../ClientHeader.h"
#include "KeyInven.h"
#include "KeyBigIcon.h"


CKeyInven::CKeyInven() :
	m_iIndex(0),
	m_iMoveIndex(0)
{
	m_eComType = (COMPONENT_TYPE)UT_KEYINVEN;
	SetTag("KeyInven");

	m_iFlag = -1;
	m_bMouseOn = false;
	m_bOnDocx = false;
	m_bVisible = false;
	m_fItemY = 0.f;
}

CKeyInven::CKeyInven(const CKeyInven & key)	:
	CUserComponent(key)
{
}

CKeyInven::~CKeyInven()
{
	SAFE_RELEASE(m_pKeyBig);
	SAFE_RELEASE(m_pKeyBigObj);

	for (size_t i = 0; i < m_vecKey.size(); ++i)
	{
		SAFE_RELEASE(m_vecKey[i]);
	}

	Safe_Release_VecList(m_vecKey);
}

void CKeyInven::SetKeyMax(int iMax)
{
	m_iKeyMax = iMax;
}

void CKeyInven::SetVisible(bool bVisible)
{
	m_pObject->SetEnable(bVisible);
}

void CKeyInven::SetVisible()
{
	if (m_iFlag == -1)
	{
		CGameObject*	pInvenObj = CGameObject::FindObject("Inven");

		if (pInvenObj->GetEnable() == true)
		{
			pInvenObj->SetEnable(false);
		}
		SAFE_RELEASE(pInvenObj);

		CSoundManager::GetInst()->SoundPlay("Docx_Open");
		PUN::CInput::GetInst()->ShowMouse(true);
		m_bVisible = true;
		m_pObject->SetEnable(true);

		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (m_vecKey[i] == nullptr)
				break;

			m_vecKey[i]->SetEnable(true);
		}
	}
	else if (m_iFlag == 1)
	{
		m_bVisible = false;
		m_pObject->SetEnable(false);
		m_pKeyBig->ChangeClip("KeyBig_Empty");
		m_pKeyBigObj->SetEnable(false);
		PUN::CInput::GetInst()->ShowMouse(false);

		for (size_t i = 0; i < m_vecKey.size(); ++i)
		{
			if (m_vecKey[i] == nullptr)
				break;

			m_vecKey[i]->SetEnable(false);
		}
	}

	m_iFlag *= -1;
}

void CKeyInven::SetMouseOn(bool bMouseOn)
{
	m_bMouseOn = bMouseOn;
}

void CKeyInven::SetMouseOnKey(bool bMouseOnInven)
{
	m_bMouseOnKey = bMouseOnInven;
}

bool CKeyInven::GetVisible() const
{
	return m_bVisible;
}

int CKeyInven::GetKeyMax() const
{
	return m_iKeyMax;
}

bool CKeyInven::Full() const
{
	return m_iKeyMax == m_vecKey.size();
}

void CKeyInven::AddItem(CGameObject * pItem)
{
	if (Full())
		return;

	m_vecKey.push_back(pItem);

	CTransform*	pItemTr = m_vecKey[m_iIndex]->GetTransform();

	Vector3	vKeyPos = m_pTransform->GetWorldPos();

	pItemTr->SetWorldPos(vKeyPos.x + 395.f, vKeyPos.y + 562.f - m_fItemY, 0.f);
	pItemTr->SetWorldPivot(0.5f, 0.5f, 0.f);

	m_fItemY += 105.f;

	if (m_pObject->GetEnable() == false)
	{
		m_vecKey[m_iIndex]->SetEnable(false);
	}

	++m_iIndex;

	SAFE_RELEASE(pItemTr);
}

const Vector3 CKeyInven::GetKeyPos() const
{
	return m_pTransform->GetWorldPos();
}

bool CKeyInven::GetMouseOnKey() const
{
	return m_bMouseOnKey;
}

void CKeyInven::AfterClone()
{
}

bool CKeyInven::Init()
{
	CSoundManager::GetInst()->CreateSoundEffect("Docx_Open", TEXT("Document_Open.WAV"));
	CResourcesManager::GetInst()->CreateTexture("KeyInven", TEXT("UI/Document/DocxInven.png"));

	CRenderer*	pRenderer = m_pObject->AddComponent<CRenderer>("KeyInvenRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetDecalEnable(false);
	pRenderer->Enable2DRenderer();

	m_pObject->SetRenderGroup(RG_UI);

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "KeyInven");
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pTransform->SetWorldScale(1280.f, 720.f, 1.f);
	m_pTransform->SetWorldPivot(0.f, 0.f, 0.f);

	m_pObject->SetEnable(false);

	m_iKeyMax = 20;
	m_iIndex = 0;

	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	m_pKeyBigObj = CGameObject::CreateObject("KeyBigIcon", pUILayer, true);
	m_pKeyBig = m_pKeyBigObj->AddComponent<CKeyBigICon>("KeyBigIcon");

	m_pKeyBigObj->SetEnable(false);
	m_pKeyBig->ChangeClip("KeyBig_Empty");

	SAFE_RELEASE(pUILayer);

	return true;
}

int CKeyInven::Input(float fTime)
{
	return 0;
}

int CKeyInven::Update(float fTime)
{
	return 0;
}

int CKeyInven::LateUpdate(float fTime)
{
	return 0;
}

void CKeyInven::Collision(float fTime)
{
}

void CKeyInven::Render(float fTime)
{
}

CKeyInven * CKeyInven::Clone()
{
	return new CKeyInven(*this);
}

void CKeyInven::Save(BinaryWrite * _pInstBW)
{
	int iSize = m_vecKey.size();
	_pInstBW->WriteData(iSize);

	for (size_t i = 0; i < iSize; ++i)
	{
		_pInstBW->WriteData(m_vecKey[i]->GetTag());
		_pInstBW->WriteData(m_vecKey[i]->GetLayer()->GetTag());
		m_vecKey[i]->Save(_pInstBW);
	}

	_pInstBW->WriteData(m_iIndex);
	_pInstBW->WriteData(m_iKeyMax);

	_pInstBW->WriteData(m_pKeyBigObj->GetTag());
	_pInstBW->WriteData(m_pKeyBigObj->GetLayer()->GetTag());
	_pInstBW->WriteData(m_pKeyBigObj);

	_pInstBW->WriteData(m_pKeyBig->GetTag());
}

void CKeyInven::Load(BinaryRead * _pInstBR)
{
	int iSize = _pInstBR->ReadInt();

	for (size_t i = 0; i < iSize; ++i)
	{
		string ObjectTag = _pInstBR->ReadString();
		string LayerTag = _pInstBR->ReadString();
		CLayer* getLayer = CSceneManager::GetInst()->FindLayer(LayerTag);

		CGameObject* pItem = CGameObject::CreateObject(ObjectTag, getLayer, true);
		pItem->Load(_pInstBR);

		SAFE_RELEASE(getLayer);
		SAFE_RELEASE(pItem);
	}

	_pInstBR->ReadInt();
	_pInstBR->ReadInt();

	string KeyObjTag = _pInstBR->ReadString();
	string strLayerTag = _pInstBR->ReadString();
	CLayer*	pLayer = GET_SINGLE(CSceneManager)->FindLayer(strLayerTag);

	CGameObject*	pKeyObj = CGameObject::CreateObject(KeyObjTag, pLayer, true);
	pKeyObj->Load(_pInstBR);

	string KeyTag = _pInstBR->ReadString();

	CKeyBigICon*	pKey = pKeyObj->AddComponent<CKeyBigICon>(KeyTag);

	SAFE_RELEASE(pKey);
	SAFE_RELEASE(pKeyObj);
	SAFE_RELEASE(pLayer);
}

void CKeyInven::AddUILayer()
{
	CLayer*	pUILayer = m_pScene->FindLayer("UI");

	for (size_t i = 0; i < m_vecKey.size(); ++i)
	{
		if (m_vecKey[i] == nullptr)
			break;

		pUILayer->AddObject(m_vecKey[i]);
	}

	SAFE_RELEASE(pUILayer);
}
