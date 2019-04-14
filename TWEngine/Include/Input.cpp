#include "EngineHeader.h"
#include "Input.h"
#include "Core.h"
#include "Device.h"
#include "CollisionManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Animation2D.h"
#include "Component/ColliderRay.h"
#include "Component/ColliderPoint.h"
#include "EditManager.h"
#include "Rendering/RenderManager.h"
#include "Rendering/Shader.h"
#include "Component/Camera.h"
#include "Rendering/RenderState.h"

PUN_USING

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_pMouseTr(nullptr),
	m_pMouse(nullptr),
	m_bShowCursor(false),
	m_sWheel(0),
	m_iSelectNavIndex(0),
	m_fBlushSize(0.f)
{
	m_NewKey = NULLPTR;
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pWorldPoint);
	SAFE_RELEASE(m_pMouseTr);
	SAFE_RELEASE(m_pMouse);

	Safe_Delete_Map(m_KeyMap);
}

void CInput::SelectNaviBoxRender(float fTime)
{
	if (m_iSelectNavIndex == -1)
		return;

	CRenderManager::GetInst()->FindRenderState(DEPTH_DISABLE)->SetState();

	CShader* pShader = CShaderManager::GetInst()->FindShader(COLLIDER_SHADER);
	pShader->SetShader();
	ID3D11InputLayout* pLayOut = CShaderManager::GetInst()->FindInputLayout(POS_LAYOUT);
	CONTEXT->IASetInputLayout(pLayOut);

	Matrix	matPos, matScale, matRot;

	matPos.Translation(m_vSelectNaviCellCenter);
	matScale.Scaling(Vector3(m_fBlushSize / 2.f, 0.1f, m_fBlushSize / 2.f));

	TransformCBuffer	tCBuffer = {};
	CMesh* pMesh = CResourcesManager::GetInst()->FindMesh("ColliderBox");

	CCamera*	pMainCamera = CSceneManager::GetInst()->GetMainCamera();

	Matrix	matView = pMainCamera->GetViewMatrix();

	tCBuffer.matWorld = matScale * matRot * matPos;
	tCBuffer.matView = matView;
	tCBuffer.matProj = pMainCamera->GetProjMatrix();
	tCBuffer.matWV = tCBuffer.matWorld * tCBuffer.matView;
	tCBuffer.matWVP = tCBuffer.matWV * tCBuffer.matProj;
	tCBuffer.vPivot = Vector3(0.f, 0.f, 0.f);
	tCBuffer.vLength = pMesh->GetLength();

	tCBuffer.matWorld.Transpose();
	tCBuffer.matView.Transpose();
	tCBuffer.matProj.Transpose();
	tCBuffer.matWV.Transpose();
	tCBuffer.matWVP.Transpose();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tCBuffer);

	SAFE_RELEASE(pMainCamera);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Collider", &Vector4::BlueViolet);


	pMesh->Render();
	SAFE_RELEASE(pMesh);
	CRenderManager::GetInst()->FindRenderState(DEPTH_DISABLE)->ResetState();

	SAFE_RELEASE(pShader);
}

RayInfo CInput::MouseRayInfo() const
{
	return ((CColliderRay*)m_pWorldPoint)->GetInfo();
}

