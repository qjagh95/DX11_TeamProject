#include "stdafx.h"
#include "EngineWrapper.h"
#include <Component/LandScape.h>

using namespace EngineWrapper;

cli::array<String^>^ CoreWrapper::GetLayerList()
{
	vector<string> vecLayerTag;
	PUN::CEditManager::GetInst()->GetLayerList(&vecLayerTag);
	cli::array<String^>^ arrStrMarshalList = gcnew cli::array<String^>((int)vecLayerTag.size());

	// Marshaling
	for (int i = 0; i < (int)vecLayerTag.size(); ++i)
	{
		String^ strMarshal = ConvertMarshal<String^, string>((vecLayerTag)[i]);
		arrStrMarshalList[i] = strMarshal;
	}

	return arrStrMarshalList;
}

cli::array<String^>^ CoreWrapper::GetSelectLayerObjList(String^ _strLayerTag)
{
	vector<string> vecObjTag;
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->GetSelectLayerObjList(strLayerTag, &vecObjTag);
	cli::array<String^>^ arrStrMarshalList = gcnew cli::array<String^>((int)vecObjTag.size());
	for (int i = 0; i < (int)vecObjTag.size(); ++i)
	{
		String^ strObjectTag = ConvertMarshal<String^, string>((vecObjTag)[i]);
		arrStrMarshalList[i] = strObjectTag;
	}

	return arrStrMarshalList;
}

void CoreWrapper::SetActiveObject(String^ _strObjectTag, String^ _strLayerTag)
{
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->SetActiveObject(strObjectTag, strLayerTag);
}

void CoreWrapper::CreateObject(String^ _strObjectTag, String^ _strLayerTag, bool _isChild)
{
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag  = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->CreateObject(strObjectTag, strLayerTag, _isChild);
}

void CoreWrapper::DeleteObject(String^ _strObjectTag, String^ _strLayerTag)
{
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->DeleteObject(strObjectTag, strLayerTag);
}

void CoreWrapper::DeleteAllObject()
{
	PUN::CEditManager::GetInst()->DeleteAllObject();
}

void CoreWrapper::ChangeObjectTag(String^ _strObjectTag)
{
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	PUN::CEditManager::GetInst()->ChangeObjectTag(strObjectTag);
}

void CoreWrapper::ChangeObjectInLayer(String^ _strLayerTag)
{
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->ChangeObjectInLayer(strLayerTag);
}

void CoreWrapper::SetLocalScale(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->SetLocalScale(_dX, _dY, _dZ);
}

void CoreWrapper::SetLocalRotate(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->SetLocalRotate(_dX, _dY, _dZ);
}

void CoreWrapper::SetLocalPosition(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->SetLocalPosition(_dX, _dY, _dZ);
}

void CoreWrapper::SetWorldScale(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->SetWorldScale(_dX, _dY, _dZ);
}

void CoreWrapper::SetWorldRotate(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->SetWorldRotate(_dX, _dY, _dZ);
}

void CoreWrapper::SetWorldPosition(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->SetWorldPosition(_dX, _dY, _dZ);
}

void CoreWrapper::SetWorldPivot(double _dx, double _dy, double _dz)
{
	PUN::CEditManager::GetInst()->SetWorldPivot(_dx, _dy, _dz);
}

bool CoreWrapper::FindRendererComponent()
{
	return PUN::CEditManager::GetInst()->FindRenderComponent();
}

bool CoreWrapper::FindLightComponent()
{
	return PUN::CEditManager::GetInst()->FindLightComponent();
}

bool CoreWrapper::IsGizmoClick()
{
	return PUN::CEditManager::GetInst()->IsGizmoCheckClick();
}

void CoreWrapper::SetPickingFalse()
{
	CInput::GetInst()->SetIsPickingFalse();
}

bool CoreWrapper::IsPicking()
{
	return CInput::GetInst()->GetIsPicking();
}

String^ EngineWrapper::CoreWrapper::GetMeshName(String^ _strTag)
{
	string strCvtTag = ConvertMarshal<string, String^>(_strTag);
	strCvtTag = PUN::CEditManager::GetInst()->GetMeshName(strCvtTag);
	String^ strTag = ConvertMarshal<String^, string>(strCvtTag);
	return strTag;
}

cli::array<String^>^ EngineWrapper::CoreWrapper::GetActiveObjectInfo()
{
	vector<string> vecObjInfo = PUN::CEditManager::GetInst()->GetActiveObjectInfo();
	cli::array<String^>^ arrActiveObjInfo = gcnew cli::array<String^>((int)vecObjInfo.size());
	String^ strTag = ConvertMarshal<String^, string>((vecObjInfo)[0]);
	String^ strLayerTag = ConvertMarshal<String^, string>((vecObjInfo)[1]);
	arrActiveObjInfo[0] = strTag;
	arrActiveObjInfo[1] = strLayerTag;

	return arrActiveObjInfo;
}

