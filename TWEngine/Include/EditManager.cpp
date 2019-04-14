#include "EngineHeader.h"
#include "Core.h"
#include "EditManager.h"
#include "GameObject.h"
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer.h"
#include "Component/Animation.h"
#include "Resource/ResourcesManager.h"
#include "Component/Gizmo.h"
#include "Component/EditTest.h"
#include "Component/Arm.h"
#include "Component/ColliderOBB3D.h"
#include "Component/Light.h"
#include "Component/Camera.h"
#include "Component/LandScape.h"
#include "NavigationMesh.h"


PUN_USING

CEditManager* CEditManager::m_pInst = nullptr;

CEditManager::CEditManager()
	:m_pScene(nullptr),
	m_pObject(nullptr),
	m_pAnimation(nullptr),
	m_pXGizmo(nullptr),
	m_pYGizmo(nullptr),
	m_pZGizmo(nullptr),
	m_pEditTest(nullptr),
	m_pXGizmoObj(nullptr),
	m_pYGizmoObj(nullptr),
	m_pZGizmoObj(nullptr),
	m_pArm(nullptr),
	m_bNaviEditorMode(false),
	m_pFreeCamObj(nullptr),
	m_NavObject(nullptr),
	m_LandScape(nullptr)
{
}

CEditManager::~CEditManager()
{
	SAFE_RELEASE(m_pFreeCamObj);
	SAFE_RELEASE(m_pObject);
	SAFE_RELEASE(m_pScene);
	SAFE_RELEASE(m_pAnimation);
	Safe_Delete_VecList(m_vecDivideFrame);
	SAFE_RELEASE(m_pXGizmo);
	SAFE_RELEASE(m_pYGizmo);
	SAFE_RELEASE(m_pZGizmo);
	SAFE_RELEASE(m_pEditTest);
	SAFE_RELEASE(m_pXGizmoObj);
	SAFE_RELEASE(m_pYGizmoObj);
	SAFE_RELEASE(m_pZGizmoObj);
	SAFE_RELEASE(m_pArm);
	SAFE_RELEASE(m_NavObject);
	SAFE_RELEASE(m_LandScape);
}

void CEditManager::SetFreeCamObj(CGameObject * _pObj)
{
	if (_pObj)
	{
		_pObj->AddRef();
		m_pFreeCamObj = _pObj;
	}
}

void CEditManager::SetArm(CArm * _pArm)
{
	if (_pArm)
	{
		_pArm->AddRef();
		m_pArm = _pArm;
	}
}

bool CEditManager::Init()
{
	m_pScene = CSceneManager::GetInst()->GetScene();
	if (m_pScene == nullptr)
	{
		TrueAssert(true);
		return false;
	}
	CLayer* pLayer = m_pScene->FindLayer("Default");

	m_pXGizmoObj = CGameObject::CreateObject("XGizmo", pLayer);
	m_pXGizmoObj->SetSave(false);
	m_pXGizmo = m_pXGizmoObj->AddComponent<CGizmo>("XGizmo");
	m_pXGizmo->SetGizmoType(GT_X);
	m_pXGizmoObj->SetRenderGroup(RG_GIZMO);

	m_pYGizmoObj = CGameObject::CreateObject("YGizmo", pLayer);
	m_pYGizmoObj->SetSave(false);
	m_pYGizmo = m_pYGizmoObj->AddComponent<CGizmo>("YGizmo");
	m_pYGizmo->SetGizmoType(GT_Y);
	m_pYGizmoObj->SetRenderGroup(RG_GIZMO);

	m_pZGizmoObj = CGameObject::CreateObject("ZGizmo", pLayer);
	m_pZGizmoObj->SetSave(false);
	m_pZGizmo = m_pZGizmoObj->AddComponent<CGizmo>("ZGizmo");
	m_pZGizmo->SetGizmoType(GT_Z);
	m_pZGizmoObj->SetRenderGroup(RG_GIZMO);

	m_pEditTest = new CEditTest;
	m_pEditTest->Init();

	SAFE_RELEASE(pLayer);

	return true;
}

void CEditManager::Render(float _fTime)
{
	m_pXGizmo->Render(_fTime);
	m_pYGizmo->Render(_fTime);
	m_pZGizmo->Render(_fTime);
}

void CEditManager::GetLayerList(vector<string>* _pVec)
{
	m_pScene->GetLayerTagList(_pVec);
}

void CEditManager::SetBlushSize(float _fSize)
{
	CInput::GetInst()->SetBlushSize(_fSize);
}

void CEditManager::DeleteNavigationMesh()
{
	if (m_NavObject)
	{
		m_NavObject->Die();
	}

	SAFE_RELEASE(m_NavObject);
	SAFE_RELEASE(m_LandScape);
}