CGameObject * CInput::GetMouseObj() const
{
	//m_pMouse->AddRef();
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

Vector2 CInput::GetMouse3DGap() const
{
	return m_vMouse3DGap;
}

Vector2 CInput::GetMouse3DClient() const
{
	return m_vMouse3DClient;
}

Vector2 CInput::GetMouse3DWorld() const
{
	return m_vMouse3DWorld;
}

void CInput::ChangeMouseScene(CScene * pScene)
{
	m_pMouse->SetScene(pScene);
}

void CInput::SetWorldMousePos(Vector3 _vPos)
{
	m_vMouseWorld = Vector2(_vPos.x, _vPos.y);
}

void CInput::SetWheelDir(short _sWheel)
{
	m_sWheel = _sWheel / 120;
}

void CInput::ClearWheel()
{
	m_sWheel = 0;
}

short CInput::GetWheelDir() const
{
	return m_sWheel;
}

bool CInput::Init()
{
	m_pMouse = CGameObject::CreateObject("MouseObj");

	CRenderer*	pRenderer = m_pMouse->AddComponent<CRenderer>("MouseRenderer");

	pRenderer->SetMesh("TexRect");
	pRenderer->SetShader(STANDARD_TEX_STATIC_SHADER);
	pRenderer->SetRenderState(ALPHA_BLEND);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->Enable2DRenderer();

	SAFE_RELEASE(pRenderer);

	CMaterial*	pMaterial = m_pMouse->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pMaterial->SetDiffuseTex(0, "Mouse", TEXT("0.png"));
	pMaterial->SetSampler(0, SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	m_pMouseTr = m_pMouse->GetTransform();

	m_pMouseTr->SetWorldScale(32.f, 31.f, 1.f);
	m_pMouseTr->SetWorldPivot(0.f, 1.f, 0.f);

	// Window 충돌체와 World 충돌체 2개를 추가한다.
	CColliderPoint*	pPoint = m_pMouse->AddComponent<CColliderPoint>("MouseWindow");

	pPoint->SetCollisionGroup("UI");

	SAFE_RELEASE(pPoint);

	if (CRenderManager::GetInst()->GetGameMode() == GM_2D)
		m_pWorldPoint = m_pMouse->AddComponent<CColliderPoint>("MouseWorld");
	else
	{
		m_pWorldPoint = m_pMouse->AddComponent<CColliderRay>("MouseWorld");
		((CColliderRay*)m_pWorldPoint)->MouseEnable();

		if (CCore::GetInst()->m_bEditorMode == true)
		{
			m_pWorldPoint->SetCollisionCallback(CCT_STAY, this, &CInput::Hit);
		}

	}

	ShowCursor(FALSE);

	AddKey("LButton", VK_LBUTTON);
	AddKey("RButton", VK_RBUTTON);
	AddKey("MButton", VK_MBUTTON);
	AddKey("TabButton", VK_TAB);
	return true;
}

void CInput::Update(float fTime)
{
	POINT tMousePos = {};

	GetCursorPos(&tMousePos);
	ScreenToClient(WINDOWHANDLE, &tMousePos);

	Vector2	vMousePos((float)tMousePos.x, (float)tMousePos.y);
	Vector2	vMouse3DPos = vMousePos;

	RECT	rc = {};

	GetClientRect(WINDOWHANDLE, &rc);

	vMousePos.y = rc.bottom - vMousePos.y;

	// 비율을 이용해서 디바이스 해상도 내에서의 좌표를 구한다.
	vMousePos *= GET_SINGLE(CDevice)->GetWindowToDeviceResolution();
	vMouse3DPos *= GET_SINGLE(CDevice)->GetWindowToDeviceResolution();

	m_vMouseGap = vMousePos - m_vMouseClient;
	m_vMouse3DGap = vMouse3DPos - m_vMouse3DClient;

	m_vMouseClient = vMousePos;
	m_vMouse3DClient = vMouse3DPos;

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

	HWND	hWnd = GetActiveWindow();

	// 포커스가 없을때는 마우스가 윈도우 안에 있어도 무조건 포커스를 false로 한다.
	if (hWnd == 0)
	{
		m_bFocus = false;
	}

	else
	{
		// 순수한 윈도우 상에서의 마우스 위치가 윈도우 창을 벗어났는지 판단한다.
		POINT	ptFocus;
		GetCursorPos(&ptFocus);
		ScreenToClient(WINDOWHANDLE, &ptFocus);

		RECT	tWindowRC;
		GetClientRect(WINDOWHANDLE, &tWindowRC);

		if (ptFocus.x < 0 || ptFocus.y < 0 || ptFocus.x > tWindowRC.right ||
			ptFocus.y > tWindowRC.bottom)
			m_bFocus = false;

		else
			m_bFocus = true;
	}

	if (!m_bFocus)
	{
		unordered_map<string, KeyInfo*>::iterator	iter;
		unordered_map<string, KeyInfo*>::iterator	iterEnd = m_KeyMap.end();

		for (iter = m_KeyMap.begin(); iter != iterEnd; ++iter)
		{
			iter->second->KeyPress = false;
			iter->second->KeyRelease = false;
			iter->second->KeyPush = false;
		}
		return;
	}

	unordered_map<string, KeyInfo*>::iterator	iter;
	unordered_map<string, KeyInfo*>::iterator	iterEnd = m_KeyMap.end();

	for (iter = m_KeyMap.begin(); iter != iterEnd; ++iter)
	{
		size_t	iCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState((int)iter->second->vecKey[i]) & 0x8000)
				++iCount;
		}

		if (iCount == iter->second->vecKey.size())
		{
			if (!iter->second->KeyPress && !iter->second->KeyPush)
			{
				iter->second->KeyPress = true;
				iter->second->KeyPush = true;
			}

			else if (iter->second->KeyPress)
			{
				iter->second->KeyPress = false;
			}
		}

		else if (iter->second->KeyPush)
		{
			iter->second->KeyRelease = true;
			iter->second->KeyPress = false;
			iter->second->KeyPush = false;
		}

		else if (iter->second->KeyRelease)
			iter->second->KeyRelease = false;
	}
}

bool CInput::KeyPress(const string & Name)
{
	KeyInfo* getKey = FindKey(Name);

	if (getKey == NULLPTR)
		return false;

	return getKey->KeyPress;
}

bool CInput::KeyPush(const string & Name)
{
	KeyInfo* getKey = FindKey(Name);

	if (getKey == NULLPTR)
		return false;

	return getKey->KeyPush;
}

bool CInput::KeyRelease(const string & Name)
{
	KeyInfo* getKey = FindKey(Name);

	if (getKey == NULLPTR)
		return false;

	return getKey->KeyRelease;
}

KeyInfo * CInput::FindKey(const string & Name)
{
	unordered_map<string, KeyInfo*>::iterator FindIter = m_KeyMap.find(Name);

	if (FindIter == m_KeyMap.end())
		return NULLPTR;

	return FindIter->second;
}

void CInput::RenderMouse(float fTime)
{
	m_pMouse->Render(fTime);
}

void CInput::AddMouseCollision()
{
	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();
	CTransform*	pCameraTr = pScene->GetMainCameraTransform();

	if (CRenderManager::GetInst()->GetGameMode() == GM_2D)
		((CColliderPoint*)m_pWorldPoint)->SetInfo(pCameraTr->GetWorldPos());

	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pScene);

	m_pMouse->LateUpdate(0.f);
}

void CInput::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "PickingCollider")
	{
		if (KeyPress("LButton") == true)
		{
			CGameObject* pDestObj = pDest->GetGameObject();
			CEditManager::GetInst()->SetActiveObject(pDestObj);
			SAFE_RELEASE(pDestObj);
		}
	}
}