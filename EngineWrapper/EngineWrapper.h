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

using namespace std;
using namespace System;
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
			PUN::CCore::GetInst()->SetGameMode(PUN::GM_3D);
			PUN::CCore::GetInst()->EditInit((HWND)WinHandle, (HINSTANCE)WinInstance, 1024, 600);

			// EditScene 
			// 툴에서 기본적으로 사용되는 객체 및 초기화 작업을 해주는 클래스
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

	// 레이어
	public:
		cli::array<String^>^ GetLayerList();

	// 오브젝트
	public:
		cli::array<String^>^ GetSelectLayerObjList(String^ _strLayerTag);
		void SetActiveObject(String^ _strObjectTag, String^ _strLayerTag);
		void CreateObject(String^ _strObjectTag, String^ _strLayerTag);
		void DeleteObject(String^ _strObjectTag, String^ _strLayerTag);
		void DeleteAllObject();
		void ChangeObjectTag(String^ _strObjectTag);
		void ChangeObjectInLayer(String^ _strLayerTag);
		void ActiveObjSetScale(double _dX, double _dY, double _dZ);
		void ActiveObjSetRotate(double _dX, double _dY, double _dZ);
		void ActiveObjSetPosition(double _dX, double _dY, double _dZ);
		cli::array<float>^ GetWorldTransform(String^ _strObjectTag, String^ _strLayerTag, int _type);

	// 메시
	public:
		cli::array<String^>^ GetMeshNameList();
		void LoadMeshFromFullPath(String^ _strMeshKey, String^ _strFullPath);
		void SetMesh(String^ _strMeshTag);
		void AddRenderComponent();

	// 애니메이션
	public:
		void LoadClipFromFullPath(String^ _strFullPath);
		cli::array<String^>^ GetClipTagList();
		void SetDivideClip();
		void DeleteDivideClip();
		void DeleteClip(String^ _strKey);
		bool ModifyClip(String^ _strKey, String^ _strChangeKey, int _iOption,
						int _iStartFrame, int _iEndFrame, double _dPlayTime);
		bool AddClip(String^ _strKey, int _iOption, int _iStartFrame, int _iEndFrame, double _dPlayTime);
		void ChangeClip(String^ _strKey);
		void ClipSave(String^ _strFullPath);

	// 파일 저장, 불러오기
	public:
		void FileSave(String^ _strFullPath);
		void FileLoad(String^ _strFullPath);

	public:
		// MT : Marshal Type
		// PT : Parameter Type
		template<typename MT, typename PT>
		MT ConvertMarshal(PT _data)
		{
			MT marshal = marshal_as<MT>(_data);
			return marshal;
		}
	};
}