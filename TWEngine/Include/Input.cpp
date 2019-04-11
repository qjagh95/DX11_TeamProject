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

	// Window �浹ü�� World �浹ü 2���� �߰��Ѵ�.
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
	ReadKeyBoard(); // 256���� Keyboard ���¸� ���´�.

	POINT tMousePos = {};

	GetCursorPos(&tMousePos);
	ScreenToClient(WINDOWHANDLE, &tMousePos);

	Vector2	vMousePos((float)tMousePos.x, (float)tMousePos.y);
	Vector2	vMouse3DPos = vMousePos;

	RECT	rc = {};

	GetClientRect(WINDOWHANDLE, &rc);

	vMousePos.y = rc.bottom - vMousePos.y;

	// ������ �̿��ؼ� ����̽� �ػ� �������� ��ǥ�� ���Ѵ�.
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

	// ��Ŀ���� �������� ���콺�� ������ �ȿ� �־ ������ ��Ŀ���� false�� �Ѵ�.
	if (hWnd == 0)
	{
		m_bFocus = false;
	}

	else
	{
		// ������ ������ �󿡼��� ���콺 ��ġ�� ������ â�� ������� �Ǵ��Ѵ�.
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

	//256���� KeyList�� ��ȸ�Ѵ�.
	for (size_t i = 0; i < m_KeyList.size(); ++i)
	{
		if (m_cKey[m_KeyList[i]] & 0x80) // ���� Key�� �����ִ� ���¶��
		{
			if (!m_bPress[m_KeyList[i]] && !m_bPush[m_KeyList[i]]) //Key ���°� Press , Push�� ��� False���ٸ�
			{
				m_bPush[m_KeyList[i]] = true; // ���� Key�� Push true
				m_bPress[m_KeyList[i]] = true; // ���� Key�� Press true
			}
			else // ���߿� �ϳ��� true ���¿��ٸ�
			{
				m_bPress[m_KeyList[i]] = false; // ���� Key�� Press false
				m_bPush[m_KeyList[i]] = true;	// ���� Key�� Push true
			}
		}

		else if (m_bPress[m_KeyList[i]] || m_bPush[m_KeyList[i]])
			// ���࿡ �ȴ����ִµ� Press�� Push�� true�ϰ��
		{
			m_bPress[m_KeyList[i]] = false; // ���� Key Press false
			m_bPush[m_KeyList[i]] = false; // ���� key Push false
			m_bRelease[m_KeyList[i]] = true; // ���� Key Release true
		}

		else if (m_bRelease[m_KeyList[i]]) // Key�� �ȴ����ִµ� Release�� true�� ���
		{
			m_bRelease[m_KeyList[i]] = false; // ���� Key Release false
		}
	}

	unordered_map<string, PBindAxis>::iterator	iter;
	unordered_map<string, PBindAxis>::iterator	iterEnd = m_mapAxis.end();

	// Axis(�̵����� Key�� �ַ� �����) Key Map�� ��ȸ�Ѵ�.
	for (iter = m_mapAxis.begin(); iter != iterEnd; ++iter)
	{
		// Map Key�� ��ϵǾ��ִ� Key List�� ��ȸ�Ѵ�.
		for (size_t i = 0; i < iter->second->KeyList.size(); ++i)
		{
			float	fScale = 0.f; // �������� fScale �����Ѵ�.

			if (m_bPush[(iter->second->KeyList[i])->cKey]) // ���� Key�� Push�� true�ϰ��
				fScale = (iter->second->KeyList[i])->fScale; // ���� Key�� ��ϵ� Scale ���� ���������� �ִ´�.

			// Func�� ��ϵ� Key���� Function�� ��� ȣ��ȴ�.
			// Key�� �ȴ������� 0.f�� ���� �ɼ��� ��� �ȵǰԲ� �ϴµ�
			// �������� �̰Ŵ� �ǵ��ؼ� ������ �ۼ��ߴٰ� ��.
			// ������ Key�� ������ Scale���� ���� ������ �Ͼ��
			// Key�� Release���°ų� �ƹ��� ������ ���������� Scale���� 0�̵���
			// �Լ� ȣ�������� ������ �Ͼ�� �ʴ´�.
			if (iter->second->bFunctionBind)
				iter->second->func(fScale, fTime);
		}
	}

	unordered_map<string, PBindAction>::iterator	iter2;
	unordered_map<string, PBindAction>::iterator	iter2End = m_mapAction.end();

	unsigned char	cSKey[SKEY_END] = { DIK_LSHIFT, DIK_LCONTROL, DIK_LALT };
	//Special Key�� tag�� ���� ���� cSKey�迭���� ����
	bool	bSKeyState[SKEY_END] = {};
	//Special Key�� ���� Key ���¸� ���� bool �迭 ���� ���� 
	for (int i = 0; i < SKEY_END; ++i) // Special Key�� ������ŭ �ݺ� ������.
	{
		if (m_cKey[cSKey[i]] & 0x80) // Special Key�� Ű�� ���ȴ��� Check �Ѵ�.
		{
			bSKeyState[i] = true; // Key�� �����ִٸ� bool �迭�� ���� Key�� true�� �����Ѵ�.
		}
		else
		{
			bSKeyState[i] = false; // �ƴ϶�� false�� �����Ѵ�.
		}
	}

	// ����Ű�� ���� mapAction Key�� ��ȸ�Ѵ�.
	for (iter2 = m_mapAction.begin(); iter2 != iter2End; ++iter2)
	{
		vector<PActionKey>::iterator	iter1;
		vector<PActionKey>::iterator	iter1End = iter2->second->KeyList.end();
		// ActionKey�� ��ϵǾ��ִ� KeyList�� ��ȸ�Ѵ�.
		for (iter1 = iter2->second->KeyList.begin(); iter1 != iter1End; ++iter1)
		{
			bool bSKeyEnable[SKEY_END] = { false  , false , false };

			for (int i = 0; i < SKEY_END; ++i)
			{
				if ((*iter1)->bSKey[i]) // Map key�� ��ϵǾ��ִ� Special Key�� true ���¶��
				{
					if (bSKeyState[i]) // ������ Check�� Special Key�� �������ִ� ���°� true���
						bSKeyEnable[i] = true; // ���������� ������ KeyEnable ���¸� true�� �����Ѵ�

					else
						bSKeyEnable[i] = false; // �ƴ϶�� false�� �����Ѵ�.
				}
				else // �ݴ�� Map Key�� ��ϵ� SpecialKey�� false���
				{
					if (bSKeyState[i]) // ������ ������� Enable�� false���ǰ�
						bSKeyEnable[i] = false;

					else
						bSKeyEnable[i] = true; // �ƴҰ�� true�� �ȴ�.
				}
			}

			bool	bPush = false;
			bool	bPress = false;
			bool	bRelease = false;

			// ���콺 ó��
			if ((*iter1)->cKey >= DIK_LBUTTON)
			{
				unsigned char	cMouseIndex = (*iter1)->cKey - DIK_LBUTTON;

				bPress = m_bMousePress[cMouseIndex];
				bPush = m_bMousePush[cMouseIndex];
				bRelease = m_bMouseRelease[cMouseIndex];
			}

			// Ű���� ó��
			else
			{
				bPress = m_bPress[(*iter1)->cKey];
				bPush = m_bPush[(*iter1)->cKey];
				bRelease = m_bRelease[(*iter1)->cKey];
			}

			if (bPush && bSKeyEnable[SKEY_CONTROL] &&
				bSKeyEnable[SKEY_SHIFT] && bSKeyEnable[SKEY_ALT])
				// ��ϵ� Key�� true�̸� SpecialKey�� Enable���°� ��� true���
			{
				if (!(*iter1)->bPress && !(*iter1)->bPush)//Press�� Push���°� ��� false���
				{
					(*iter1)->bPress = true; // ���� Key Press true
					(*iter1)->bPush = true;// ���� Key Push true
				}

				else // ���߿� �ϳ��� true���
					(*iter1)->bPress = false;// ���� Key�� Press ���� false
			}

			else if ((*iter1)->bPress || (*iter1)->bPush)
				// SpecialŰ�� �Ϲ�Ű�� ������ �� �ȴ��� �ִ� ���¶��
				// Press�� Push ���߿� �ϳ��� true �����̸�
			{
				(*iter1)->bPress = false;  // Press false
				(*iter1)->bPush = false; // Push false
				(*iter1)->bRelease = true; // Release true
			}

			else if ((*iter1)->bRelease)
				// Ȱ��ȭ�� �ȵǾ��ְ� release�� true �����̸�
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
	// ���� Keyboard 256���� Key ���¸� ���´�.

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			// KeyBoard ��ġ �� ������ ���������
			m_pKeyboard->Acquire();// �ٽ� ���� �Ѵ�. 

		else // �ƴҰ��(���������� �Ǵ�)
			return false;//(false�� ��ȯ�Ѵ�)
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
