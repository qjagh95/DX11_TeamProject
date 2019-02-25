#pragma once

PUN_BEGIN

class CTimer;
class PUN_DLL CCore
{
private:
	static bool	m_bLoop;
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Resolution	m_tRS;
	float		m_fClearColor[4];
	float		m_fTimeScale;
	GAME_MODE	m_eGameMode;
	CTimer*		m_pTimer;

public:
	HWND GetWindowHandle()	const;
	HINSTANCE GetWindowInstance()	const;

public:
	void SetClearColor(unsigned char r,
		unsigned char g, unsigned char b,
		unsigned char a);
	void SetGameMode(GAME_MODE eMode);

public:
	bool Init(HINSTANCE hInst, unsigned int iWidth,
		unsigned int iHeight, const TCHAR* pTitle,
		const TCHAR* pClass, int iIconID, int iSmallIconID,
		bool bWindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int iWidth,
		unsigned int iHeight, bool bWindowMode = true);
	int Run();
	void Logic();

	bool EditInit(HWND hWnd, unsigned int iWidth,
		unsigned int iHeight, bool bWindowMode = true);
	void EditLogic();
	void EditDelete();

private:
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void Render(float fTime);

	void EditRender(float fTime);

private:
	void Register(const TCHAR* pClass, int iIconID, int iSmallIconID);
	void CreateWnd(const TCHAR* pTitle, const TCHAR* pClass);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	DECLARE_SINGLE(CCore)
};

PUN_END