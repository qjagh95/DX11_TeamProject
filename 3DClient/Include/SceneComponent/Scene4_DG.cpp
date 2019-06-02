#include "../ClientHeader.h"
#include "Scene4_DG.h"
#include "../UserComponent/Door.h"
#include "../UserComponent/Human_Player.h"
#include <StrUtility.h>
#include <Component/Light.h>
#include <Component/FreeCamera.h>
#include <Component/ColliderOBB3D.h>

// �׽�Ʈ
#define IS_TEST			true
#define IS_CONVERTFBX	false

CScene4::CScene4() :
	m_pObjPlayer(nullptr),
	m_pHumanPlayer(nullptr),
	m_pObjHeavyDoor(nullptr),
	m_pHeavyDoor(nullptr),
	m_pObjBSDecal(nullptr),
	m_pObjBBDecal(nullptr),
	m_pBSDecal(nullptr),
	m_pBBDecal(nullptr)
{
}

CScene4::~CScene4()
{
#if IS_TEST
	// �޸� ����
	SAFE_RELEASE(m_pObjPlayer);
	SAFE_RELEASE(m_pHumanPlayer);

#endif
	SAFE_RELEASE(m_pObjHeavyDoor);
	SAFE_RELEASE(m_pHeavyDoor);
	SAFE_RELEASE(m_pObjBSDecal);
	SAFE_RELEASE(m_pObjBBDecal);
	SAFE_RELEASE(m_pBSDecal);
	SAFE_RELEASE(m_pBBDecal);
}

void CScene4::Start()
{
}

bool CScene4::Init()
{
#if IS_CONVERTFBX
	// FBX ���� �ε�
	this->ConvertCorridorFBXFiles();
#endif

	// ��������4 �� ������ ���� �ε�
	wstring wstrPath = CPathManager::GetInst()->FindPath(DATA_PATH);
	wstrPath += L"Stage4_DG.dat";
	string strDataPath = CW2A(wstrPath.c_str());
	m_pScene->Load(strDataPath);
	//
	//// ������(Directional Light) ����
	//Vector4 vWhiteColor = Vector4(1.f, 1.f, 1.f, 1.0f);
	//Vector4 vDarkColor  = Vector4(0.005f, 0.005f, 0.005f, 1.0f);
	//CLayer* pLayer = m_pScene->FindLayer("Light");
	//list<CGameObject*>* pLightList = pLayer->GetObjectList();
	//list<CGameObject*>::iterator iter;
	//list<CGameObject*>::iterator iterEnd = pLightList->end();
	//for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	//{
	//	CLight* pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);
	//	pLight->SetLightColor(vWhiteColor, vWhiteColor, vWhiteColor);
	//	SAFE_RELEASE(pLight);
	//}
	//SAFE_RELEASE(pLayer);

	// ��Į(Blood Screen Space Decal)
	//CreateDecal();



	// �ʱ�ȭ
	{
		//SetDoorInit();
	}


	/*
	CreateBloodDecal();*/

	// y�� �������� ��Į�� ���δ�.


	//CLayer* pLayer = m_pScene->FindLayer("Default");

	// BloodDecal_Small
	//CGameObject* pObjBSDecal = CGameObject::CreateObject("Decal", pLayer);
	//CTransform* pBSTr = pObjBSDecal->GetTransform();
	//pBSTr->SetWorldRotX(-90.f);
	//pBSTr->SetWorldScale(5.f, 5.f, 5.f);
	//pBSTr->SetWorldPos(10.f, 5.f, 8.f);
	//CDecal* pBSDecal = pObjBSDecal->AddComponent<CDecal>("Decal");
	//CRenderer* pRenderer = pObjBSDecal->FindComponentFromTag<CRenderer>("DecalRenderer");
	//pRenderer->SetRenderState(ALPHA_BLEND);
	//// SetDiffuseTex, NormalTex, SpecularTex() ȣ�� �� Texture�� ������ش�.
	//CMaterial* pBSMtrl = pObjBSDecal->FindComponentFromType<CMaterial>(CT_MATERIAL);
	//pBSMtrl->SetDiffuseTex(0, "BloodDecalD_Small", TEXT("Decal/ControlCenter.png"));
	///*pBSMtrl->SetNormalTex(1, "BloodDecalN_Small", TEXT("Decal/ControlCenter.png"));
	//pBSMtrl->SetSpecularTex(2, "BloodDecalS_Small", TEXT("Decal/ControlCenter.png"));*/
	//pBSMtrl->SetSampler(0, SAMPLER_LINEAR);
	////pBSMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	////pBSMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);


	/*CColliderOBB3D*	pDecalSphere = pObjBSDecal->AddComponent<CColliderOBB3D>("DecalSphere");
	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(2.5f, 1.5f, 2.5f));
	SAFE_RELEASE(pDecalSphere);*/






	//SAFE_RELEASE(pLayer);

	return true;
}

