#include "EngineHeader.h"
#include "Input.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Core.h"
#include "Device.h"
#include "Component/ColliderPoint.h"
#include "CollisionManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

PUN_USING

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_pCreateKey(nullptr),
	m_pMouseTr(nullptr),
	m_pMouse(nullptr),
	m_bShowCursor(false)
{
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pWorldPoint);
	SAFE_RELEASE(m_pMouseTr);
	SAFE_RELEASE(m_pMouse);
	Safe_Delete_Map(m_mapKey);
}

CGameObject * CInput::GetMouseObj() const
{
	m_pMouse->AddRef();
	return m_pMouse;
}

CGameObject * CInput::GetMouseObjNonCount() const
{
	return m_pMouse;
}

Vector2 CInput::GetMouseGap() const
{
	return m_vMouseGap;
}

Vector2 CInput::GetMouseClient() const
{
	return m_vMouseClient;
}

Vector2 CInput::GetMouseWorld() const
{
	return m_vMouseWorld;
}

Vector3 CInput::GetMousePos() const
{
	return m_pMouseTr->GetWorldPos();
}

void CInput::ChangeMouseScene(CScene * pScene)
{
	m_pMouse->SetScene(pScene);
}

void CInput::SetWorldMousePos(Vector3 _vPos)
{
	m_vMouseWorld = Vector2(_vPos.x , _vPos.y);
}

bool CInput::Init()
{
	AddKey("MoveUp", 'W');
	AddKey("MoveDown", 'S');
	AddKey("MoveLeft", 'A');
	AddKey("MoveRight", 'D');
	AddKey(VK_F1, "Pause");
	AddKey(VK_F7, "HP_Potion");
	AddKey(VK_F8, "MP_Potion");
	AddKey(VK_LBUTTON, "LButton");
	AddKey(VK_RBUTTON, "RButton");
	AddKey(VK_MBUTTON, "MButton");
	AddKey("SoulScream", '1');
	AddKey("SpaceCutting", '2');
	AddKey("HellGate", '3');
	AddKey("SpinHammer", '4');
	AddKey("FullHammer", '5');
	AddKey("IronMaden", '6');
	AddKey("MeteorSoul", '7');

	m_pMouse = CGameObject::CreateObject("MouseObj");

	CRenderer*	pRenderer = m_pMouse->AddComponent<CRenderer>("MouseRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetShader(STANDARD_TEX_STATIC_SHADER);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pMouse->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Mouse", TEXT("0.png"));

	SAFE_RELEASE(pMaterial);

	m_pMouseTr = m_pMouse->GetTransform();

	m_pMouseTr->SetWorldScale(32.f, 31.f, 1.f);
	m_pMouseTr->SetWorldPivot(0.f, 1.f, 0.f);		

	// Window 충돌체와 World 충돌체 2개를 추가한다.
	CColliderPoint*	pPoint = m_pMouse->AddComponent<CColliderPoint>("MouseWindow");

	pPoint->SetCollisionGroup("UI");

	SAFE_RELEASE(pPoint);

	m_pWorldPoint = m_pMouse->AddComponent<CColliderPoint>("MouseWorld");

	ShowCursor(FALSE);

	return true;
}

void CInput::Update(float fTime)
{
	unordered_map<string, PKeyInfo>::iterator	iter;
	unordered_map<string, PKeyInfo>::iterator	iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		size_t	iCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState((int)iter->second->vecKey[i]) & 0x8000)
				++iCount;
		}

		if (iCount == iter->second->vecKey.size())
		{
			if (!iter->second->bPress && !iter->second->bPush)
			{
				iter->second->bPress = true;
				iter->second->bPush = true;
			}

			else if (iter->second->bPress)
			{
				iter->second->bPress = false;
			}
		}

		else if (iter->second->bPush)
		{
			iter->second->bUp = true;
			iter->second->bPress = false;
			iter->second->bPush = false;
		}

		else if (iter->second->bUp)
			iter->second->bUp = false;
	}

	POINT	tMousePos;

	GetCursorPos(&tMousePos);
	ScreenToClient(WINDOWHANDLE, &tMousePos);

	Vector2	vMousePos((float)tMousePos.x, (float)tMousePos.y);

	RECT	rc = {};

	GetClientRect(WINDOWHANDLE, &rc);

	vMousePos.y = rc.bottom - vMousePos.y;

	// 비율을 이용해서 디바이스 해상도 내에서의 좌표를 구한다.
	vMousePos *= GET_SINGLE(CDevice)->GetWindowToDeviceResolution();

	m_vMouseGap = vMousePos - m_vMouseClient;

	m_vMouseClient = vMousePos;

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();

	CTransform*	pCameraTr = pScene->GetMainCameraTransform();
	m_vMouseWorld = m_vMouseClient + Vector2(pCameraTr->GetWorldPos().x, pCameraTr->GetWorldPos().y);

	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pScene);

	m_pMouseTr->SetWorldPos(Vector3(m_vMouseClient.x, m_vMouseClient.y, 0.f));
	m_pMouse->Update(fTime);

	if (!m_bShowCursor && (m_vMouseClient.x < 0 || m_vMouseClient.x > _RESOLUTION.iWidth ||
		m_vMouseClient.y < 0 || m_vMouseClient.y > _RESOLUTION.iHeight))
	{
		m_bShowCursor = true;

		while (ShowCursor(TRUE) != 0)
		{
		}
	}

	else if (m_bShowCursor && 0.f <= m_vMouseClient.x && m_vMouseClient.x <= _RESOLUTION.iWidth &&
		0.f <= m_vMouseClient.y && m_vMouseClient.y <= _RESOLUTION.iHeight)
	{
		m_bShowCursor = false;
		while (ShowCursor(FALSE) >= 0)
		{
		}
	}
}

bool CInput::KeyPress(const string & strKey)
{
	PKeyInfo	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bPress;
}

bool CInput::KeyPush(const string & strKey)
{
	PKeyInfo	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bPush;
}

bool CInput::KeyUp(const string & strKey)
{
	PKeyInfo	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bUp;
}

void CInput::RenderMouse(float fTime)
{
	m_pMouse->Render(fTime);
}

void CInput::AddMouseCollision()
{
	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
	CTransform*	pCameraTr = pScene->GetMainCameraTransform();

	m_pWorldPoint->SetInfo(pCameraTr->GetWorldPos());
	Vector3	vWorldPos = m_pWorldPoint->GetInfo();

	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pScene);

	m_pMouse->LateUpdate(0.f);
}

PKeyInfo CInput::FindKey(const string & strKey)
{
	unordered_map<string, PKeyInfo>::iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return nullptr;

	return iter->second;
}