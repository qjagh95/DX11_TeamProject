#include "stdafx.h"
#include "EngineWrapper.h"

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

void CoreWrapper::CreateObject(String^ _strObjectTag, String^ _strLayerTag)
{
	string strObjectTag = ConvertMarshal<string, String^>(_strObjectTag);
	string strLayerTag  = ConvertMarshal<string, String^>(_strLayerTag);
	PUN::CEditManager::GetInst()->CreateObject(strObjectTag, strLayerTag);
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

void CoreWrapper::ActiveObjSetScale(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->ActiveObjectSetScale(_dX, _dY, _dZ);
}

void CoreWrapper::ActiveObjSetRotate(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->ActiveObjectSetRotate(_dX, _dY, _dZ);
}

void CoreWrapper::ActiveObjSetPosition(double _dX, double _dY, double _dZ)
{
	PUN::CEditManager::GetInst()->ActiveObjectSetPosition(_dX, _dY, _dZ);
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

void CoreWrapper::LoadMeshFromFullPath(String^ _strMeshKey, String^ _strFullPath)
{
	string strMeshKey = ConvertMarshal<string, String^>(_strMeshKey);
	wstring strFullPath = ConvertMarshal<wstring, String^>(_strFullPath);
	PUN::CResourcesManager::GetInst()->LoadMeshFromFullPath(strMeshKey, strFullPath.c_str());
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