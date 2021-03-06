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
#include "Component/ColliderSphere.h"
#include "Component/Decal.h"

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
	m_pNavObject(nullptr),
	m_LandScape(nullptr),
	m_isGizmoClick(false),
	m_pSelectCollider(nullptr),
	m_pDecal(nullptr)
{
}

CEditManager::~CEditManager()
{
	SAFE_RELEASE(m_pFreeCamObj);
	SAFE_RELEASE(m_pObject);
	SAFE_RELEASE(m_pScene);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pXGizmo);
	SAFE_RELEASE(m_pYGizmo);
	SAFE_RELEASE(m_pZGizmo);
	SAFE_RELEASE(m_pEditTest);
	SAFE_RELEASE(m_pXGizmoObj);
	SAFE_RELEASE(m_pYGizmoObj);
	SAFE_RELEASE(m_pZGizmoObj);
	SAFE_RELEASE(m_pArm);
	SAFE_RELEASE(m_pNavObject);
	SAFE_RELEASE(m_LandScape);
	SAFE_RELEASE(m_pSelectCollider);
	Safe_Delete_VecList(m_vecDivideFrame);
	SAFE_RELEASE(m_pDecal);
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

void CEditManager::GetObjFromCollTag()
{
	m_vecObjCollTag.clear();

	const std::list<CComponent*>* pCollFindList = m_pObject->FindComponentsFromType(CT_COLLIDER);
	
	std::list<CComponent*>::const_iterator Iter;
	std::list<CComponent*>::const_iterator EndIter = pCollFindList->end();
	for (Iter = pCollFindList->begin(); Iter != EndIter; ++Iter)
	{
		m_vecObjCollTag.push_back((*Iter)->GetTag());
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
	m_pEditTest->SetSave(false);

	SAFE_RELEASE(pLayer);

	return true;
}

void CEditManager::Render(float _fTime)
{
	if (m_pXGizmo->GetEnable())
	{
		m_pXGizmo->Render(_fTime);
	}

	if (m_pYGizmo->GetEnable())
	{
		m_pYGizmo->Render(_fTime);
	}

	if (m_pZGizmo->GetEnable())
	{
		m_pZGizmo->Render(_fTime);
	}
}

void CEditManager::GetLayerList(vector<string>* _pVec)
{
	m_pScene->GetLayerTagList(_pVec);
}

void CEditManager::PrivateEditObjSettingLayer()
{
	CLayer* pLayer = m_pScene->FindLayer("Default");
	pLayer->AddObject(m_pXGizmoObj);
	pLayer->AddObject(m_pYGizmoObj);
	pLayer->AddObject(m_pZGizmoObj);


	SAFE_RELEASE(pLayer);
}

void CEditManager::SetBrushSize(int _fSize)
{
	CInput::GetInst()->SetXBrushSize(_fSize);
}

void CEditManager::SetBrushXSize(int _fSize)
{
	CInput::GetInst()->SetXBrushSize(_fSize);
}

void CEditManager::SetBrushZSize(int _fSize)
{
	CInput::GetInst()->SetZBrushSize(_fSize);
}

void CEditManager::DeleteNavigationMesh()
{
	if (m_pNavObject)
	{
		m_pNavObject->Die();
	}
	SAFE_RELEASE(m_pNavObject);
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

	m_pObject = pLayer->FindObject(_strObjectTag);;
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

	GetObjFromCollTag();

	SAFE_RELEASE(pLayer);

	if (m_pDecal)
	{
		SAFE_RELEASE(m_pDecal);
	}


	CDecal* pDecal = m_pObject->FindComponentFromType<CDecal>(CT_DECAL);

	if (pDecal)
	{
		m_pDecal = pDecal;
		m_strDecalDiffuseTag = pDecal->GetDiffuseTexTag();
		m_strDecalNormalTag = pDecal->GetNormalTexTag();
		m_strDecalSpecularTag = pDecal->GetSpecTexTag();
	}
	else
	{
		m_strDecalDiffuseTag = "None";
		m_strDecalNormalTag = "None";
		m_strDecalSpecularTag = "None";
	}
}

void CEditManager::SetActiveObject(CGameObject * _pObject)
{
	if (m_pObject == _pObject)
		return;

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

vector<string> CEditManager::GetActiveObjectInfo()
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}

	vector<string> vecInfo;
	string strTag = m_pObject->GetTag();
	string strLayerTag = m_pObject->GetLayerName();
	vecInfo.push_back(strTag);
	vecInfo.push_back(strLayerTag);
	return vecInfo;
}

