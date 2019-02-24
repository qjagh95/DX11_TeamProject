#include "EngineHeader.h"
#include "Core.h"
#include "Device.h"
#include "Resource/Mesh.h"
#include "Rendering/Shader.h"
#include "Timer.h"

PUN_USING

DEFINITION_SINGLE(CCore)

bool CCore::m_bLoop = true;

CCore::CCore()
{
	m_pTimer = NULLPTR;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2258);

	memset(m_fClearColor, 0, sizeof(float) * 4);
}

CCore::~CCore()
{
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CObjectManager);
	DESTROY_SINGLE(CFontManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CTimerManager);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CResourcesManager);
	DESTROY_SINGLE(CNavigationManager);
	DESTROY_SINGLE(CDevice);
	GUIManager::Delete();
	CSoundManager::Delete();
}

HWND CCore::GetWindowHandle() const
{
	return m_hWnd;
}

HINSTANCE CCore::GetWindowInstance() const
{
	return m_hInst;
}

void CCore::SetClearColor(unsigned char r,
	unsigned char g, unsigned char b,
	unsigned char a)
{
	m_fClearColor[0] = r / 255.f;
	m_fClearColor[1] = g / 255.f;
	m_fClearColor[2] = b / 255.f;
	m_fClearColor[3] = a / 255.f;
}

void CCore::SetGameMode(GAME_MODE eMode)
{
	m_eGameMode = eMode;
	GET_SINGLE(CRenderManager)->SetGameMode(eMode);
}


bool CCore::Init(HINSTANCE hInst, unsigned int iWidth,
	unsigned int iHeight, const TCHAR * pTitle,
	const TCHAR * pClass, int iIconID, int iSmallIconID,
	bool bWindowMode)
{
	m_hInst = hInst;
	m_tRS = Resolution(iWidth, iHeight);

	Register(pClass, iIconID, iSmallIconID);

	CreateWnd(pTitle, pClass);

	return Init(m_hInst, m_hWnd, iWidth, iHeight, bWindowMode);
}

bool CCore::Init(HINSTANCE hInst, HWND hWnd,
	unsigned int iWidth, unsigned int iHeight, bool bWindowMode)
{
	m_hInst = hInst;
	m_hWnd = hWnd;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	// Device �ʱ�ȭ
	if (!GET_SINGLE(CDevice)->Init(hWnd, iWidth, iHeight, bWindowMode))
		return false;

	SetClearColor(0x00, 0xff, 0x00, 0x00);

	// ��ΰ����� �ʱ�ȭ
	if (!GET_SINGLE(CPathManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// ���ҽ� ������ �ʱ�ȭ
	if (!GET_SINGLE(CResourcesManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// ������ ������ �ʱ�ȭ
	if (!GET_SINGLE(CRenderManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// Ÿ�̸� ������ �ʱ�ȭ
	if (!GET_SINGLE(CTimerManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// �Է°����� �ʱ�ȭ
	if (!GET_SINGLE(CInput)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// �浹 ������ �ʱ�ȭ
	if (!GET_SINGLE(CCollisionManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// ��Ʈ ������ �ʱ�ȭ
	if (!GET_SINGLE(CFontManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// ������Ʈ ������ �ʱ�ȭ
	if (!GET_SINGLE(CObjectManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// ������̼� ������ �ʱ�ȭ
	if (!GET_SINGLE(CNavigationManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// �������� �ʱ�ȭ
	if (!GET_SINGLE(CSceneManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	if (CSoundManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	GUIManager::Get()->CreateImGui(m_hWnd, CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());
	m_fTimeScale = 1.0f;
	m_pTimer = CTimerManager::GetInst()->FindTimer("MainTimer");

	return true;
}

int CCore::Run()
{
	MSG msg;

	// �⺻ �޽��� �����Դϴ�.
	while (m_bLoop)
	{
		// PeekMessage : �޼����� �������� FALSE�� ��ȯ�ϸ鼭 �ٷ� �������´�.
		// �޼����� ���� ��� TRUE�� ��ȯ�ϰ� �ȴ�.
		// �� �޼����� �̿��ϸ� �������� ����Ÿ���� �̿��ؼ� ������ ������ �� �ִ�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// ���� ���� �����κ��� ���⿡ ���;� �Ѵ�.
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	m_pTimer->Update();
	float fTime = m_pTimer->GetTime();

	GUIManager::Get()->ImGuiBegin("MaJaSinInNa");

	Input(fTime);
	Update(fTime);
	LateUpdate(fTime);
	Collision(fTime);
	Render(fTime);
}

int CCore::Input(float fTime)
{
	GET_SINGLE(CInput)->Update(fTime);

	int iRet = GET_SINGLE(CSceneManager)->Input(fTime);

	return iRet;
}

int CCore::Update(float fTime)
{
	int iRet = GET_SINGLE(CSceneManager)->Update(fTime);

	return iRet;
}

int CCore::LateUpdate(float fTime)
{
	int iRet = GET_SINGLE(CSceneManager)->LateUpdate(fTime);

	return iRet;
}

int CCore::Collision(float fTime)
{
	return GET_SINGLE(CSceneManager)->Collision(fTime);
}

void CCore::Render(float fTime)
{
	// Ÿ�ٺ�� ���̺� �ʱ�ȭ
	GET_SINGLE(CDevice)->Clear(m_fClearColor);
	GET_SINGLE(CSceneManager)->Render(fTime);
	GET_SINGLE(CRenderManager)->Render(fTime);

	GUIManager::Get()->ImGuiEnd();
	GET_SINGLE(CInput)->RenderMouse(fTime);
	GET_SINGLE(CDevice)->Present();
	CInput::GetInst()->ClearWheel();
}

void CCore::Register(const TCHAR * pClass, int iIconID, int iSmallIconID)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY180629);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iSmallIconID));

	RegisterClassEx(&wcex);
}

void CCore::CreateWnd(const TCHAR * pTitle, const TCHAR * pClass)
{
	m_hWnd = CreateWindow(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, m_tRS.iWidth, m_tRS.iHeight,
		nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
		return;

	RECT		rc = { 0, 0, (LONG)m_tRS.iWidth, (LONG)m_tRS.iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_MOUSEWHEEL:
		CInput::GetInst()->SetWheelDir(HIWORD(wParam));
		break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}