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

PUN_USING

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_pInput(nullptr),
	m_pKeyboard(nullptr),
	m_pMouseTr(nullptr),
	m_pMouse(nullptr),
	m_bShowCursor(false),
	m_sWheel(0),
	m_bFocus(false),
	m_iSelectNavIndex(0)
{
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pWorldPoint);
	SAFE_RELEASE(m_pMouseTr);
	SAFE_RELEASE(m_pMouse);
	Safe_Delete_Map(m_mapAxis);
	Safe_Delete_Map(m_mapAction);

	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		SAFE_RELEASE(m_pKeyboard);
	}

	SAFE_RELEASE(m_pInput);
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

bool CInput::GetMousePress(MOUSE_STATE eState)
{
	return m_bMousePress[eState];
}

bool CInput::GetMousePush(MOUSE_STATE eState)
{
	return m_bMousePush[eState];
}

bool CInput::GetMouseRelease(MOUSE_STATE eState)
{
	return m_bMouseRelease[eState];
}

bool CInput::Init()
{
	if (FAILED(DirectInput8Create(WINDOWINSTANCE, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		return false;

	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return false;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	/*if (FAILED(m_pKeyboard->SetCooperativeLevel(WINDOWHANDLE, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		return false;*/

		//if (FAILED(m_pKeyboard->Acquire()))
		//	return false;

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

	memset(m_bPress, 0, sizeof(bool) * 256);
	memset(m_bPush, 0, sizeof(bool) * 256);
	memset(m_bRelease, 0, sizeof(bool) * 256);
	memset(m_cKey, 0, sizeof(unsigned char) * 256);

	memset(m_bMousePress, 0, sizeof(bool) * MS_END);
	memset(m_bMousePush, 0, sizeof(bool) * MS_END);
	memset(m_bMouseRelease, 0, sizeof(bool) * MS_END);

	return true;
}

void CInput::Update(float fTime)
{
	ReadKeyBoard(); // 256개의 Keyboard 상태를 얻어온다.

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

	//#ifdef _DEBUG
	//	static float	ftime11 = 0.f;
	//	ftime11 += fTime;
	//
	//	if (ftime11 >= 2.f)
	//	{
	//		ftime11 = 0.f;
	//		TCHAR	str[256] = {};
	//		wsprintf(str, TEXT("Active Window : %d\n"), hWnd);
	//
	//		OutputDebugString(str);
	//	}
	//#endif

	if (!m_bFocus)
	{
		ClearKeyState();
		return;
	}

	const char	cMouse[3] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };

	for (int i = 0; i < MS_END; ++i)
	{
		if (GetAsyncKeyState(cMouse[i]) & 0x8000)
		{
			if (!m_bMousePress[i] && !m_bMousePush[i])
			{
				m_bMousePress[i] = true;
				m_bMousePush[i] = true;
			}

			else if (m_bMousePush[i])
				m_bMousePress[i] = false;
		}

		else if (m_bMousePush[i])
		{
			m_bMousePress[i] = false;
			m_bMousePush[i] = false;
			m_bMouseRelease[i] = true;
		}

		else if (m_bMouseRelease[i])
			m_bMouseRelease[i] = false;
	}

	//256개의 KeyList를 순회한다.
	for (size_t i = 0; i < m_KeyList.size(); ++i)
	{
		if (m_cKey[m_KeyList[i]] & 0x80) // 현재 Key가 눌려있는 상태라면
		{
			if (!m_bPress[m_KeyList[i]] && !m_bPush[m_KeyList[i]]) //Key 상태가 Press , Push가 모두 False였다면
			{
				m_bPush[m_KeyList[i]] = true; // 현재 Key의 Push true
				m_bPress[m_KeyList[i]] = true; // 현재 Key의 Press true
			}
			else // 둘중에 하나라도 true 상태였다면
			{
				m_bPress[m_KeyList[i]] = false; // 현재 Key의 Press false
				m_bPush[m_KeyList[i]] = true;	// 현재 Key의 Push true
			}
		}

		else if (m_bPress[m_KeyList[i]] || m_bPush[m_KeyList[i]])
			// 만약에 안눌려있는데 Press나 Push가 true일경우
		{
			m_bPress[m_KeyList[i]] = false; // 현재 Key Press false
			m_bPush[m_KeyList[i]] = false; // 현재 key Push false
			m_bRelease[m_KeyList[i]] = true; // 현재 Key Release true
		}

		else if (m_bRelease[m_KeyList[i]]) // Key가 안눌려있는데 Release가 true일 경우
		{
			m_bRelease[m_KeyList[i]] = false; // 현재 Key Release false
		}
	}

	unordered_map<string, PBindAxis>::iterator	iter;
	unordered_map<string, PBindAxis>::iterator	iterEnd = m_mapAxis.end();

	// Axis(이동관련 Key를 주로 사용함) Key Map을 순회한다.
	for (iter = m_mapAxis.begin(); iter != iterEnd; ++iter)
	{
		// Map Key에 등록되어있는 Key List를 순회한다.
		for (size_t i = 0; i < iter->second->KeyList.size(); ++i)
		{
			float	fScale = 0.f; // 지역변수 fScale 선언한다.

			if (m_bPush[(iter->second->KeyList[i])->cKey]) // 현재 Key의 Push가 true일경우
				fScale = (iter->second->KeyList[i])->fScale; // 현재 Key가 등록된 Scale 값을 지역변수에 넣는다.

			// Func이 등록된 Key들은 Function이 계속 호출된다.
			// Key가 안눌렸을때 0.f가 들어가서 옵션이 사용 안되게끔 하는데
			// 선생님이 이거는 의도해서 구조를 작성했다고 함.
			// 실제로 Key가 눌려서 Scale값이 들어가면 행위가 일어나고
			// Key가 Release상태거나 아무런 행위를 안했을때는 Scale값에 0이들어가서
			// 함수 호출했을때 행위가 일어나지 않는다.
			if (iter->second->bFunctionBind)
				iter->second->func(fScale, fTime);
		}
	}

	unordered_map<string, PBindAction>::iterator	iter2;
	unordered_map<string, PBindAction>::iterator	iter2End = m_mapAction.end();

	unsigned char	cSKey[SKEY_END] = { DIK_LSHIFT, DIK_LCONTROL, DIK_LALT };
	//Special Key의 tag를 담을 변수 cSKey배열변수 선언
	bool	bSKeyState[SKEY_END] = {};
	//Special Key의 현재 Key 상태를 담을 bool 배열 변수 선언 
	for (int i = 0; i < SKEY_END; ++i) // Special Key의 갯수만큼 반복 돌린다.
	{
		if (m_cKey[cSKey[i]] & 0x80) // Special Key의 키가 눌렸는지 Check 한다.
		{
			bSKeyState[i] = true; // Key가 눌려있다면 bool 배열의 현재 Key를 true로 정의한다.
		}
		else
		{
			bSKeyState[i] = false; // 아니라면 false로 정의한다.
		}
	}

	// 조합키를 담은 mapAction Key를 순회한다.
	for (iter2 = m_mapAction.begin(); iter2 != iter2End; ++iter2)
	{
		vector<PActionKey>::iterator	iter1;
		vector<PActionKey>::iterator	iter1End = iter2->second->KeyList.end();
		// ActionKey에 등록되어있는 KeyList도 순회한다.
		for (iter1 = iter2->second->KeyList.begin(); iter1 != iter1End; ++iter1)
		{
			bool bSKeyEnable[SKEY_END] = { false  , false , false };

			for (int i = 0; i < SKEY_END; ++i)
			{
				if ((*iter1)->bSKey[i]) // Map key에 등록되어있는 Special Key가 true 상태라면
				{
					if (bSKeyState[i]) // 위에서 Check한 Special Key의 눌려져있는 상태가 true라면
						bSKeyEnable[i] = true; // 지역변수로 선언한 KeyEnable 상태를 true로 정의한다

					else
						bSKeyEnable[i] = false; // 아니라면 false로 정의한다.
				}
				else // 반대로 Map Key에 등록된 SpecialKey가 false라면
				{
					if (bSKeyState[i]) // 눌려져 있을경우 Enable은 false가되고
						bSKeyEnable[i] = false;

					else
						bSKeyEnable[i] = true; // 아닐경우 true가 된다.
				}
			}

			bool	bPush = false;
			bool	bPress = false;
			bool	bRelease = false;

			// 마우스 처리
			if ((*iter1)->cKey >= DIK_LBUTTON)
			{
				unsigned char	cMouseIndex = (*iter1)->cKey - DIK_LBUTTON;

				bPress = m_bMousePress[cMouseIndex];
				bPush = m_bMousePush[cMouseIndex];
				bRelease = m_bMouseRelease[cMouseIndex];
			}

			// 키보드 처리
			else
			{
				bPress = m_bPress[(*iter1)->cKey];
				bPush = m_bPush[(*iter1)->cKey];
				bRelease = m_bRelease[(*iter1)->cKey];
			}

			if (bPush && bSKeyEnable[SKEY_CONTROL] &&
				bSKeyEnable[SKEY_SHIFT] && bSKeyEnable[SKEY_ALT])
				// 등록된 Key가 true이며 SpecialKey의 Enable상태가 모두 true라면
			{
				if (!(*iter1)->bPress && !(*iter1)->bPush)//Press와 Push상태가 모두 false라면
				{
					(*iter1)->bPress = true; // 현재 Key Press true
					(*iter1)->bPush = true;// 현재 Key Push true
				}

				else // 둘중에 하나라도 true라면
					(*iter1)->bPress = false;// 현재 Key의 Press 상태 false
			}

			else if ((*iter1)->bPress || (*iter1)->bPush)
				// Special키와 일반키가 조합이 다 안눌려 있는 상태라면
				// Press와 Push 둘중에 하나라도 true 상태이면
			{
				(*iter1)->bPress = false;  // Press false
				(*iter1)->bPush = false; // Push false
				(*iter1)->bRelease = true; // Release true
			}

			else if ((*iter1)->bRelease)
				// 활성화가 안되어있고 release가 true 상태이면
			{
				(*iter1)->bRelease = false; // release false
			}

			if (iter2->second->iKeyState & KEY_PRESS &&
				(*iter1)->bPress && iter2->second->bFunctionBind[AT_PRESS])
			{
				iter2->second->func[AT_PRESS](fTime);
			}

			if (iter2->second->iKeyState & KEY_PUSH &&
				(*iter1)->bPush && iter2->second->bFunctionBind[AT_PUSH])
			{
				iter2->second->func[AT_PUSH](fTime);
			}

			if (iter2->second->iKeyState & KEY_RELEASE &&
				(*iter1)->bRelease && iter2->second->bFunctionBind[AT_RELEASE])
			{
				iter2->second->func[AT_RELEASE](fTime);
			}
		}
	}


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

bool CInput::ReadKeyBoard()
{
	HRESULT	result = m_pKeyboard->GetDeviceState(256, m_cKey);
	// 현재 Keyboard 256개의 Key 상태를 얻어온다.

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			// KeyBoard 장치 의 연결이 끊겼을경우
			m_pKeyboard->Acquire();// 다시 연결 한다. 

		else // 아닐경우(비정상으로 판단)
			return false;//(false를 반환한다)
	}

	return true;
}

void CInput::ClearKeyState()
{
	m_sWheel = 0;

	vector<unsigned char>::iterator	iterKey;
	vector<unsigned char>::iterator	iterKeyEnd = m_KeyList.end();

	for (iterKey = m_KeyList.begin(); iterKey != iterKeyEnd; ++iterKey)
	{
		m_bPress[*iterKey] = false;
		m_bPush[*iterKey] = false;
		m_bRelease[*iterKey] = false;
	}

	unordered_map<string, PBindAction>::iterator	iter2;
	unordered_map<string, PBindAction>::iterator	iter2End = m_mapAction.end();

	for (iter2 = m_mapAction.begin(); iter2 != iter2End; ++iter2)
	{
		vector<PActionKey>::iterator	iter1;
		vector<PActionKey>::iterator	iter1End = iter2->second->KeyList.end();

		for (iter1 = iter2->second->KeyList.begin(); iter1 != iter1End; ++iter1)
		{
			(*iter1)->bPress = false;
			(*iter1)->bPush = false;
			(*iter1)->bRelease = false;
		}
	}
}

PBindAxis CInput::FindAxis(const std::string & _strName)
{
	unordered_map<string, PBindAxis>::iterator	iter = m_mapAxis.find(_strName);

	if (iter == m_mapAxis.end())
		return nullptr;

	return iter->second;
}

PBindAction CInput::FindAction(const std::string & _strName)
{
	unordered_map<string, PBindAction>::iterator	iter = m_mapAction.find(_strName);

	if (iter == m_mapAction.end())
		return nullptr;

	return iter->second;
}

void CInput::Hit(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "PickingCollider")
	{
		if (GetMousePress(MS_LBUTTON) == true)
		{
			CGameObject* pDestObj = pDest->GetGameObject();
			CEditManager::GetInst()->SetActiveObject(pDestObj);
			SAFE_RELEASE(pDestObj);
		}
	}
}