void CEditManager::CreateObject(const string _strObjectTag, const string _strLayerTag, bool _isChild)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		SAFE_RELEASE(pLayer);
		TrueAssert(true);
		return;
	}
	CGameObject* pObject = nullptr;

	pObject = CGameObject::CreateObject(_strObjectTag, pLayer);

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

	if (pObject->GetChildList()->size() > 0)
	{
		list<CGameObject*>::iterator iter;
		list<CGameObject*>::iterator iterEnd = pObject->GetChildList()->end();
		for (iter = pObject->GetChildList()->begin(); iter != iterEnd; ++iter)
		{
			(*iter)->SetParentNullptr();
		}
	}

	if (pObject->GetParentTag() != "None")
	{
		CGameObject* pParent = pLayer->FindObject(pObject->GetParentTag());

		if (pParent != nullptr)
		{
			pParent->DeleteChild(pObject);

			SAFE_RELEASE(pParent);
		}
	}

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


void CEditManager::SetLocalScale(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetLocalScale(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::SetLocalRotate(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetLocalRot(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::SetLocalPosition(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldPos(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::SetWorldScale(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldScale(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::SetWorldRotate(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
		return;

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldRot(Vector3((float)_dX, (float)_dY, (float)_dZ));
	SAFE_RELEASE(pTransform);
}

void CEditManager::SetWorldPosition(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
	{
		return;
	}

	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldPos((float)_dX, (float)_dY, (float)_dZ);
	SAFE_RELEASE(pTransform);
}

void CEditManager::SetWorldPivot(double _dX, double _dY, double _dZ)
{
	if (m_pObject == nullptr)
	{
		return;
	}
	CTransform* pTransform = m_pObject->GetTransform();
	pTransform->SetWorldPivot((float)_dX, (float)_dY, (float)_dZ);
	SAFE_RELEASE(pTransform);
}

bool CEditManager::FindRenderComponent()
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}
	CRenderer* pRenderer = m_pObject->FindComponentFromTypeNonCount<CRenderer>(CT_RENDERER);
	if (pRenderer != nullptr)
	{
		return true;
	}
	return false;
}

bool CEditManager::FindLightComponent()
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
	}
	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	if (pLight != nullptr)
	{
		return true;
	}
	return false;
}

void CEditManager::SetGizmoClick(bool _isFlag)
{
	m_isGizmoClick = _isFlag;
}

bool CEditManager::IsGizmoCheckClick()
{
	return m_isGizmoClick;
}

string CEditManager::GetMeshName(string _strTag)
{
	CGameObject* pObj = m_pScene->FindObject(_strTag);
	if(pObj == nullptr)
	{
		TrueAssert(true);
	}

	CRenderer* pRender = pObj->FindComponentFromType<CRenderer>(CT_RENDERER);
	if (pRender == nullptr)
	{
		SAFE_RELEASE(pObj);
		return "None";
	}

	CMesh* pMesh = pRender->GetMesh();

	if (!pMesh)
		return "CantFindMesh";

	string strMeshTag = pMesh->GetTag();

	SAFE_RELEASE(pRender);
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pObj);

	return strMeshTag;
}

void CEditManager::AddChild(const string _strObjectTag, const string _strLayerTag)
{
	if (m_pObject == nullptr)
	{
		TrueAssert(true);
		return;
	}

	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	CGameObject* pParent = pLayer->FindObject(_strObjectTag);
	pParent->AddChild(m_pObject, true);
	SAFE_RELEASE(pParent);
	SAFE_RELEASE(pLayer);
}

string CEditManager::GetParentTag()
{
	if (m_pObject->GetParent() == nullptr)
	{
		return "None";
	}

	return m_pObject->GetParentTag();
}

vector<Vector3> CEditManager::GetLocalTransform(const string _strObjectTag, const string _strLayerTag, int _eType)
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
			pVecTranform.push_back(pTr->GetLocalScale());
			break;
		}
		case eTransformType::TT_ROTATE:
		{
			pVecTranform.push_back(pTr->GetLocalRot());
			break;
		}
		case eTransformType::TT_POSITION:
		{
			pVecTranform.push_back(pTr->GetLocalPos());
			break;
		}
	}

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pLayer);

	return pVecTranform;
}

