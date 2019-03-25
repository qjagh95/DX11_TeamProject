#include "EngineHeader.h"
#include "GUIManager.h"
#include "Core.h"

PUN_USING
SINGLETON_VAR_INIT(GUIManager)
using namespace ImGui;

GUIManager::GUIManager()
{
	m_isShow = true;
	m_bHDR = false;
	m_bBlur = false;
	m_bMotionBlur = false;
}

GUIManager::~GUIManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void GUIManager::CreateImGui(HWND hWnd, ID3D11Device * Device, ID3D11DeviceContext * DeviceConstext)
{
	IMGUI_CHECKVERSION();
	CreateContext();
	StyleColorsClassic();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Device, DeviceConstext);
}

void GUIManager::ImGuiBegin(const char * TitleName)
{
	if (m_isShow == false)
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	NewFrame();

	Begin(TitleName);
}

void GUIManager::ImGuiEnd()
{
	if (m_isShow == false)
		return;

	Debug();

	End();
	 
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
}

void GUIManager::Debug()
{
	if (CCore::GetInst()->m_bEditorMode == true)
		return;

	if (CCore::GetInst()->m_bGuiMode == false)
		return;


	ImGui::Checkbox("HDR", &m_bHDR);
	ImGui::Checkbox("Blur", &m_bBlur);
	ImGui::Checkbox("MotionBlur", &m_bMotionBlur);

}
