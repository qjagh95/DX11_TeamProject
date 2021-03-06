#include "EngineHeader.h"
#include "Core.h"
#include "Timer.h"
#include "Device.h"
#include "EditManager.h"
#include "NavigationManager3D.h"
#include "Resource/Mesh.h"
#include "Rendering/Shader.h"

#include <DirectXCollision.h>

PUN_USING

DEFINITION_SINGLE(CCore)

unordered_map<string, vector<float>*> CCore::m_ManagerMap;
unordered_map<string, FileStream*> CCore::m_FSMap;
time_t CCore::m_iTime = 0;
tm* CCore::m_pDateInfo = NULLPTR;
bool CCore::m_bLoop = true;

CCore::CCore()
{
	m_bGuiMode = true;
	m_bHeader = false;
	m_bEditorMode = false;
	m_bTreeOnOff = true;
	m_pTimer = NULLPTR;
	m_LogText = NULLPTR;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(7773077);
	
	memset(m_fClearColor, 0, sizeof(float) * 4);

	m_vecInput = NULLPTR;
	m_vecUpdate = NULLPTR;
	m_vecLateUpdate = NULLPTR;
	m_vecCollsion = NULLPTR;
	m_vecRender = NULLPTR;
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
	DESTROY_SINGLE(CNavigationManager3D);
	DESTROY_SINGLE(CDevice);
	GUIManager::Delete();
	CSoundManager::Delete();
	CoUninitialize();

	Safe_Delete_Map(m_ManagerMap);
	Safe_Delete_Map(m_FSMap);
	BTManager::Delete();
}

HWND CCore::GetWindowHandle() const
{
	return m_hWnd;
}

HINSTANCE CCore::GetWindowInstance() const
{
	return m_hInst;
}

void CCore::WriteLogText(ofstream & Stream, float Compute)
{
	Stream << Compute << '\t' << m_pDateInfo->tm_year + 1900 << "년 " << m_pDateInfo->tm_mon + 1 << "월 " << m_pDateInfo->tm_mday << "일 " << m_pDateInfo->tm_hour << "시 " << m_pDateInfo->tm_min << "분 " << m_pDateInfo->tm_sec << "초 " << endl;
}

void CCore::SetLoop(bool _bLoop)
{
	m_bLoop = _bLoop;
}