vector<Vector3> CEditManager::GetWorldTransform(const string _strObjectTag, const string _strLayerTag, int _eType)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		TrueAssert(true);
	}

	CGameObject* pObj = pLayer->FindObject(_strObjectTag);
	if (pObj == nullptr)
	{
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
		pVecTranform.push_back(pTr->GetWorldRelativePos());
		break;
	}
	}

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pLayer);

	return pVecTranform;
}

vector<Vector3> CEditManager::GetWorldPivot(string _strObjectTag, string _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		TrueAssert(true);
	}

	CGameObject* pObj = pLayer->FindObject(_strObjectTag);
	if (pObj == nullptr)
	{
		SAFE_RELEASE(pObj);
		TrueAssert(true);
	}

	CTransform* pTr = pObj->GetTransform();
	vector<Vector3> pVecPivot;
	pVecPivot.reserve(1);
	pVecPivot.push_back(pTr->GetPivot());

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pObj);
	SAFE_RELEASE(pLayer);

	return pVecPivot;
}

Vector3 CEditManager::GetChildWorldPosition(const string _strParentTag, const string _strLayerTag)
{
	CLayer* pLayer = m_pScene->FindLayer(_strLayerTag);
	if (pLayer == nullptr)
	{
		TrueAssert(true);
	}
	CGameObject* pParent = pLayer->FindObject(_strParentTag);
	if (pParent == nullptr)
	{
		TrueAssert(true);
	}

	CTransform* pChildTr = m_pObject->GetTransform();
	if (pChildTr == nullptr)
	{
		TrueAssert(true);
	}
	CTransform* pParentTr = pParent->GetTransform();

	float parentPosX = pParentTr->GetPosDelta()[3][0];
	float parentPosY = pParentTr->GetPosDelta()[3][1];
	float parentPosZ = pParentTr->GetPosDelta()[3][2];
	Vector3 vecParentPos = Vector3(parentPosX, parentPosY, parentPosZ);
	Vector3 vecChildPos = pChildTr->GetWorldPos();
	Vector3 vecChildWorldPos = Vector3(vecParentPos.x + vecChildPos.x,
									   vecParentPos.y + vecChildPos.y,
									   vecParentPos.z + vecChildPos.z);

	SAFE_RELEASE(pParent);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pChildTr);
	SAFE_RELEASE(pParentTr);

	return vecChildWorldPos;
}

void CEditManager::SetMouseWheel(short _sMouseWheel)
{
	CInput::GetInst()->SetWheelDir(_sMouseWheel);
}

void CEditManager::SetGizmoEnable(bool _bEnable)
{
	m_pXGizmoObj->SetEnable(_bEnable);
	m_pYGizmoObj->SetEnable(_bEnable);
	m_pZGizmoObj->SetEnable(_bEnable);
	m_pXGizmo->SetEnable(_bEnable);
	m_pYGizmo->SetEnable(_bEnable);
	m_pZGizmo->SetEnable(_bEnable);
}

void CEditManager::SetPickingColliderEnable(bool _bEnable)
{
	m_pScene->SetPickingColliderEnable(_bEnable);
	if (m_pObject)
		m_pObject->SetPickingColliderEnable(false);

}

void CEditManager::SetColliderEnable(bool _bEnable)
{
	m_pScene->SetColliderEnable(_bEnable);
}

