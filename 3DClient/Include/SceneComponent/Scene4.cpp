#include "../ClientHeader.h""
#include "Scene4.h"
#include <Component/FreeCamera.h>

CScene4::CScene4()
{
}

CScene4::~CScene4()
{
}

void CScene4::Start()
{
}

bool CScene4::Init()
{
	CGameObject* pCamera = m_pScene->GetMainCameraObj();
	CFreeCamera* pFreeCamera = pCamera->AddComponent<CFreeCamera>("FreeCamera");
	SAFE_RELEASE(pFreeCamera);
	SAFE_RELEASE(pCamera);

	this->LoadMap();
	return true;
}

int CScene4::Input(float _fTime)
{
	return 0;
}

int CScene4::Update(float _fTime)
{
	return 0;
}

int CScene4::LateUpdate(float _fTime)
{
	return 0;
}

void CScene4::Collision(float _fTime)
{
}

void CScene4::Render(float _fTime)
{
}

const string TCHARToString(const TCHAR* _ptsz)
{
	// TCHAR -> string 변환
	int len = wcslen((wchar_t*)_ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)_ptsz, 2 * len + 1);
	string cvtStr = psz;
	delete[] psz;
	return cvtStr;
}

void CScene4::LoadMap()
{
	this->CreateEntryPoint();
	this->CreateCorridor();
}

void CScene4::CreateEntryPoint()
{
	// 시작 지점
	// 문을 열면서 시작한다.
	// 세 번째 씬과 연결점이 되어야한다.
	
	CLayer* pDefaultLayer	= CSceneManager::GetInst()->FindLayer("Default");
	
	// Frame
	CGameObject* pFrame		= CGameObject::CreateObject("DoorFrame", pDefaultLayer);
	CTransform* pFrameTr	= pFrame->GetTransform();
	CRenderer* pFrameRender	= pFrame->AddComponent<CRenderer>("Renderer_Frame");
	pFrameTr->SetWorldScale(0.1f, 0.1f, 0.1f);
	pFrameRender->SetMesh("DoorFrame", L"Door_Frame_Wood.msh", MESH_DATA_PATH);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pFrameTr);
	SAFE_RELEASE(pFrameRender);

	// Door
	CGameObject* pDoor		= CGameObject::CreateObject("EntryPoint_Door", pDefaultLayer);
	CTransform* pDoorTr		= pDoor->GetTransform();
	CRenderer* pDoorRender	= pDoor->AddComponent<CRenderer>("Renderer_Door");
	pDoorTr->SetWorldRotY(90.f);
	pDoorRender->SetMesh("EntryPoint_Door", L"Door_Wood-01.msh", MESH_DATA_PATH);
	SAFE_RELEASE(pDoor);
	SAFE_RELEASE(pDoorTr);
	SAFE_RELEASE(pDoorRender);
	SAFE_RELEASE(pDefaultLayer);
}

void CScene4::CreateCorridor()
{
	// 복도

}