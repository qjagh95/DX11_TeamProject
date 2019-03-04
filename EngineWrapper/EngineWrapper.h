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
			cli::array<String^>^ arrStrMarshalList = gcnew cli::array<String^>(vecMeshNameList->size());
			for (size_t i = 0; i < vecMeshNameList->size(); ++i)
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
			//pManager->ObjectAddComponent(strTag);
		}

		void SetMesh()
		{
			
		}
	};
}