bool CEditManager::CreateLandScape(int _iX, int _iZ)
{
	if (m_pNavObject == nullptr)
	{
		CLayer* pLayer = m_pScene->FindLayer("Default");
		m_pNavObject = CGameObject::CreateObject("LandTestObj", pLayer);
		m_pNavObject->SetSave(false);
		m_LandScape = m_pNavObject->AddComponent<CLandScape>("TestLandScape");
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

void CEditManager::SetAlphaEnable(bool _isFlag)
{
	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetAlphaEnable(_isFlag);
	SAFE_RELEASE(pRenderer);
}

bool CEditManager::GetAlphaEnabled()
{
	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	bool isEnabled = pRenderer->GetAlphaEnable();
	SAFE_RELEASE(pRenderer);
	return isEnabled;
}

void CEditManager::SetBumpScale(float _fScale)
{
	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetBumpScale(_fScale, 1.f);
	SAFE_RELEASE(pRenderer);
}

float CEditManager::GetBumpScale()
{
	CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	float fBumpScale = pRenderer->GetBumpScale();
	SAFE_RELEASE(pRenderer);
	return fBumpScale;
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
	if (m_pNavObject == NULLPTR)
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
	if (m_pNavObject)
	{
		m_pNavObject->Die();
	}

	SAFE_RELEASE(m_pNavObject);
	SAFE_RELEASE(m_LandScape);

	m_pNavObject = CGameObject::CreateObject("LandTestObj", pLayer);
	m_pNavObject->SetSave(false);
	m_LandScape = m_pNavObject->AddComponent<CLandScape>("TestLandScape");

	SAFE_RELEASE(pLayer);
	m_LandScape->LoadLandScape(FullPath);

	return true;
}

int CEditManager::GetLightType()
{
	if (m_pObject == nullptr)
	{
		return -1;
	}
	if (m_pObject->GetRenderGroup() != RG_LIGHT)
	{
		return -1;
	}

	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	if (pLight == nullptr)
	{
		return -1;
	}

	return (int)pLight->GetLightType();
}

float CEditManager::GetLightRange()
{
	if (m_pObject == nullptr)
	{
		return -1;
	}
	if (m_pObject->GetRenderGroup() != RG_LIGHT)
	{
		return -1;
	}

	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	if (pLight == nullptr)
	{
		return -1;
	}

	return pLight->GetRange();
}

void CEditManager::SetAngle(float _inAngle, float _outAngle)
{
	if (m_pObject == nullptr)
	{
		return;
	}
	if (m_pObject->GetRenderGroup() != RG_LIGHT)
	{
		return;
	}
	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	pLight->SetAngle(_inAngle, _outAngle);
}

float CEditManager::GetInAngle()
{
	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	return pLight->GetInAngle();
}

float CEditManager::GetOutAngle()
{
	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	return pLight->GetOutAngle();
}

vector<Vector4> CEditManager::GetSpecular()
{
	vector<Vector4> vecWpecular;
	if (m_pObject == nullptr)
	{
		return vecWpecular;
	}
	if (m_pObject->GetRenderGroup() != RG_LIGHT)
	{
		return vecWpecular;
	}

	CLight* pLight = m_pObject->FindComponentFromTypeNonCount<CLight>(CT_LIGHT);
	if (pLight == nullptr)
	{
		return vecWpecular;
	}

	vecWpecular.push_back(pLight->GetLightSpecular());
	return vecWpecular;
}

void CEditManager::CreateDefaultGlobalLight()
{
	m_pScene->CreateDefaultGlobalLight();
}

vector<string> CEditManager::GetGlobalLightList()
{
	vector<string> vecGlobalLightTag;
	vector<CGameObject*>* vecGlobalLight = m_pScene->GetGlobalLightList();
	vecGlobalLightTag.reserve(vecGlobalLight->size());
	for (size_t i = 0; i < (int)vecGlobalLight->size(); ++i)
	{
		string strTag = (*vecGlobalLight)[i]->GetTag();
		vecGlobalLightTag.push_back(strTag);
	}

	return vecGlobalLightTag;
}

bool CEditManager::FindActiveCollider(const std::string & _strTag)
{
	if (m_pSelectCollider)
		SAFE_RELEASE(m_pSelectCollider);

	m_pSelectCollider = m_pObject->FindComponentFromTag<CCollider>(_strTag);

	if (m_pSelectCollider == nullptr)
		return false;

	return true;
}

Vector3 CEditManager::GetOBBLength()
{
	if (m_pSelectCollider == nullptr)
	{
		return Vector3::Zero;
	}
	return ((CColliderOBB3D*)m_pSelectCollider)->GetRelativeInfo().vLength;
}

float CEditManager::GetSphereRadius()
{
	if (m_pSelectCollider == nullptr)
	{
		return 0.f;
	}
	return ((CColliderSphere*)m_pSelectCollider)->GetRelativeInfo().fRadius;
}

Vector3 CEditManager::GetOBBRelativeCenter()
{
	if (m_pSelectCollider == nullptr)
	{
		return Vector3::Zero;
	}
	return ((CColliderOBB3D*)m_pSelectCollider)->GetRelativeInfo().vCenter;
}

Vector3 CEditManager::GetSphereCenter()
{
	if (m_pSelectCollider == nullptr)
	{
		return Vector3::Zero;
	}
	return ((CColliderSphere*)m_pSelectCollider)->GetRelativeInfo().vCenter;
}

int CEditManager::GetCollType() const
{
	return (int)m_pSelectCollider->GetColliderType();
}

void CEditManager::SetSphereColliderInfo(float dCenterX, float dCenterY, float dCenterZ, float fRadius, bool _bMeshScale)
{
	if (m_pSelectCollider == nullptr)
	{
		return;
	}

	float fColliderRadius = fRadius;
	if (_bMeshScale)
	{
		CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Renderer Component가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}
		CMesh* pMesh = pRenderer->GetMesh();
		SAFE_RELEASE(pRenderer);
		if (pMesh == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Mesh가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}

		Vector3 vMeshLength = pMesh->GetLength();

		fColliderRadius *= vMeshLength.x;
		SAFE_RELEASE(pMesh);
	}

	((CColliderSphere*)m_pSelectCollider)->SetInfo(Vector3(dCenterX, dCenterY, dCenterZ), fColliderRadius);
}

void CEditManager::SetOBB3DColliderInfo(float dCenterX, float dCenterY, float dCenterZ, float dLengthX, float dLengthY, float dLengthZ, bool _bMeshScale)
{
	if (m_pSelectCollider == nullptr)
	{
		return;
	}

	float fLengthX = dLengthX;
	float fLengthY = dLengthY;
	float fLengthZ = dLengthZ;


	if (_bMeshScale)
	{
		CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Renderer Component가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}
		CMesh* pMesh = pRenderer->GetMesh();
		SAFE_RELEASE(pRenderer);
		if (pMesh == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Mesh가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}
		
		Vector3 vMeshLength = pMesh->GetLength();

		fLengthX *= vMeshLength.x;
		fLengthY *= vMeshLength.y;
		fLengthZ *= vMeshLength.z;

		SAFE_RELEASE(pMesh);
	}

	((CColliderOBB3D*)m_pSelectCollider)->SetInfo(Vector3(dCenterX, dCenterY, dCenterZ), Vector3::Axis , Vector3(fLengthX, fLengthY, fLengthZ));
}

