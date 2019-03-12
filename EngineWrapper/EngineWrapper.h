#pragma once
#include <Core.h>
#include <Scene/Scene.h>
#include <EditorScene.h>
#include <Scene/SceneManager.h>
#include <EditManager.h>
#include <string>
#include <array>
#include <msclr/marshal.h>
#include <msclr/marshal_cppstd.h>
#include <cliext/vector>
#include <Resource/ResourcesManager.h>

using namespace System;
using namespace std;
using namespace msclr::interop;

namespace EngineWrapper
{
	public ref class CoreWrapper
	{
	public:
		void Init(IntPtr hWnd, IntPtr hInstance)
		{
			auto WinHandle = hWnd.ToInt64();
			auto WinInstance = hInstance.ToInt64();

			PUN::CCore::GetInst()->EditInit((HWND)WinHandle, (HINSTANCE)WinInstance, 1024, 600);
			PUN::CCore::GetInst()->SetGameMode(PUN::GM_3D);

			PUN::CSceneManager::GetInst()->AddSceneComponent<PUN::EditorScene>("EditScene");
		}

		void Logic()
		{
			PUN::CCore::GetInst()->EditLogic();
		}

		void Delete()
		{
			PUN::CCore::GetInst()->EditDelete();
		}

		void EditCreateObject(String ^ _str , String ^ _strLayerTag)
		{
			std::string str = marshal_as<std::string>(_str);
			std::string strLayerTag = marshal_as<std::string>(_strLayerTag);
			PUN::CEditManager::GetInst()->CreateObject(str , strLayerTag);
		}

		void ConvertString(String ^ _str)
		{
			std::string str = marshal_as<std::string>(_str);
		}

		void CurrentLayerGetObjTag(String ^ _strLayerTag)
		{
			std::string strLayerTag = marshal_as<std::string>(_strLayerTag);
			PUN::CEditManager::GetInst()->GetLayerListObjTag(strLayerTag);
		}
		int GetVecListObjSize()
		{
			return PUN::CEditManager::GetInst()->GetVecListObjSize();
		}

		String ^ GetIndexFromObjListTag(int _idx)
		{
			std::string strTag = PUN::CEditManager::GetInst()->GetIndexFromObjTag(_idx);
			String ^ strObjTag = marshal_as<String ^>(strTag);

			return strObjTag;
		}
		void SetIndexFromActiveObj(int _idx , String ^ _strLayerTag)
		{
			std::string str = marshal_as<std::string>(_strLayerTag);

			PUN::CEditManager::GetInst()->SetIndexFromSetObject(_idx , str);
		}

		void ActiveObjSetPos(double _dX, double _dY, double _dZ)
		{
			PUN::CEditManager::GetInst()->ActiveObjectSetPosition(_dX, _dY, _dZ);
		}

		void ActiveObjSetScale(double _dX, double _dY, double _dZ)
		{
			PUN::CEditManager::GetInst()->ActiveObjectSetScale(_dX, _dY, _dZ);
		}

		void ActiveObjSetRot(double _dX, double _dY, double _dZ)
		{
			PUN::CEditManager::GetInst()->ActiveObjectSetRotation(_dX, _dY, _dZ);
		}

		// Renderer
		cli::array<String^>^ GetMeshNameList()
		{
			PUN::CEditManager* pManager = PUN::CEditManager::GetInst();
			vector<string>* vecMeshNameList = pManager->GetMeshNameList();
			
			// Marshal
			cli::array<String^>^ arrStrMarshalList = gcnew cli::array<String^>((int)vecMeshNameList->size());
			for (int i = 0; i < (int)vecMeshNameList->size(); ++i)
			{
				String^ marshalStr = marshal_as<String^>((*vecMeshNameList)[i]);
				arrStrMarshalList[i] = marshalStr;
			}

			return arrStrMarshalList;
		}

		void AddRenderComponent()
		{
			// Renderer 컴포넌트 추가
			PUN::CEditManager* pManager = PUN::CEditManager::GetInst();
			std::string strTag = "renderer";
			pManager->ObjectAddComponent(strTag);
		}

		void SetMesh(String ^ _strMeshTag)
		{
			std::string str = marshal_as<std::string>(_strMeshTag);

			PUN::CEditManager::GetInst()->ActiveObjectFromSetMesh(str);
		}

		void SelectObjChangeTag(String ^ _strObjTag)
		{
			std::string str = marshal_as<std::string>(_strObjTag);

			PUN::CEditManager::GetInst()->ActiveObjectFromSetTag(str);
		}

		void LoadMeshFromFullPath(String ^ _strMeshKey , String ^ _strFullPath)
		{
			std::string strMeshKey = marshal_as<std::string>(_strMeshKey);
			std::wstring strFullPath = marshal_as<std::wstring>(_strFullPath);

			PUN::CResourcesManager::GetInst()->LoadMeshFromFullPath(strMeshKey, strFullPath.c_str());
		}

		void LoadClipFromFullPath(String ^ _strFullPath)
		{
			std::wstring strFullPath = marshal_as<std::wstring>(_strFullPath);
			PUN::CEditManager::GetInst()->LoadClipFromFullPath(strFullPath);
		}

		cli::array<String^>^ GetClipTagList()
		{
			vector<string> vecstrClip;
			PUN::CEditManager::GetInst()->GetClipNameList(&vecstrClip);

			cli::array<String^>^ arrStrMarshalList = gcnew cli::array<String^>((int)vecstrClip.size());
			for (int i = 0; i < (int)vecstrClip.size(); ++i)
			{
				String^ marshalStr = marshal_as<String^>(vecstrClip[i]);
				arrStrMarshalList[i] = marshalStr;
			}

			return arrStrMarshalList;
		}

		void SetDivideClip()
		{
			PUN::CEditManager::GetInst()->SetDivideKeyFrame();
		}

		void DeleteDivideClip()
		{
			PUN::CEditManager::GetInst()->DeleteDivideKeyFrame();
		}
		void DeleteClip(String ^ _strKey)
		{
			std::string strOriginKey = marshal_as<std::string>(_strKey);

			PUN::CEditManager::GetInst()->DeleteClip(strOriginKey);
		}

		bool ModifyClip(String ^ _strKey,
			String ^ _strChangeKey, int _iOption,
			int _iStartFrame, int _iEndFrame, double _dPlayTime)
		{
			std::string strOriginKey = marshal_as<std::string>(_strKey);

			std::string strChangeKey = marshal_as<std::string>(_strChangeKey);

			return PUN::CEditManager::GetInst()->ModifyClip(strOriginKey, strChangeKey, _iOption, _iStartFrame, _iEndFrame, (float)_dPlayTime);
		}

		bool AddClip(String ^ _strKey, int _iOption,
			int _iStartFrame, int _iEndFrame, double _dPlayTime)
		{
			std::string strOriginKey = marshal_as<std::string>(_strKey);

			return PUN::CEditManager::GetInst()->AddClip(strOriginKey, _iOption, _iStartFrame, _iEndFrame, (float)_dPlayTime);

			return true;
		}

		void ChangeClip(String ^ _strKey)
		{
			std::string strOriginKey = marshal_as<std::string>(_strKey);
			
			PUN::CEditManager::GetInst()->ChangeClip(strOriginKey);
		}
		void ClipSave(String ^ _strFullPath)
		{
			std::string strFullPath = marshal_as<std::string>(_strFullPath);

			PUN::CEditManager::GetInst()->ClipSaveFromFullPath(strFullPath);
		}
	};
}