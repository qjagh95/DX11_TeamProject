#include "EngineHeader.h"
#include "GUIManager.h"

PUN_USING
SINGLETON_VAR_INIT(GUIManager)
using namespace ImGui;

GUIManager::GUIManager()
{
	m_isShow = true;
}

GUIManager::~GUIManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void GUIManager::CreateImGui(HWND hWnd, ID3D11Device * Device, ID3D11DeviceContext * DeviceConstext)
{
#ifdef _DEBUG
	IMGUI_CHECKVERSION();
	CreateContext();
	StyleColorsClassic();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Device, DeviceConstext);
#endif
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
	static float RotX;
	static float RotY;
	static float RotZ;

	static Vector3 Pos = Vector3(0.0f, 1.0f, 1.0f);

	CScene* getScene = CSceneManager::GetInst()->GetScene();
	CGameObject* getObject = getScene->FindObject("Pyramid");
	CGameObject* GetLightObj = getScene->FindObject("GlobalLight1");

	if (getObject == NULLPTR)
	{
		SAFE_RELEASE(getObject);
		SAFE_RELEASE(getScene);
		return;
	}

	ImGui::Text("Pyramid");

	ImGui::SliderFloat("RotX", &RotX, 0.0f, 10.0f);
	ImGui::SliderFloat("RotY", &RotY, 0.0f, 10.0f);
	ImGui::SliderFloat("RotZ", &RotZ, 0.0f, 10.0f);

	ImGui::SliderFloat3("PyramidPos", (float*)&Pos, 0.0f, 10.0f);

	getObject->GetTransformNoneCount()->RotationX(RotX);
	getObject->GetTransformNoneCount()->RotationY(RotY);
	getObject->GetTransformNoneCount()->RotationZ(RotZ);
	getObject->GetTransformNoneCount()->SetWorldPos(Pos);

	ImGui::BeginTabBar("Pyramid");
	ImGui::EndTabBar();


	static float LightRotX;
	static float LightRotY;
	static float LightRotZ;


	ImGui::Text("GlobalLight1");

	ImGui::SliderFloat("LightRotX", &LightRotX, 0.0f, 10.0f);
	ImGui::SliderFloat("LightRotY", &LightRotY, 0.0f, 10.0f);
	ImGui::SliderFloat("LightRotZ", &LightRotZ, 0.0f, 10.0f);

	static Vector3 LightPos = Vector3(0.0f, 1.0f, 1.0f);
	ImGui::SliderFloat3("GlobalLight1Pos", (float*)&LightPos, 0.0f, 10.0f);

	GetLightObj->GetTransformNoneCount()->RotationX(LightRotX);
	GetLightObj->GetTransformNoneCount()->RotationY(LightRotY);
	GetLightObj->GetTransformNoneCount()->RotationZ(LightRotZ);
	GetLightObj->GetTransformNoneCount()->SetWorldPos(LightPos);
	ImGui::BeginTabBar("GlobalLight1");
	ImGui::EndTabBar();

	SAFE_RELEASE(GetLightObj);
	SAFE_RELEASE(getObject);
	SAFE_RELEASE(getScene);
}