void CEditManager::AddColliderOBB(float dCenterX, float dCenterY, float dCenterZ, float dLengthX, float dLengthY,
	float dLengthZ, int iColliderID, const std::string & _strTag, const std::string& _strCollTypeTag, const std::string& _strExceptTypeTag, bool _bMeshScale)
{
	float fLengthX = dLengthX;
	float fLengthY = dLengthY;
	float fLengthZ = dLengthZ;


	if (_bMeshScale)
	{
		CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Renderer Component가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}
		CMesh* pMesh = pRenderer->GetMesh();
		SAFE_RELEASE(pRenderer);
		if (pMesh == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Mesh가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}

		Vector3 vMeshLength = pMesh->GetLength();

		fLengthX *= vMeshLength.x;
		fLengthY *= vMeshLength.y;
		fLengthZ *= vMeshLength.z;

		SAFE_RELEASE(pMesh);
	}

	CColliderOBB3D* pOBB3D = m_pObject->AddComponent<CColliderOBB3D>(_strTag);
	pOBB3D->SetInfo(Vector3(dCenterX, dCenterY, dCenterZ), Vector3::Axis, Vector3(fLengthX, fLengthY, fLengthZ));
	pOBB3D->SetColliderID(iColliderID);
	pOBB3D->SetMyTypeName(_strCollTypeTag);
	pOBB3D->SetContinueTypeName(_strExceptTypeTag);
	SAFE_RELEASE(pOBB3D);
}