FileStream * CCore::CreateFileStream(const string & Path, const string& Middle, const string & KeyName)
{
	FileStream* newStream = FindFileStream(KeyName);

	if (newStream != NULLPTR)
		return newStream;

	newStream = new FileStream();

	newStream->Input.open(Path + Middle + " Input.txt");
	newStream->Update.open(Path + Middle + " Update.txt");
	newStream->LateUpdate.open(Path + Middle + " LateUpdate.txt");
	newStream->Collsion.open(Path + Middle + " Collsion.txt");
	newStream->Render.open(Path + Middle + " Render.txt");

	m_FSMap.insert(make_pair(KeyName, newStream));

	return newStream;
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

void CCore::UpdateSoundManager(float fTime)
{
	PUN::CSoundManager::GetInst()->Update(fTime);
}

bool CCore::Init(HINSTANCE hInst, unsigned int iWidth, unsigned int iHeight, const TCHAR * pTitle, const TCHAR * pClass, int iIconID, int iSmallIconID, bool bWindowMode)
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
	m_bGuiMode = true;

	CoInitializeEx(NULLPTR, COINIT_MULTITHREADED);

	// Device 초기화
	if (!GET_SINGLE(CDevice)->Init(hWnd, iWidth, iHeight, bWindowMode))
	{
		TrueAssert(true);
		return false;
	}

	SetClearColor(0x00, 0xff, 0x00, 0x00);

	// 경로관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 리소스 관리자 초기화
	if (!GET_SINGLE(CResourcesManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 렌더링 관리자 초기화
	if (!GET_SINGLE(CRenderManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 타이머 관리자 초기화
	if (!GET_SINGLE(CTimerManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 입력관리자 초기화
	if (!GET_SINGLE(CInput)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 충돌 관리자 초기화
	if (!GET_SINGLE(CCollisionManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 폰트 관리자 초기화
	if (!GET_SINGLE(CFontManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 오브젝트 관리자 초기화
	if (!GET_SINGLE(CObjectManager)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 3D 내비게이션 관리자 초기화
	if (!GET_SINGLE(CNavigationManager3D)->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 장면관리자 초기화
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

	GET_SINGLE(CInput)->AddKey("GuiOnOff", VK_F1);
	GET_SINGLE(CInput)->AddKey("ShaderOptionOnOff", VK_F2);
	GET_SINGLE(CInput)->AddKey("TreeOnOff", VK_F3);
	GET_SINGLE(CInput)->AddKey("WireOnOff", VK_F6);

	m_vecInput = AddManagerVector("LogicInput");
	m_vecUpdate = AddManagerVector("LogicUpdate");
	m_vecLateUpdate = AddManagerVector("LogicLateUpdate");
	m_vecCollsion = AddManagerVector("LogicCollsion");
	m_vecRender = AddManagerVector("LogicRender");

	string Path = CPathManager::GetInst()->FindPathFromMultibyte(DATA_PATH);

	m_LogText = CreateFileStream(Path, "Logic", "Logic");

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
			if (m_bGuiMode == true)
				GUIManager::Get()->ImGuiBegin("Profiles");

			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	m_pTimer->Update();
	float fTime = m_pTimer->GetTime();

	if (fTime > 0.5f)
		fTime = 0.0f;

	Input(fTime);
	Update(fTime);
	UpdateSoundManager(fTime);
	LateUpdate(fTime);
	Collision(fTime);
	Render(fTime);
}

int CCore::Input(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	m_iTime = time(NULL);
	m_pDateInfo = localtime(&m_iTime);

	GET_SINGLE(CInput)->Update(fTime);
	int iRet = GET_SINGLE(CSceneManager)->Input(fTime);

	if (m_bGuiMode == false)
		return iRet;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecInput->push_back(Compute);

	WriteLogText(m_LogText->Input, Compute);

	if (m_vecInput->size() >= 100)
		m_vecInput->erase(m_vecInput->begin());

	return iRet;
}

int CCore::Update(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	int iRet = GET_SINGLE(CSceneManager)->Update(fTime);

	if (m_bGuiMode == false)
		return iRet;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecUpdate->push_back(Compute);

	WriteLogText(m_LogText->Update, Compute);

	if (m_vecUpdate->size() >= 100)
		m_vecUpdate->erase(m_vecUpdate->begin());

	return iRet;
}

int CCore::LateUpdate(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	int iRet = GET_SINGLE(CSceneManager)->LateUpdate(fTime);

	if (m_bGuiMode == false)
		return iRet;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecLateUpdate->push_back(Compute);

	WriteLogText(m_LogText->LateUpdate, Compute);

	if (m_vecLateUpdate->size() >= 100)
		m_vecLateUpdate->erase(m_vecLateUpdate->begin());

	//if (GUIManager::GetInst()->m_bHDR)
	//	GET_SINGLE(CRenderManager)->EnableFilter(CFT_HDR);
	//else
	//	GET_SINGLE(CRenderManager)->DisableFilter(CFT_HDR);	

	//if (GUIManager::GetInst()->m_bBlur)
	//	GET_SINGLE(CRenderManager)->EnableFilter(CFT_BLUR);
	//else
	//	GET_SINGLE(CRenderManager)->DisableFilter(CFT_BLUR);

	//if (GUIManager::GetInst()->m_bMotionBlur)
	//	GET_SINGLE(CRenderManager)->EnableFilter(CFT_MOTIONBLUR);
	//else
	//	GET_SINGLE(CRenderManager)->DisableFilter(CFT_MOTIONBLUR);


	return iRet;
}

int CCore::Collision(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	GET_SINGLE(CSceneManager)->Collision(fTime);

	if (m_bGuiMode == false)
		return 0;

	Info.End = clock();
	float Compute = (float)(Info.End - Info.Start) * 0.01f;
	m_vecCollsion->push_back(Compute);

	WriteLogText(m_LogText->Collsion, Compute);

	if (m_vecCollsion->size() >= 100)
		m_vecCollsion->erase(m_vecCollsion->begin());

	return 0;
}

void CCore::Render(float fTime)
{
	TimeInfo Info = {};
	Info.Start = clock();

	// 타겟뷰와 깊이뷰 초기화
	GET_SINGLE(CDevice)->Clear(m_fClearColor);
	{
		GET_SINGLE(CSceneManager)->Render(fTime);
		BTManager::Get()->GUIRender();
		GET_SINGLE(CRenderManager)->Render(fTime);
		CInput::GetInst()->ClearWheel();

		if (m_bGuiMode == true)
		{
			m_bHeader = ImGui::CollapsingHeader("Logic");

			if (m_bHeader == true)
			{
				Info.End = clock();
				float Compute = (float)(Info.End - Info.Start) * 0.01f;
				m_vecRender->push_back(Compute);

				WriteLogText(m_LogText->Render, Compute);

				if (m_vecRender->size() >= 100)
					m_vecRender->erase(m_vecRender->begin());

				ImGui::PlotLines("Input", &m_vecInput->at(0), (int)m_vecInput->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
				ImGui::PlotLines("Update", &m_vecUpdate->at(0), (int)m_vecUpdate->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
				ImGui::PlotLines("LateUpdate", &m_vecLateUpdate->at(0), (int)m_vecLateUpdate->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
				ImGui::PlotLines("Collsion", &m_vecCollsion->at(0), (int)m_vecCollsion->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
				ImGui::PlotLines("Render", &m_vecRender->at(0), (int)m_vecRender->size(), 0, NULLPTR, 0.0f, 0.05f, ImVec2(0.0f, 50.0f));
			}

			GUIManager::Get()->ImGuiEnd();
		}
		GET_SINGLE(CInput)->RenderMouse(fTime);
	}
	GET_SINGLE(CDevice)->Present();

	GUIOnOff(fTime);
	ShaderOptionOnOff(fTime);
	TreeOnOff(fTime);
	LightWireOnOff();
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
	wcex.lpszMenuName = NULL;
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

void CCore::GUIOnOff(float fTime)
{
	if (CInput::GetInst()->KeyPress("GuiOnOff"))
		m_bGuiMode ^= true;
}

void CCore::ShaderOptionOnOff(float fTime)
{
	if (CInput::GetInst()->KeyPress("ShaderOptionOnOff"))
		GUIManager::Get()->m_bShaderOption ^= true;
}

void CCore::TreeOnOff(float fTime)
{
	if (CInput::GetInst()->KeyPress("TreeOnOff"))
		m_bTreeOnOff ^= true;
}

void CCore::LightWireOnOff()
{
	if (CInput::GetInst()->KeyPress("WireOnOff"))
		CRenderManager::GetInst()->m_bLightWireFrame ^= true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO : Editor 전용
bool CCore::EditInit(HWND hWnd, HINSTANCE hInstance, unsigned int iWidth, unsigned int iHeight, bool bWindowMode)
{
	m_bEditorMode = true;
	m_bGuiMode = false;
	m_hInst = hInstance;
	m_hWnd = hWnd;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	CoInitializeEx(NULLPTR, COINIT_MULTITHREADED);

	// Device 초기화
	if (!CDevice::GetInst()->EditInit(hWnd, iWidth, iHeight, bWindowMode))
	{
		TrueAssert(true);
		return false;
	}

	SetClearColor(0x00, 0xff, 0x00, 0x00);

	// 경로관리자 초기화
	if (!CPathManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 리소스 관리자 초기화
	if (!CResourcesManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 렌더링 관리자 초기화
	if (!CRenderManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 타이머 관리자 초기화
	if (!CTimerManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 입력관리자 초기화
	if (!CInput::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 충돌 관리자 초기화
	if (!CCollisionManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 오브젝트 관리자 초기화
	if (!CObjectManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	if (!CNavigationManager3D::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	// 장면관리자 초기화
	if (!CSceneManager::GetInst()->Init())
	{
		TrueAssert(true);
		return false;
	}

	//if (!GET_SINGLE(CEditManager)->GetInst()->Init())
	//{
	//	TrueAssert(true);
	//	return false;
	//}

	m_fTimeScale = 1.0f;
	m_pTimer = CTimerManager::GetInst()->FindTimer("MainTimer");

	return true;
}
void CCore::EditLogic()
{
	m_pTimer->Update();
	float fTime = m_pTimer->GetTime();

	Input(fTime);
	Update(fTime);
	LateUpdate(fTime);
	Collision(fTime);
	EditRender(fTime);
}

void CCore::EditDelete()
{
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CObjectManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CTimerManager);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CResourcesManager);
	CNavigationManager3D::DestroyInst();
	DESTROY_SINGLE(CEditManager);
	DESTROY_SINGLE(CDevice);
	Safe_Delete_Map(m_ManagerMap);
	Safe_Delete_Map(m_FSMap);
	//CoUninitialize();
}

void CCore::EditCreateObject(const std::string & _strTag)
{
	CScene* pScene = CSceneManager::GetInst()->GetScene();
	CLayer* pLayer = pScene->FindLayer("Default");
	CGameObject* pObject = CGameObject::CreateObject(_strTag, pLayer);

	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);
}

vector<float>* CCore::AddManagerVector(const string& KeyName)
{
	vector<float>* newVector = FindManagerMap(KeyName);

	if (newVector != NULLPTR)
		return newVector;

	newVector = new vector<float>();
	newVector->reserve(100);
	newVector->clear();

	m_ManagerMap.insert(make_pair(KeyName, newVector));

	return newVector;
}

vector<float>* CCore::FindManagerMap(const string& KeyName) const
{
	auto FindIter = m_ManagerMap.find(KeyName);

	if (FindIter == m_ManagerMap.end())
		return NULLPTR;

	return FindIter->second;
}

FileStream * CCore::FindFileStream(const string & KeyName) const
{
	auto FindIter = m_FSMap.find(KeyName);

	if (FindIter == m_FSMap.end())
		return NULLPTR;

	return FindIter->second;
}

void CCore::EditRender(float fTime)
{
	// 타겟뷰와 깊이뷰 초기화
	GET_SINGLE(CDevice)->Clear(m_fClearColor);
	GET_SINGLE(CSceneManager)->Render(fTime);
	GET_SINGLE(CRenderManager)->Render(fTime);

	GET_SINGLE(CInput)->RenderMouse(fTime);

	GET_SINGLE(CDevice)->Present();
	CInput::GetInst()->ClearWheel();
}