void CScene4::AfterInit()
{
#if IS_TEST
	CLayer* pLayer = m_pScene->FindLayer("Default");
	m_pObjPlayer = CGameObject::CreateObject("Player", pLayer);
	CTransform* pPlayerTr = m_pObjPlayer->GetTransform();
	pPlayerTr->SetWorldPos(Vector3::Zero);
	m_pHumanPlayer = m_pObjPlayer->AddComponent<CHuman_Player>("UC_HumanPlayer");
	CRenderer* pRenderer = m_pObjPlayer->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDecalEnable(true);
	SAFE_RELEASE(pPlayerTr);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);

#else
	CLayer* pLayer = m_pScene->FindLayer("Default");
	m_pObjPlayer = CGameObject::CreateObject("Player", pLayer);
	CTransform* pPlayerTr = m_pObjPlayer->GetTransform();
	pPlayerTr->SetWorldPos(Vector3::Zero);
	m_pHumanPlayer = m_pObjPlayer->AddComponent<CHuman_Player>("UC_HumanPlayer");
	CRenderer* pRenderer = m_pObjPlayer->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->SetDecalEnable(true);

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);


	//// �÷��̾�
	//m_pObjPlayer = CObjectManager::GetInst()->FindObject("Player");
	//if (m_pObjPlayer == nullptr)
	//{
	//	TrueAssert(true);
	//}
	//// 2019/05/02 Comment : "UC_HumanPlayer" ������Ʈ �±״� ���Ŀ� �ٲ� �� �ִ�.
	//m_pHumanPlayer = m_pObjPlayer->FindComponentFromTag<CHuman_Player>("UC_HumanPlayer");
	//if (m_pHumanPlayer == nullptr)
	//{
	//	TrueAssert(true);
	//}
#endif
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

void CScene4::ConvertCorridorFBXFiles()
{
	CStrUtility* pUtil = CStrUtility::GetInst();

	// .FBX ���� ���� ���
	vector<wstring> vecException;
	vecException.push_back(L"Waylon2.FBX");
	vecException.push_back(L"shooter_anim.FBX");
	vecException.push_back(L"Enem_Cannibal.FBX");
	vecException.push_back(L"SE_handycam_HandyCam.FBX");
	vecException.push_back(L"Handycam.FBX");
	vecException.push_back(L"glock_OnlyFire.FBX");

	// MESH_PATH �����ȿ� .FBX ���ϸ��� ��� �о�´�.
	const TCHAR* path = CPathManager::GetInst()->FindPath(MESH_PATH);
	const TCHAR* meshDataPath = CPathManager::GetInst()->FindPath(MESH_DATA_PATH);
	string strPath = pUtil->TCHARToString(path);
	wstring wstrPath = L"";
	wstrPath.assign(strPath.begin(), strPath.end());
	vector<wstring> vecFBXList = pUtil->GetFilesInFolder(wstrPath, L"*.FBX*");

	// �о�� .FBX ���ϸ��� ���� .FBX ���ϸ��� ��ġ�ϸ� �����Ѵ�.
	for (size_t i = 0; i < vecFBXList.size(); ++i)
	{
		for (size_t j = 0; j < vecException.size(); ++j)
		{
			if (vecFBXList[i] == vecException[j])
			{
				vecFBXList.erase(vecFBXList.begin() + i);
				break;
			}
		}
	}

	// .FBX ������ .MSH ���Ϸ� ��ȯ�Ѵ�.
	CLayer* pLayer = m_pScene->FindLayer("Default");
	CGameObject* pObject = CGameObject::CreateObject("ConvertFBX", pLayer);
	CRenderer* pRenderer = pObject->AddComponent<CRenderer>("Renderer");
	for (size_t i = 0; i < vecFBXList.size(); ++i)
	{
		std::wstring wstrFileName = vecFBXList[i];
		std::string strFileName = "";
		strFileName.assign(wstrFileName.begin(), wstrFileName.end());
		const TCHAR* fileName = pUtil->StringToTCHAR(strFileName);
		pRenderer->SetMesh(strFileName, fileName, MESH_PATH);

		// ��ȯ�� .MSH ������ ���(MESH_DATA_PATH)�� �̵���Ű�µ�, �̶� �̹� �ִ� �����̸� �����Ѵ�.
		string strTok = strtok(const_cast<char*>(strFileName.c_str()), ".");
		strTok += ".msh";
		wstring wstrExistingFileName = CA2W((pUtil->TCHARToString(path) + strTok).c_str());
		wstring wstrNewFileName = CA2W((pUtil->TCHARToString(meshDataPath) + strTok).c_str());
		BOOL isDeleteResult = DeleteFile(wstrNewFileName.c_str());

		// .MSH ������ �ӽ������� �����Ѵ�. (�� �ϵ��)
		/*wstring wstrCopyFolder = CA2W((pUtil->TCHARToString(L"D:\\TempMshFile\\") + strTok).c_str());
		CopyFile(wstrExistingFileName.c_str(), wstrCopyFolder.c_str(), false);*/

		// .MSH ���� �̵�
		BOOL isMoveResult = MoveFile(wstrExistingFileName.c_str(), wstrNewFileName.c_str());
	}
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pObject);
	SAFE_RELEASE(pLayer);
}