void CEditManager::AddColliderSphere(float dCenterX, float dCenterY, float dCenterZ, float fRadius,
	int iColliderID, const std::string & _strTag, const std::string& _strCollTypeTag, const std::string& _strExceptTypeTag, bool _bMeshScale)
{
	float fColliderRadius = fRadius;
	if (_bMeshScale)
	{
		CRenderer* pRenderer = m_pObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Renderer Component가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}
		CMesh* pMesh = pRenderer->GetMesh();
		SAFE_RELEASE(pRenderer);
		if (pMesh == nullptr)
		{
			MessageBox(WINDOWHANDLE, L"Mesh가 Active Object에 없습니다", L"Message", MB_OK);
			return;
		}

		Vector3 vMeshLength = pMesh->GetLength();

		fColliderRadius *= vMeshLength.x;
		SAFE_RELEASE(pMesh);
	}

	CColliderSphere* pSphere = m_pObject->AddComponent<CColliderSphere>(_strTag);
	pSphere->SetInfo(Vector3(dCenterX, dCenterY, dCenterZ), fColliderRadius);
	pSphere->SetColliderID(iColliderID);
	pSphere->SetMyTypeName(_strCollTypeTag);
	pSphere->SetContinueTypeName(_strExceptTypeTag);
	SAFE_RELEASE(pSphere);
}


void CEditManager::SetStageSection(STAGE_SECTION_TYPE _eType)
{
	if (m_pObject == nullptr)
	{
		return;
	}

	m_pObject->SetStageSection(_eType);
}

void CEditManager::AddDecalComponent()
{
	if (m_pObject == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object가 없습니다.", L"", MB_OK);
		return;
	}

	if (m_pObject->CheckComponentFromType(CT_DECAL) == true)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object에 Decal Component가 있습니다.", L"", MB_OK);
		return;
	}

	CDecal* pDecal = m_pObject->AddComponent<CDecal>("Decal");

	SAFE_RELEASE(pDecal);
}

void CEditManager::AddDecalDiffuseTex(const std::string& _strTag, const std::wstring & _strFileName)
{
	if (m_pObject == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object가 없습니다.", L"", MB_OK);
		return;
	}

	CDecal* pDecal = m_pObject->FindComponentFromType<CDecal>(CT_DECAL);

	if (pDecal == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object에 Decal Component가 없습니다.", L"", MB_OK);
		return;
	}
	CMaterial* pDecalMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pDecalMaterial->SetDiffuseTex(0, _strTag, _strFileName.c_str());
	pDecalMaterial->SetSampler(0, SAMPLER_LINEAR);
	pDecal->SetDiffuseTexTag(_strTag);
	m_strDecalDiffuseTag = _strTag;

	SAFE_RELEASE(pDecalMaterial);
	SAFE_RELEASE(pDecal);
}

void CEditManager::AddDecalNormalTex(const std::string& _strTag, const std::wstring & _strFileName)
{
	if (m_pObject == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object가 없습니다.", L"", MB_OK);
		return;
	}

	CDecal* pDecal = m_pObject->FindComponentFromType<CDecal>(CT_DECAL);

	if (pDecal == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object에 Decal Component가 없습니다.", L"", MB_OK);
		return;
	}

	CMaterial* pDecalMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pDecalMaterial->SetNormalTex(1, _strTag, _strFileName.c_str());
	pDecalMaterial->SetNormalSampler(0, SAMPLER_LINEAR);
	pDecal->SetNormalTexTag(_strTag);
	m_strDecalNormalTag = _strTag;

	SAFE_RELEASE(pDecalMaterial);
	SAFE_RELEASE(pDecal);
}

void CEditManager::AddDecalSpecularTex(const std::string& _strTag, const std::wstring & _strFileName)
{
	if (m_pObject == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object가 없습니다.", L"", MB_OK);
		return;
	}

	CDecal* pDecal = m_pObject->FindComponentFromType<CDecal>(CT_DECAL);

	if (pDecal == nullptr)
	{
		MessageBox(WINDOWHANDLE, L"현재 선택된 Object에 Decal Component가 없습니다.", L"", MB_OK);
		return;
	}

	CMaterial* pDecalMaterial = m_pObject->FindComponentFromType<CMaterial>(CT_MATERIAL);

	pDecalMaterial->SetSpecularTex(2, _strTag, _strFileName.c_str());
	pDecalMaterial->SetSpecularSampler(0, SAMPLER_LINEAR);
	pDecal->SetSpecTexTag(_strTag);
	m_strDecalSpecularTag = _strTag;
	SAFE_RELEASE(pDecalMaterial);
	SAFE_RELEASE(pDecal);
}

const string & CEditManager::GetDecalDiffuseTag()
{
	return m_strDecalDiffuseTag;
}

const string & CEditManager::GetDecalNormalTag()
{
	return m_strDecalNormalTag;
}

const string & CEditManager::GetDecalSpecularTag()
{
	return m_strDecalSpecularTag;
}