void CEditManager::GetSelectLayerObjList(string _strLayerTag, vector<string>* _pVec)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	pLayer->GetLayerListObjTag(_pVec);
	SAFE_RELEASE(pLayer);
}

void CEditManager::SetActiveObject(const string _strObjectTag, const string _strLayerTag)
{
	// 기존 선택 오브젝트 해제
	if (m_pObject != nullptr)
	{
		CColliderOBB3D* pPickOBB = m_pObject->GetPickingOBB();
		if (pPickOBB)
		{
			pPickOBB->SetEnable(true);

			SAFE_RELEASE(pPickOBB);
		}

		m_pObject->RemoveComponent(m_pEditTest);
		SAFE_RELEASE(m_pObject);
	}
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	m_pObject = pLayer->FindObject(_strObjectTag);
	CColliderOBB3D* pPickOBB = m_pObject->GetPickingOBB();
	if (pPickOBB)
	{
		pPickOBB->SetEnable(false);

		SAFE_RELEASE(pPickOBB);
	}
	m_pXGizmo->SetTarget(m_pObject);
	m_pYGizmo->SetTarget(m_pObject);
	m_pZGizmo->SetTarget(m_pObject);
	m_pObject->AddComponent(m_pEditTest);

	SAFE_RELEASE(pLayer);
}

void CEditManager::SetActiveObject(CGameObject * _pObject)
{
	// 기존 선택 오브젝트 해제
	if (m_pObject != nullptr)
	{
		CColliderOBB3D* pPickOBB = m_pObject->GetPickingOBB();
		if (pPickOBB)
		{
			pPickOBB->SetEnable(true);

			SAFE_RELEASE(pPickOBB);
		}
		m_pObject->RemoveComponent(m_pEditTest);
		SAFE_RELEASE(m_pObject);
	}
	_pObject->AddRef();
	m_pObject = _pObject;
	CColliderOBB3D* pPickOBB = m_pObject->GetPickingOBB();

	if (pPickOBB)
	{
		pPickOBB->SetEnable(false);

		SAFE_RELEASE(pPickOBB);
	}
	m_pXGizmo->SetTarget(m_pObject);
	m_pYGizmo->SetTarget(m_pObject);
	m_pZGizmo->SetTarget(m_pObject);
	m_pObject->AddComponent(m_pEditTest);
}

void CEditManager::CreateObject(const string _strObjectTag, const string _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
		return;
	}
	CGameObject* pObject = CGameObject::CreateObject(_strObjectTag, pLayer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::DeleteObject(const string _strObjectTag, const string _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	CGameObject* pObject = pLayer->FindObject(_strObjectTag);
	pLayer->EraseObject(pObject);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::DeleteAllObject()
{
	vector<string> vecLayerTag;
	m_pScene->GetLayerTagList(&vecLayerTag);
	for (size_t i = 0; i < vecLayerTag.size(); ++i)
	{
		CLayer* pLayer = m_pScene->FindLayer(vecLayerTag[i]);
		pLayer->EraseObjectAll();
		SAFE_RELEASE(pLayer);
	}
}

void CEditManager::ChangeObjectTag(const string _strObjectTag)
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}
	m_pObject->SetTag(_strObjectTag);
}

void CEditManager::ChangeObjectInLayer(const string _strLayerTag)
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}

	// 기존 레이어에서 삭제
	CLayer* pLayer = m_pObject->GetLayer();
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	pLayer->EraseObject(m_pObject);

	// 새로운 레이어에 오브젝트 등록
	pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}
	pLayer->AddObject(m_pObject);
	SAFE_RELEASE(pLayer);
}

void CEditManager::AddComponent(std::string& _strCompTag)
{
	if (m_pObject == nullptr)
		return;

	std::string objTag = "";
	std::string compTag = "";

	// Renderer
	if (_strCompTag.compare("renderer") == 0)
	{
		objTag = m_pObject->GetTag();
		compTag = objTag + "renderer";

		// Add
		CRenderer* pRenderer = m_pObject->AddComponent<CRenderer>(compTag);
		SAFE_RELEASE(pRenderer);
	}
	// Light
	else if (_strCompTag.compare("light") == 0)
	{
		objTag = m_pObject->GetTag();
		compTag = objTag + "light";
	}
}

