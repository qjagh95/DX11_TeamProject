
// TWEditorView.cpp: CTWEditorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "TWEditor.h"
#endif

#include "TWEditorDoc.h"
#include "TWEditorView.h"
#include "SceneComponent/EditScene.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/FreeCamera.h"
#include "GameObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTWEditorView

IMPLEMENT_DYNCREATE(CTWEditorView, CView)

BEGIN_MESSAGE_MAP(CTWEditorView, CView)
END_MESSAGE_MAP()

// CTWEditorView 생성/소멸

CTWEditorView::CTWEditorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTWEditorView::~CTWEditorView()
{
}

BOOL CTWEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTWEditorView 그리기

void CTWEditorView::OnDraw(CDC* /*pDC*/)
{
	CTWEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTWEditorView 진단

#ifdef _DEBUG
void CTWEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CTWEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTWEditorDoc* CTWEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTWEditorDoc)));
	return (CTWEditorDoc*)m_pDocument;
}
#endif //_DEBUG

// CTWEditorView 메시지 처리기

void CTWEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate(); 

	GET_SINGLE(CCore)->Init(AfxGetInstanceHandle(), m_hWnd, 1280, 720);
	CCore::GetInst()->SetGameMode(GM_3D);

	// Edit Scene 추가
	GET_SINGLE(CSceneManager)->AddSceneComponent<CEditScene>("EditScene");

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetScene();

	// 프리카메라 지정
	CGameObject*	pCameraObj = pScene->GetMainCameraObj();

	CFreeCamera*	pFreeCam = pCameraObj->AddComponent<CFreeCamera>("FreeCamera");
	
	SAFE_RELEASE(pFreeCam);

	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);
} 