void CScene4::InitDecal()
{
	// �� �ڱ�


}

void CScene4::SetDoorInit()
{
	CLayer* pLayer = m_pScene->FindLayer("Default");
	CTransform* pTr = nullptr;

	// Stage4 Entry Point(��������4 ������) ������Ʈ

	// Stage4 Gun-Room Entry Point ������Ʈ

	// Gun-Room�� �����ϴ� Ÿ��(Heavy) ������Ʈ
	m_pObjHeavyDoor = pLayer->FindObject("UC_GunRoom_Door");
	if (m_pObjHeavyDoor == nullptr)
	{
		return;
	}
	m_pHeavyDoor = m_pObjHeavyDoor->AddComponent<CDoor>("HeavyDoor");
	if (m_pHeavyDoor == nullptr)
	{
		return;
	}
	m_pHeavyDoor->SetDoorType(DOOR_TYPE::DOOR_HEAVY);
	m_pHeavyDoor->SetOpenRot(60.f);
	m_pHeavyDoor->SetOpenTime(10.f);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pLayer);
}

//void CScene4::CreateDecal(const string& _strTag,   const string& _decalTag,
//						  const string& _difName,  const string& _difTexName, 
//						  const string& _norName,  const string& _norTexName,
//						  const string& _specName, const string& _specTexName,
//						  Vector3 _scale, Vector3 _rotate, Vector3 _pos)
//{
	//CLayer* pLayer = m_pScene->FindLayer("Default");

	//// BloodDecal_Small
	//m_pObjBSDecal = CGameObject::CreateObject("Decal", pLayer);
	//CTransform* pBSTr = m_pObjBSDecal->GetTransform();
	//pBSTr->SetWorldRotZ(-90.f);
	//pBSTr->SetWorldScale(5.f, 5.f, 5.f);
	//pBSTr->SetWorldPos(8.f, 5.f, 1.f);
	//m_pBSDecal = m_pObjBSDecal->AddComponent<CDecal>("Decal");
	//CRenderer* pRenderer = m_pObjBSDecal->FindComponentFromTag<CRenderer>("DecalRenderer");
	//pRenderer->SetRenderState(ALPHA_BLEND);
	//// SetDiffuseTex, NormalTex, SpecularTex() ȣ�� �� Texture�� ������ش�.
	//CMaterial* pBSMtrl = m_pObjBSDecal->FindComponentFromType<CMaterial>(CT_MATERIAL);
	//pBSMtrl->SetDiffuseTex(0,  "BloodDecalD_Small", TEXT("Decal/Shout24674-perfil3_COLOR.png"));
	//pBSMtrl->SetNormalTex(1,   "BloodDecalN_Small", TEXT("Decal/Shout24674-perfil3_NRM.png"));
	//pBSMtrl->SetSpecularTex(2, "BloodDecalS_Small", TEXT("Decal/Shout24674-perfil3_SPEC.png"));
	//pBSMtrl->SetSampler(0, SAMPLER_LINEAR);
	//pBSMtrl->SetNormalSampler(0, SAMPLER_LINEAR);
	//pBSMtrl->SetSpecularSampler(0, SAMPLER_LINEAR);
//
//	CColliderOBB3D*	pDecalSphere = m_pObjBSDecal->AddComponent<CColliderOBB3D>("DecalSphere");
//	pDecalSphere->SetInfo(Vector3::Zero, Vector3::Axis, Vector3(2.5f, 1.5f, 2.5f));
//	SAFE_RELEASE(pDecalSphere);
//
//	SAFE_RELEASE(pBSMtrl);
//	SAFE_RELEASE(pRenderer);
//	SAFE_RELEASE(pBSTr);
//	SAFE_RELEASE(pLayer);
//
//	// BloodDecal_Big
//	//CMaterial* pBBMtrl = m_pObjBBDecal->FindComponentFromType<
//}

void CScene4::SetAmbientSoundPlay(const string& _soundName)
{
	// ȯ��(�ֺ�) ���� ���
}