void CEditManager::ActiveObjectSetScale(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::ActiveObjectSetRotate(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldRot(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::ActiveObjectSetPosition(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
	{
		return;
	}

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldPos((float)_dX, (float)_dY, (float)_dZ);
	SAFE_RELEASE(pTransform);
}

vector<Vector3> CEditManager::GetWorldTransform(const string _strObjectTag, const string _strLayerTag, int _eType)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}

	CGameObject* pObj = pLayer->FindObject(_strObjectTag);
	if (pObj == nullptr)
	{
		SAFE_RELEASE(pObj);
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
	}

	CTransform* pTr = pObj->GetTransform();
	vector<Vector3> pVecTranform;
	pVecTranform.reserve(1);
	switch ((eTransformType)_eType)
	{
	case eTransformType::TT_SCALE:
	{
		pVecTranform.push_back(pTr->GetWorldScale());
		break;
	}
	case eTransformType::TT_ROTATE:
	{
		pVecTranform.push_back(pTr->GetWorldRot());
		break;
	}
	case eTransformType::TT_POSITION:
	{
		pVecTranform.push_back(pTr->GetWorldPos());
		break;
	}
	}

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pLayer);

	return pVecTranform;
}

void CEditManager::SetMouseWheel(short _sMouseWheel)
{
	CInput::GetInst()->SetWheelDir(_sMouseWheel);
}

void CEditManager::SetGizmoEnable(bool _bEnable)
{
	m_pXGizmo->SetEnable(_bEnable);
	m_pYGizmo->SetEnable(_bEnable);
	m_pZGizmo->SetEnable(_bEnable);
}

bool CEditManager::CreateLandScape(int _iX, int _iZ)
{
	if (m_NavObject == nullptr)
	{
		CLayer* pLayer = m_pScene->FindLayer("Default");
		m_NavObject = CGameObject::CreateObject("LandTestObj", pLayer);
		m_NavObject->SetSave(false);
		m_LandScape = m_NavObject->AddComponent<CLandScape>("TestLandScape");
		m_LandScape->CreateLandScape("TestLandScape", _iX, _iZ, "LandScapeDif", NULLPTR, NULLPTR, NULLPTR, NULLPTR);
		SAFE_RELEASE(pLayer);
	}
	else
	{
		return false;
	}
	
	return true;
}

int CEditManager::GetNaviSelectIndex() const
{
	return CInput::GetInst()->GetiSelectNavIndex();
}

void CEditManager::GetMeshNameList(vector<string>* _pVec)
{
	CResourcesManager::GetInst()->GetMeshNameList(_pVec);
}

void CEditManager::SetMesh(const string& _strMeshTag)
{
	if (m_pObject == nullptr)
		return;

	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer == nullptr)
		return;
	wstring wstrFileName;

	wstrFileName = CA2W(_strMeshTag.c_str());
	wstrFileName += L".msh";
	pRenderer->SetMesh(_strMeshTag, wstrFileName.c_str());
	SAFE_RELEASE(pRenderer);
}

void CEditManager::ChangeCameraTarget(bool _bTarget)
{
	if (_bTarget)
	{
		CCamera* pCamera = m_pScene->GetMainCamera();
		pCamera->SetTarget(m_pObject);
		m_pArm->SetTarget(m_pObject);
		SAFE_RELEASE(pCamera);
	}
	else
	{
		CCamera* pCamera = m_pScene->GetMainCamera();
		pCamera->SetTarget(m_pFreeCamObj);
		m_pArm->SetTarget(m_pFreeCamObj);
		SAFE_RELEASE(pCamera);
	}
}

void CEditManager::LoadClipFromFullPath(const wstring& _strFullPath)
{
	if (m_pObject == nullptr)
		return;

	if (m_pAnimation)
	{
		SAFE_RELEASE(m_pAnimation);
		m_pObject->RemoveComponentFromType(CT_ANIMATION);
	}

	m_pAnimation = m_pObject->AddComponent<CAnimation>("Animation");
	int	iLength = (int)_strFullPath.length();

	if (_strFullPath[iLength - 3] == 'f' &&
		_strFullPath[iLength - 2] == 'b' &&
		_strFullPath[iLength - 1] == 'x')
		m_pAnimation->AddClip(_strFullPath.c_str());

	else
		m_pAnimation->LoadBoneAndAnimationFullPath(_strFullPath.c_str());
}

void CEditManager::GetClipNameList(vector<string>* _vecstrClipList)
{
	if (m_pAnimation == nullptr)
		return;

	m_pAnimation->GetClipTagList(_vecstrClipList);
}

void CEditManager::SetDivideKeyFrame()
{
	if (m_pAnimation == nullptr)
		return;

	m_pAnimation->GetCurrentKeyFrame(m_vecDivideFrame);
}

void CEditManager::DeleteDivideKeyFrame()
{
	if (m_vecDivideFrame.empty())
		return;

	Safe_Delete_VecList(m_vecDivideFrame);
}

void CEditManager::DeleteClip(const string& _strKey)
{
	if (m_pAnimation == nullptr)
		return;

	m_pAnimation->DeleteClip(_strKey);
}

