#include "EngineHeader.h"
#include "Core.h"
#include "Device.h"
#include "Resource/Mesh.h"
#include "Rendering/Shader.h"
#include "Timer.h"
#include "Input.h"

PUN_USING

DEFINITION_SINGLE(CCore)

bool CCore::m_bLoop = true;

CCore::CCore()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1087);

	memset(m_fClearColor, 0, sizeof(float) * 4);
}

CCore::~CCore()
{
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CObjectManager);
	DESTROY_SINGLE(CSoundManager);
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
	SoundManagerT::Delete();
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

void CCore::SetMasterVolume(float fVolume)
{
	GET_SINGLE(CSoundManager)->SetMasterVolume(fVolume);
}

void CCore::SetVolume(float fVolume, bool bBGM)
{
	GET_SINGLE(CSoundManager)->SetVolume(fVolume, bBGM);
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

	// Device 초기화
	if (!GET_SINGLE(CDevice)->Init(hWnd, iWidth, iHeight, bWindowMode))
		return false;

	SetClearColor(0xff, 0x69, 0xb4, 0x00);

	// 경로관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// 리소스 관리자 초기화
	if (!GET_SINGLE(CResourcesManager)->Init())
		return false;

	// 렌더링 관리자 초기화
	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	// 타이머 관리자 초기화
	if (!GET_SINGLE(CTimerManager)->Init())
		return false;

	// 입력관리자 초기화
	if (!GET_SINGLE(CInput)->Init())
		return false;

	// 충돌 관리자 초기화
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	// 폰트 관리자 초기화
	if (!GET_SINGLE(CFontManager)->Init())
		return false;

	// 사운드 관리자 초기화
	if (!GET_SINGLE(CSoundManager)->Init())
		return false;

	// 오브젝트 관리자 초기화
	if (!GET_SINGLE(CObjectManager)->Init())
		return false;

	// 내비게이션 관리자 초기화
	if (!GET_SINGLE(CNavigationManager)->Init())
		return false;

	// 장면관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	if (SoundManagerT::Get()->Init() == false)
		return false;

	GUIManager::Get()->CreateImGui(m_hWnd, CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());

	m_fTimeScale = 1.0f;

	return true;
}

int CCore::Run()
{
	MSG msg;

	// 기본 메시지 루프입니다.
	while (m_bLoop)
	{
		// PeekMessage : 메세지가 없을때는 FALSE를 반환하면서 바로 빠져나온다.
		// 메세지가 있을 경우 TRUE를 반환하게 된다.
		// 이 메세지를 이용하면 윈도우의 데드타임을 이용해서 게임을 제작할 수 있다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			// 실제 게임 구현부분은 여기에 들어와야 한다.
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainTimer");
	pTimer->Update();

	float fTime = pTimer->GetTime();	

#ifdef _DEBUG
	GUIManager::Get()->ImGuiBegin("MaJaSinInNa");
#endif

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
	// 타겟뷰와 깊이뷰 초기화
	GET_SINGLE(CDevice)->Clear(m_fClearColor);

	GET_SINGLE(CSceneManager)->Render(fTime);
	GET_SINGLE(CRenderManager)->Render(fTime);

#ifdef _DEBUG
	GUIManager::Get()->ImGuiEnd();
#endif

	GET_SINGLE(CInput)->RenderMouse(fTime);

	GET_SINGLE(CDevice)->Present();
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
#ifdef _DEBUG

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

#endif

	switch (message)
	{
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}