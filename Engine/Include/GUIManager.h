#pragma once

PUN_BEGIN

class PUN_DLL GUIManager
{
public:
	void CreateImGui(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceConstext);
	void ImGuiBegin(const char* TitleName);
	void ImGuiEnd();
	void Debug();

	bool m_isShow;
	bool m_bHDR;
	bool m_bBlur;
	bool m_bMotionBlur;

	bool m_bShaderOption;

public:
	CLASS_IN_SINGLE(GUIManager)
};

PUN_END