bool CEditManager::ModifyClip(const string& strKey, const string& strChangeKey, int iOption,
	int iStartFrame, int iEndFrame, float fPlayTime)
{
	if (m_pAnimation == nullptr)
		return false;

	if (m_vecDivideFrame.empty())
		return false;

	return m_pAnimation->ModifyClip(strKey, strChangeKey, (ANIMATION_OPTION)iOption, iStartFrame, iEndFrame, fPlayTime, m_vecDivideFrame);
}

bool CEditManager::AddClip(const string& strKey, int iOption, int iStartFrame, int iEndFrame, float fPlayTime)
{
	if (m_pAnimation == nullptr)
		return false;

	if (m_vecDivideFrame.empty())
		return false;

	m_pAnimation->AddClip(strKey, (ANIMATION_OPTION)iOption, iStartFrame, iEndFrame, fPlayTime, m_vecDivideFrame);

	return true;
}

void CEditManager::ChangeClip(const string& _strKey)
{
	if (m_pAnimation == nullptr)
		return;

	m_pAnimation->ChangeClip(_strKey);
}

void CEditManager::ClipSaveFromFullPath(const string& _strFullPath)
{
	if (m_pAnimation == nullptr)
		return;

	m_pAnimation->SaveFromFullPath(_strFullPath.c_str());
}

void CEditManager::AddLightComponent()
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetComponentSize() > 2)
		return;

	CLight* newLight = m_pObject->AddComponent<CLight>(m_pObject->GetTag());
	newLight->SetLightType(LT_DIR);
	newLight->SetLightRange(10.0f);

	SAFE_RELEASE(newLight);
}

void CEditManager::SetLightType(LIGHT_TYPE eType)
{
	if (m_pObject == nullptr)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightType(eType);
}

void CEditManager::SetLightDiffuse(const Vector4 & _vDiffuse)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightDiffuse(_vDiffuse);
}

void CEditManager::SetLightAmbient(const Vector4 & _vAmbient)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightAmbient(_vAmbient);
}

void CEditManager::SetLightSpcular(const Vector4 & _vSpcular)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightAmbient(_vSpcular);
}

void CEditManager::SetLightSpcularR(float r)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightSpcular(Vector4(r, getLight->GetLightInfo().vSpc.y, getLight->GetLightInfo().vSpc.z, getLight->GetLightInfo().vSpc.w));
}

void CEditManager::SetLightSpcularG(float g)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightSpcular(Vector4(getLight->GetLightInfo().vSpc.x, g, getLight->GetLightInfo().vSpc.z, getLight->GetLightInfo().vSpc.w));
}

void CEditManager::SetLightSpcularB(float b)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightSpcular(Vector4(getLight->GetLightInfo().vSpc.x, getLight->GetLightInfo().vSpc.y, b, getLight->GetLightInfo().vSpc.w));
}

void CEditManager::SetLightSpcularW(float w)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightSpcular(Vector4(getLight->GetLightInfo().vSpc.x, getLight->GetLightInfo().vSpc.y, getLight->GetLightInfo().vSpc.z, w));
}

void CEditManager::SetLightDirX(float x)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightDirectionX(x / 100.0f);
}

void CEditManager::SetLightDirY(float y)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightDirectionY(y / 100.0f);
}

void CEditManager::SetLightDirZ(float z)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightDirectionZ(z / 100.0f);
}

void CEditManager::SetLightRange(float Range)
{
	if (m_pObject == nullptr)
		return;

	if (m_pObject->GetRenderGroup() != RG_LIGHT)
		return;

	CLight* getLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);

	if (getLight == NULLPTR)
		return;

	getLight->SetLightRange(Range);
}

bool CEditManager::SaveNavFile(const string & FullPath)
{
	if (m_NavObject == NULLPTR)
		return false;

	if (m_LandScape == NULLPTR)
		return false;

	if (m_LandScape->GetNaviMesh() == NULLPTR)
		return false;

	m_LandScape->SaveLandScape(FullPath);
	m_LandScape->GetNaviMesh()->SaveFromFullPath(FullPath.c_str());

	return true;
}

bool CEditManager::LoadNavFile(const string & FullPath)
{
	CLayer* pLayer = m_pScene->FindLayer("Default");
	if (m_NavObject)
	{
		m_NavObject->Die();
	}

	SAFE_RELEASE(m_NavObject);
	SAFE_RELEASE(m_LandScape);

	m_NavObject = CGameObject::CreateObject("LandTestObj", pLayer);
	m_NavObject->SetSave(false);
	m_LandScape = m_NavObject->AddComponent<CLandScape>("TestLandScape");

	SAFE_RELEASE(pLayer);
	m_LandScape->LoadLandScape(FullPath);

	return true;
}
