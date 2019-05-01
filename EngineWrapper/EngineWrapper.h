#pragma once
#include <Core.h>
#include <Input.h>
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
#include <Rendering/RenderManager.h>
#include <Rendering/ViewManager.h>

using namespace std;
using namespace System;
using namespace msclr::interop;
using namespace PUN;

namespace EngineWrapper
{
	public ref class CoreWrapper
	{
	public:
		void Init(IntPtr hWnd, IntPtr hInstance)
		{
			auto WinHandle = hWnd.ToInt64();
			auto WinInstance = hInstance.ToInt64();
			CCore::GetInst()->SetGameMode(GM_3D);
			CCore::GetInst()->EditInit((HWND)WinHandle, (HINSTANCE)WinInstance, 1024, 600);

			// EditScene 
			// 툴에서 기본적으로 사용되는 객체 및 초기화 작업을 해주는 클래스
			CSceneManager::GetInst()->AddScene<PUN::EditorScene>("EditScene", "EditCom");

			GET_SINGLE(CEditManager)->GetInst()->Init();

		}

		void Logic()
		{
			CCore::GetInst()->EditLogic();
		}

		void Delete()
		{
			CCore::GetInst()->EditDelete();
		}

	// 레이어
	public:
		cli::array<String^>^ GetLayerList();

	// 오브젝트
	public:
		cli::array<String^>^ GetSelectLayerObjList(String^ _strLayerTag);
		void SetActiveObject(String^ _strObjectTag, String^ _strLayerTag);
		void CreateObject(String^ _strObjectTag, String^ _strLayerTag, bool _isChild);
		void DeleteObject(String^ _strObjectTag, String^ _strLayerTag);
		void DeleteAllObject();
		void ChangeObjectTag(String^ _strObjectTag);
		void ChangeObjectInLayer(String^ _strLayerTag);
		void SetLocalScale(double _dX, double _dY, double _dZ);
		void SetLocalRotate(double _dX, double _dY, double _dZ);
		void SetLocalPosition(double _dX, double _dY, double _dZ);
		void SetWorldScale(double _dX, double _dY, double _dZ);
		void SetWorldRotate(double _dX, double _dY, double _dZ);
		void SetWorldPosition(double _dX, double _dY, double _dZ);
		void SetWorldPivot(double _dx, double _dy, double _dz);
		bool FindRendererComponent();
		bool FindLightComponent();
		bool IsGizmoClick();
		void SetPickingFalse();
		bool IsPicking();
		String^ GetMeshName(String^ _strTag);

		cli::array<String^>^ GetActiveObjectInfo();

		void AddChild(String^ _strParentTag, String^ _strLayerTag);
		String^ GetParentTag();
		cli::array<float>^ GetLocalTransform(String^ _strObjectTag, String^ _strLayerTag, int _type);
		cli::array<float>^ GetWorldTransform(String^ _strObjectTag, String^ _strLayerTag, int _type);
		cli::array<float>^ GetWorldPivot(String^ _strObjectTag, String^ _strLayerTag);
		cli::array<float>^ GetChildWorldPosition(String^ _strParentTag, String^ _strLayerTag);

		void SetLightAmbient(double _dR, double _dG, double _dB)
		{
			CEditManager::GetInst()->SetLightAmbient(Vector4((float)(255.0f / _dR), (float)(255.0f / _dG), (float)(255.0f / _dB), 1.0f));
		}

		void SetLightDiffuse(double _dR, double _dG, double _dB)
		{
			CEditManager::GetInst()->SetLightDiffuse(Vector4((float)(255.0f / _dR), (float)(255.0f / _dG), (float)(255.0f / _dB), 1.0f));
		}

		void SetLightSpcular(double _dR, double _dG, double _dB, double _dA)
		{
			CEditManager::GetInst()->SetLightSpcular(Vector4((float)(255.0f / _dR), (float)(255.0f / _dG), (float)(255.0f / _dB), (float)(255.0f / _dA)));
		}

		void AddLightComponent()
		{
			CEditManager::GetInst()->AddLightComponent();
		}

		void SetLightType(int eType)
		{
			CEditManager::GetInst()->SetLightType((LIGHT_TYPE)eType);
		}

		void SetLightDirX(int x)
		{
			CEditManager::GetInst()->SetLightDirX((float)x);
		}

		void SetLightDirY(int y)
		{
			CEditManager::GetInst()->SetLightDirY((float)y);
		}

		void SetLightDirZ(int z)
		{
			CEditManager::GetInst()->SetLightDirZ((float)z);
		}

		void SetLightRange(int Range)
		{
			CEditManager::GetInst()->SetLightRange((float)Range);
		}

		void SetLightWireFrame(bool Value)
		{
			CRenderManager::GetInst()->m_bLightWireFrame = Value;
		}

		void SetTargetOnOff(bool Value)
		{
			CViewManager::GetInst()->SetTargetOnOff(Value);
		}

		void SetLightSpclularR(double r)
		{
			CEditManager::GetInst()->SetLightSpcularR((float)r);
		}

		void SetLightSpclularG(double g)
		{
			CEditManager::GetInst()->SetLightSpcularG((float)g);
		}

		void SetLightSpclularB(double b)
		{
			CEditManager::GetInst()->SetLightSpcularB((float)b);
		}

		void SetLightSpclularW(double w)
		{
			CEditManager::GetInst()->SetLightSpcularW((float)w);
		}

		void ChangeTarget(bool _bTarget)
		{
			CEditManager::GetInst()->ChangeCameraTarget(_bTarget);
		}
	// 메시
	public:
		cli::array<String^>^ GetMeshNameList();
		bool LoadMeshFromFullPath(String^ _strMeshKey, String^ _strFullPath);
		void SetMesh(String^ _strMeshTag);
		void AddRenderComponent();

	// 조명
	public:
		int GetLightType();
		cli::array<float>^ GetSpecular();
		float GetRange();

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

		void SetMouseWheel(int _iWheel)
		{
			CEditManager::GetInst()->SetMouseWheel((short)_iWheel);
		}
	  /////////////////////Navi/////////////////////////////////////////////////
		int GetSelectNaviIndex()
		{
			return CEditManager::GetInst()->GetNaviSelectIndex();
		}
		bool CreateTestLandScape(int _iX, int _iZ)
		{
			return CEditManager::GetInst()->CreateLandScape(_iX, _iZ);
		}
		void SetNaviEditorMode(bool _bMode)
		{
			CEditManager::GetInst()->SetNaviEditorMode(_bMode);
		}
		
		void SaveNavFile(String^ FullPath)
		{
			string strFullPath = ConvertMarshal<string, String^>(FullPath);
			CEditManager::GetInst()->SaveNavFile(strFullPath);
		}

		void LoadNavFile(String^ FullPath)
		{
			string strFullPath = ConvertMarshal<string, String^>(FullPath);
			CEditManager::GetInst()->LoadNavFile(strFullPath);
		}

		void SetBrushSize(int _dSize)
		{
			CEditManager::GetInst()->SetBrushSize(_dSize);
		}

		void SetBrushXSize(int _dSize)
		{
			CEditManager::GetInst()->SetBrushXSize(_dSize);
		}

		void SetBrushZSize(int _dSize)
		{
			CEditManager::GetInst()->SetBrushZSize(_dSize);
		}


		void DeleteNaviMesh()
		{
			CEditManager::GetInst()->DeleteNavigationMesh();
		}

		bool GetSelectNaviMove()
		{
			return CEditManager::GetInst()->GetNaviMove();
		}

		void SetGridShow(bool Value);
		void SetGridSize(int Value);
		
		void GizmoEnable(bool _bEnable)
		{
			CEditManager::GetInst()->SetGizmoEnable(_bEnable);
		}

		void PickingColliderEnable(bool _bEnable)
		{
			CEditManager::GetInst()->SetPickingColliderEnable(_bEnable);
		}
		void ObjectColliderEnable(bool _bEnable)
		{
			CEditManager::GetInst()->SetColliderEnable(_bEnable);
		}

		/////////////////////////////Collider/////////////////////////////////
		void AddColliderOBB(double dCenterX, double dCenterY, double dCenterZ, double dLengthX, double dLengthY, double dLengthZ,
			int iColliderID, String ^ _strTag , String ^ _strCollTypeTag , String ^ _strExceptTypeTag)
		{
			string strTag = ConvertMarshal<string, String^>(_strTag);
			string strCollTypeTag = ConvertMarshal<string, String^>(_strCollTypeTag);
			string strExceptTypeTag = ConvertMarshal<string, String^>(_strExceptTypeTag);
			CEditManager::GetInst()->AddColliderOBB((float)dCenterX, (float)dCenterY, (float)dCenterZ, (float)dLengthX, (float)dLengthY,
				(float)dLengthZ, iColliderID, strTag, strCollTypeTag, strExceptTypeTag);
		}

		void AddColliderSphere(double dCenterX, double dCenterY, double dCenterZ, double dRadius , int iColliderID, String ^ _strTag
			, String ^ _strCollTypeTag, String ^ _strExceptTypeTag)
		{
			string strTag = ConvertMarshal<string, String^>(_strTag);
			string strCollTypeTag = ConvertMarshal<string, String^>(_strCollTypeTag);
			string strExceptTypeTag = ConvertMarshal<string, String^>(_strExceptTypeTag);
			CEditManager::GetInst()->AddColliderSphere((float)dCenterX, (float)dCenterY, (float)dCenterZ,(float)dRadius
				, iColliderID, strTag, strCollTypeTag, strExceptTypeTag);
		}
	};
}