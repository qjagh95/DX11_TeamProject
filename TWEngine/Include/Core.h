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
	bool		m_bHeader;
	static unordered_map<string, vector<float>*> m_ManagerMap;
	static time_t m_iTime;
	static tm* m_pDateInfo;

	vector<float>* m_vecInput;
	vector<float>* m_vecUpdate;
	vector<float>* m_vecLateUpdate;
	vector<float>* m_vecCollsion;
	vector<float>* m_vecRender;
	FileStream m_LogText;

public:
	HWND GetWindowHandle()	const;
	HINSTANCE GetWindowInstance()	const;
	bool m_bEditorMode;
	bool m_bGuiMode;
	static int m_iVecIndex;
	static void WriteLogText(ofstream& Stream, float Compute);

public:
	void SetClearColor(unsigned char r,
		unsigned char g, unsigned char b,
		unsigned char a);
	void SetGameMode(GAME_MODE eMode);

private:
	void UpdateSoundManager(float fTime); //에디터에서 사운드매니저를 불러올 수 없으므로 따로 처리했음

public:
	bool Init(HINSTANCE hInst, unsigned int iWidth,
		unsigned int iHeight, const TCHAR* pTitle,
		const TCHAR* pClass, int iIconID, int iSmallIconID,
		bool bWindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int iWidth,
		unsigned int iHeight, bool bWindowMode = true);
	int Run();
	void Logic();

	bool EditInit(HWND hWnd, HINSTANCE hInstance, unsigned int iWidth,
		unsigned int iHeight, bool bWindowMode = true);
	void EditLogic();
	void EditDelete();
	void EditCreateObject(const std::string& _strTag);
	void AddManagerVector(const string& KeyName);
	vector<float>* FindManagerMap(const string& KeyName) const;

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
	void GUIOnOff(float fTime);
	void ShaderOptionOnOff(float fTime);

	DECLARE_SINGLE(CCore)
};

PUN_END