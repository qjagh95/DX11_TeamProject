#pragma once

PUN_BEGIN

struct PUN_DLL KeyInfo
{
	string KeyName;
	vector<__int64> vecKey;
	bool KeyPress;
	bool KeyPush;
	bool KeyRelease;
};

class PUN_DLL CInput
{
private:
	vector<unsigned char>	m_KeyList;
	short m_sWheel;

private:
	bool m_isPicking;

private:
	Vector2					m_vMouseClient;
	Vector2					m_vMouseWorld;
	Vector2					m_vMouseGap;
	Vector2					m_vMouse3DClient;
	Vector2					m_vMouse3DWorld;
	Vector2					m_vMouse3DGap;
	class CGameObject*		m_pMouse;
	class CTransform*		m_pMouseTr;
	bool					m_bShowCursor;
	class CCollider*		m_pWorldPoint;
	unordered_map<string, KeyInfo*> m_KeyMap;
	KeyInfo* m_NewKey;
	bool m_bFocus;
	bool m_bRenderMouse;
	////////////////////Navi///////////////////////////
	int	m_iSelectNavIndex;
	int	m_iBrushXRange;
	int m_iBrushZRange;
	Vector3	m_vSelectNaviCellCenter;
public:
	void SetSelectNavIndex(int _idx) { m_iSelectNavIndex = _idx; }
	int GetiSelectNavIndex() const { return m_iSelectNavIndex; }
	void SetSelectCellCenter(Vector3 _vCenter) { m_vSelectNaviCellCenter = _vCenter; }
	void SetXBrushSize(int _fSize) { m_iBrushXRange = _fSize; }
	int GetXBrushSize() const { return m_iBrushXRange; }
	void SetZBrushSize(int _fSize) { m_iBrushZRange = _fSize; }
	int GetZBrushSize() const { return m_iBrushZRange; }

	void SelectNaviBoxRender(float fTime);

public:
	RayInfo MouseRayInfo() const;
	class CGameObject*	GetMouseObj()	const;
	class CGameObject * GetMouseObjNonCount() const;
	class CTransform* GetMouseTr() const;
	class CTransform* GetMouseTrNonCount() const;
	Vector2	GetMouseGap()	const;
	Vector2 GetMouseClient()	const;
	Vector2 GetMouseWorld()	const;
	Vector3 GetMousePos() const;
	Vector2 GetMouse3DGap()	const;
	Vector2 GetMouse3DClient()	const;
	Vector2 GetMouse3DWorld()	const;
	void ChangeMouseScene(class CScene* pScene);
	void SetWorldMousePos(Vector3 _vPos);
	void SetWheelDir(short _sWheel);
	void ClearWheel();
	short GetWheelDir() const;
	bool ShowMouse() const;
	void ShowMouse(bool bShow);
	void SetIsPickingFalse();
	bool GetIsPicking();

public:
	bool Init();
	void Update(float fTime);
	bool KeyPush(const string& Name);
	bool KeyPress(const string& Name);
	bool KeyRelease(const string& Name);
	bool IsFocused() const;

	template <typename T>
	bool AddKey(const T& value)
	{
		if (m_NewKey == NULLPTR)
		{
			m_NewKey = new KeyInfo();
			m_NewKey->KeyRelease = false;
			m_NewKey->KeyPress = false;
			m_NewKey->KeyPush = false;
		}

		//타입이름 가져옴.
		const char* typeName = typeid(T).name();

		if (strcmp(typeName, "int") == 0 || strcmp(typeName, "char") == 0)
			m_NewKey->vecKey.push_back((__int64)value);
		else
		{
			m_NewKey->KeyName = value;
			m_KeyMap.insert(make_pair(m_NewKey->KeyName, m_NewKey));
		}

		return true;
	}

	template <typename T, typename ... Types>
	bool AddKey(const T& value, Types ... Args)
	{
		if (m_NewKey == NULLPTR)
		{
			m_NewKey = new KeyInfo();
			m_NewKey->KeyPress = false;
			m_NewKey->KeyPush = false;
			m_NewKey->KeyRelease = false;
		}

		//타입이름 가져옴.
		const char* typeName = typeid(T).name();

		if (strcmp(typeName, "int") == 0 || strcmp(typeName, "char") == 0)
			m_NewKey->vecKey.push_back((__int64)value);
		else
		{
			m_NewKey->KeyName = value;
			m_KeyMap.insert(make_pair(m_NewKey->KeyName, m_NewKey));
		}

		AddKey(Args...);

		if (m_NewKey != NULLPTR)
			m_NewKey = NULLPTR;

		return true;
	}
	void RenderMouse(float fTime);
	void AddMouseCollision();

private:
	KeyInfo* FindKey(const string& Name);

public:
	void Hit(CCollider * pSrc, CCollider * pDest, float fTime);

public:
	DECLARE_SINGLE(CInput)
};

PUN_END