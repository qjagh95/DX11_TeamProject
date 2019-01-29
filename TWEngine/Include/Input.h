#pragma once

#include "Engine.h"
#include "Component/Animation2D.h"

PUN_BEGIN

typedef struct PUN_DLL _tagKeyInfo
{
	string			strName;
	vector<DWORD>	vecKey;
	bool			bPress;
	bool			bPush;
	bool			bUp;
}KeyInfo, *PKeyInfo;

class PUN_DLL CInput
{
private:
	unordered_map<string, PKeyInfo>	m_mapKey;
	PKeyInfo		m_pCreateKey;
	Vector2			m_vMouseClient;
	Vector2			m_vMouseWorld;
	Vector2			m_vMouseGap;
	class CGameObject*	m_pMouse;
	class CTransform*	m_pMouseTr;
	bool			m_bShowCursor;
	class CColliderPoint*	m_pWorldPoint;

public:
	class CGameObject*	GetMouseObj()	const;
	Vector2	GetMouseGap()	const;
	Vector2 GetMouseClient()	const;
	Vector2 GetMouseWorld()	const;
	Vector3 GetMousePos() const;
	void ChangeMouseScene(class CScene* pScene);
	void SetWorldMousePos(Vector3 _vPos);
public:
	bool Init();
	void Update(float fTime);
	bool KeyPress(const string& strKey);
	bool KeyPush(const string& strKey);
	bool KeyUp(const string& strKey);
	void RenderMouse(float fTime);
	void AddMouseCollision();

private:
	PKeyInfo FindKey(const string& strKey);

public:
	// 템플릿 가변인자의 종료조건 함수를 만들어준다.
	template <typename T>
	bool AddKey(const T& value)
	{
		const char* pType = typeid(T).name();

		if (strcmp(pType, "int") == 0 || strcmp(pType, "char") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)value);
		}

		else
		{
			m_pCreateKey->strName = value;

			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}

		return true;
	}

	template <typename T, typename ... Types>
	bool AddKey(const T& value, Types ... Args)
	{
		if (!m_pCreateKey)
		{
			m_pCreateKey = new KeyInfo;
			m_pCreateKey->bPress = false;
			m_pCreateKey->bPush = false;
			m_pCreateKey->bUp = false;
		}

		const char* pType = typeid(T).name();

		if (strcmp(pType, "int") == 0 || strcmp(pType, "char") == 0)
		{
			m_pCreateKey->vecKey.push_back((DWORD)value);
		}

		else
		{
			m_pCreateKey->strName = value;

			m_mapKey.insert(make_pair(m_pCreateKey->strName, m_pCreateKey));
		}

		AddKey(Args...);

		if (m_pCreateKey)
			m_pCreateKey = nullptr;

		return true;
	}

	DECLARE_SINGLE(CInput)
};

PUN_END