void CoreWrapper::AddChild(String^ _strParentTag, String^ _strLayerTag)
{
	string strObjectTag = ConvertMarshal<string, String^>(_strParentTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->AddChild(strObjectTag, strLayerTag);
}

String^ EngineWrapper::CoreWrapper::GetParentTag()
{
	string strParentTag = PUN::CEditManager::GetInst()->GetParentTag();
	String^ strCvtParentTag = ConvertMarshal<String^, string>(strParentTag);
	return strCvtParentTag;
}

cli::array<float>^ CoreWrapper::GetLocalTransform(String ^ _strObjectTag, String ^ _strLayerTag, int _type)
{
	vector<Vector3> pVecTranform;
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	pVecTranform = PUN::CEditManager::GetInst()->GetLocalTransform(strObjectTag, strLayerTag, _type);

	cli::array<float>^ arrTransform = gcnew cli::array<float>(3);
	arrTransform[0] = pVecTranform[0].x;
	arrTransform[1] = pVecTranform[0].y;
	arrTransform[2] = pVecTranform[0].z;

	return arrTransform;
}

cli::array<float>^ CoreWrapper::GetWorldTransform(String^ _strObjectTag, String^ _strLayerTag, int _type)
{
	vector<Vector3> pVecTranform;
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	pVecTranform = PUN::CEditManager::GetInst()->GetWorldTransform(strObjectTag, strLayerTag, _type);

	cli::array<float>^ arrTransform = gcnew cli::array<float>(3);
	arrTransform[0] = pVecTranform[0].x;
	arrTransform[1] = pVecTranform[0].y;
	arrTransform[2] = pVecTranform[0].z;

	return arrTransform;
}

cli::array<float>^ CoreWrapper::GetWorldPivot(String^ _strObjectTag, String^ _strLayerTag)
{
	vector<Vector3> pVecPivot;
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	pVecPivot = PUN::CEditManager::GetInst()->GetWorldPivot(strObjectTag, strLayerTag);

	cli::array<float>^ arrWorldPivot = gcnew cli::array<float>(3);
	arrWorldPivot[0] = pVecPivot[0].x;
	arrWorldPivot[1] = pVecPivot[0].y;
	arrWorldPivot[2] = pVecPivot[0].z;

	return arrWorldPivot;
}

cli::array<float>^ CoreWrapper::GetChildWorldPosition(String^ _strParentTag, String^ _strLayerTag)
{
	// 부모 오브젝트를 찾아서 자신의 위치와 더한 좌표를 돌려준다.
	cli::array<float>^ arrTransform = gcnew cli::array<float>(3);
	string strParentTag = ConvertMarshal<string, String^>(_strParentTag);
	string strLayerTag = ConvertMarshal<string, String^>(_strLayerTag);
	Vector3 pVecPosition = PUN::CEditManager::GetInst()->GetChildWorldPosition(strParentTag, strLayerTag);
	arrTransform[0] = pVecPosition.x;
	arrTransform[1] = pVecPosition.y;
	arrTransform[2] = pVecPosition.z;
	return arrTransform;
}

cli::array<String^>^ CoreWrapper::GetMeshNameList()
{
	vector<string> vecMeshNameList;
	PUN::CEditManager::GetInst()->GetMeshNameList(&vecMeshNameList);
	cli::array<String^>^ arrStrMarshalList = gcnew cli::array<String^>((int)vecMeshNameList.size());
	for (int i = 0; i < (int)vecMeshNameList.size(); ++i)
	{
		String^ strMeshTag = ConvertMarshal<String^, string>(vecMeshNameList[i]);
		arrStrMarshalList[i] = strMeshTag;
	}
	return arrStrMarshalList;
}

bool CoreWrapper::LoadMeshFromFullPath(String^ _strMeshTag, String^ _strFullPath)
{
	string strMeshKey = ConvertMarshal<string, String^>(_strMeshTag);
	wstring strFullPath = ConvertMarshal<wstring, String^>(_strFullPath);
	return PUN::CResourcesManager::GetInst()->LoadMeshFromFullPath(strMeshKey, strFullPath.c_str());
}

void CoreWrapper::SetMesh(String^ _strMeshTag)
{
	string strMeshTag = ConvertMarshal<string, String^>(_strMeshTag);
	PUN::CEditManager::GetInst()->SetMesh(strMeshTag);
}

void CoreWrapper::AddRenderComponent()
{
	PUN::CEditManager* pManager = PUN::CEditManager::GetInst();
	string strComponentTag = "renderer";
	pManager->AddComponent(strComponentTag);
}

int CoreWrapper::GetLightType()
{
	return PUN::CEditManager::GetInst()->GetLightType();
}

cli::array<float>^ CoreWrapper::GetSpecular()
{
	vector<Vector4> pVecTranform;
	pVecTranform = PUN::CEditManager::GetInst()->GetSpecular();

	cli::array<float>^ arrSpecular = gcnew cli::array<float>(4);
	arrSpecular[0] = pVecTranform[0].x;
	arrSpecular[1] = pVecTranform[0].y;
	arrSpecular[2] = pVecTranform[0].z;
	arrSpecular[3] = pVecTranform[0].w;

	return arrSpecular;
}

float CoreWrapper::GetRange()
{
	return PUN::CEditManager::GetInst()->GetLightRange();
}

void CoreWrapper::LoadClipFromFullPath(String^ _strFullPath)
{
	wstring strFullPath = ConvertMarshal<wstring, String^>(_strFullPath);
	PUN::CEditManager::GetInst()->LoadClipFromFullPath(strFullPath);
}

cli::array<String^>^ CoreWrapper::GetClipTagList()
{
	vector<string> vecstrClip;
	PUN::CEditManager::GetInst()->GetClipNameList(&vecstrClip);
	cli::array<String^>^ arrClipTag = gcnew cli::array<String^>((int)vecstrClip.size());
	for (int i = 0; i < (int)vecstrClip.size(); ++i)
	{
		String^ strClipTag = ConvertMarshal<String^, string>(vecstrClip[i]);
		arrClipTag[i] = strClipTag;
	}

	return arrClipTag;
}

void CoreWrapper::SetDivideClip()
{
	PUN::CEditManager::GetInst()->SetDivideKeyFrame();
}

void CoreWrapper::DeleteDivideClip()
{
	PUN::CEditManager::GetInst()->DeleteDivideKeyFrame();
}

void CoreWrapper::DeleteClip(String^ _strKey)
{
	string strOriginKey = ConvertMarshal<string, String^>(_strKey);
	PUN::CEditManager::GetInst()->DeleteClip(strOriginKey);
}

bool CoreWrapper::ModifyClip(String^ _strKey, String^ _strChangeKey, int _iOption,
	int _iStartFrame, int _iEndFrame, double _dPlayTime)
{
	string strOriginKey = ConvertMarshal<string, String^>(_strKey);
	string strChangeKey = ConvertMarshal<string, String^>(_strChangeKey);
	return PUN::CEditManager::GetInst()->ModifyClip(strOriginKey, strChangeKey, _iOption, _iStartFrame, _iEndFrame, (float)_dPlayTime);
}

bool CoreWrapper::AddClip(String^ _strKey, int _iOption, int _iStartFrame, int _iEndFrame, double _dPlayTime)
{
	string strOriginKey = ConvertMarshal<string, String^>(_strKey);
	return PUN::CEditManager::GetInst()->AddClip(strOriginKey, _iOption, _iStartFrame, _iEndFrame, (float)_dPlayTime);
}

void CoreWrapper::ChangeClip(String^ _strKey)
{
	string strOriginKey = ConvertMarshal<string, String^>(_strKey);
	PUN::CEditManager::GetInst()->ChangeClip(strOriginKey);
}
void CoreWrapper::ClipSave(String^ _strFullPath)
{
	string strFullPath = ConvertMarshal<string, String^>(_strFullPath);
	PUN::CEditManager::GetInst()->ClipSaveFromFullPath(strFullPath);
}

void CoreWrapper::FileSave(String^ _strFullPath)
{
	string strFullPath = ConvertMarshal<string, String^>(_strFullPath);
	PUN::CSceneManager::GetInst()->GetSceneNonCount()->Save(strFullPath);
}

void CoreWrapper::FileLoad(String^ _strFullPath)
{
	string strFullPath = ConvertMarshal<string, String^>(_strFullPath);
	PUN::CSceneManager::GetInst()->GetSceneNonCount()->Load(strFullPath);
}

void EngineWrapper::CoreWrapper::SetGridShow(bool Value)
{
	CLandScape* getLand = CEditManager::GetInst()->GetLandScape();

	if (getLand == NULLPTR)
		return;

	getLand->GridShow(Value);
}

void EngineWrapper::CoreWrapper::SetGridSize(int Value)
{
	CLandScape* getLand = CEditManager::GetInst()->GetLandScape();

	if (getLand == NULLPTR)
		return;

	getLand->SetGridLineSize(Value);
}
