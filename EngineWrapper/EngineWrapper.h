#pragma once
using namespace System;

#include <Core.h>
#include <Scene/Scene.h>
#include <EditorScene.h>
#include <Scene/SceneManager.h>

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
	};
}
