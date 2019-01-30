#pragma once
#include "Component/Animation2D.h"

PUN_BEGIN

typedef struct PUN_DLL _tagKeyScale // Key가 꾹눌렸을때(BindAxis 방식) 행위를 하기위해서 Key의 이름과 크기값을 가진 구조체
{
	unsigned char cKey;
	float		  fScale;
}KeyScale , *PKeyScale;

typedef struct PUN_DLL _tagBindAxis // Key가 꾹누르고 있는 입력 방식을 만들 구조체
{
	string strName;
	bool	    bFunctionBind;
	function<void(float, float)> func;
	vector<PKeyScale> KeyList;
	_tagBindAxis() :
		bFunctionBind(false)
	{
	}

	~_tagBindAxis()
	{
		Safe_Delete_VecList(KeyList);
	}
}BindAxis , *PBindAxis;

typedef struct PUN_DLL _tagActionKey
{
	unsigned char cKey;
	bool		  bSKey[SKEY_END];
	bool		  bPress;
	bool		  bPush;
	bool          bRelease;
	_tagActionKey()
	{
		memset(bSKey, 0, sizeof(bool) * SKEY_END);
		bPress = false;
		bPush = false;
		bRelease = false;
	}
}ActionKey , *PActionKey;

typedef struct PUN_DLL _tagBindAction
{
	std::string strName;
	bool		bFunctionBind[AT_END];
	std::function<void(float)> func[AT_END];
	std::vector<PActionKey> KeyList;
	int					  iKeyState;
	_tagBindAction()
	{
		iKeyState = 0;
		memset(bFunctionBind, 0, sizeof(bool) * AT_END);
	}

	~_tagBindAction()
	{
		Safe_Delete_VecList(KeyList);
	}
}BindAction , *PBindAction;

class PUN_DLL CInput
{
private:
	IDirectInput8*					m_pInput;
	IDirectInputDevice8*			m_pKeyboard;
	unsigned char					m_cKey[256];
	bool							m_bPress[256];
	bool							m_bPush[256];
	bool							m_bRelease[256];
	std::vector<unsigned char>		m_KeyList;

private:
	std::unordered_map<std::string, PBindAxis>  m_mapAxis;
	std::unordered_map<std::string, PBindAction> m_mapAction;

private:
	Vector2					m_vMouseClient;
	Vector2					m_vMouseWorld;
	Vector2					m_vMouseGap;
	class CGameObject*		m_pMouse;
	class CTransform*		m_pMouseTr;
	bool					m_bShowCursor;
	class CColliderPoint*	m_pWorldPoint;

public:
	class CGameObject*	GetMouseObj()	const;
	class CGameObject * GetMouseObjNonCount() const;
	Vector2	GetMouseGap()	const;
	Vector2 GetMouseClient()	const;
	Vector2 GetMouseWorld()	const;
	Vector3 GetMousePos() const;
	void ChangeMouseScene(class CScene* pScene);
	void SetWorldMousePos(Vector3 _vPos);
public:
	bool Init();
	void Update(float fTime);	
	void RenderMouse(float fTime);
	void AddMouseCollision();
private:
	bool ReadKeyBoard();

public:
	PBindAxis FindAxis(const std::string& _strName);
	PBindAction FindAction(const std::string& _strName);

public:
	void AddKeyScale(const std::string& _strName, unsigned char _cKey, float _fScale)
	{
		PBindAxis pBind = FindAxis(_strName);
		 
		if (!pBind)
		{
			pBind = new _tagBindAxis;
			m_mapAxis.insert(std::make_pair(_strName, pBind));
		}

		PKeyScale pKeyScale = new KeyScale;
		pKeyScale->fScale = _fScale;
		pKeyScale->cKey = _cKey;

		pBind->KeyList.push_back(pKeyScale);	

		vector<unsigned char>::iterator	iter;
		vector<unsigned char>::iterator	iterEnd = m_KeyList.end();

		bool	bFind = false;
		for (iter = m_KeyList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == _cKey)
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
			m_KeyList.push_back(_cKey);
	}

	PBindAxis BindAxis(const std::string& _strName, void(*pfunc)(float, float))
	{
		PBindAxis pBind = FindAxis(_strName);

		if (!pBind)
		{
			pBind = new _tagBindAxis;

			m_mapAxis.insert(std::make_pair(_strName, pBind));
		}

		pBind->strName = _strName;
		pBind->bFunctionBind = true;
		pBind->func = bind(pfunc, placeholders::_1, placeholders::_2);

		return pBind;
	}

	template <typename T>
	PBindAxis BindAxis(const string& strName, T* pObj, void(T::*pFunc)(float, float))
	{
		PBindAxis	pBind = FindAxis(strName);

		if (!pBind)
		{
			pBind = new _tagBindAxis;

			m_mapAxis.insert(make_pair(strName, pBind));
		}

		pBind->strName = strName;
		pBind->bFunctionBind = true;
		pBind->func = bind(pFunc, pObj, placeholders::_1, placeholders::_2);

		return pBind;
	}


	public:
		void AddKeyAction(const string& strName, unsigned char cKey,
			bool* pSKey = nullptr)
		{
			PBindAction	pBind = FindAction(strName);

			if (!pBind)
			{
				pBind = new _tagBindAction;
				m_mapAction.insert(make_pair(strName, pBind));
			}

			PActionKey	pKeyAction = new ActionKey;

			pKeyAction->cKey = cKey;

			if (pSKey)
				memcpy(pKeyAction->bSKey, pSKey, sizeof(bool) * SKEY_END);

			pBind->KeyList.push_back(pKeyAction);

			vector<unsigned char>::iterator	iter;
			vector<unsigned char>::iterator	iterEnd = m_KeyList.end();

			bool	bFind = false;
			for (iter = m_KeyList.begin(); iter != iterEnd; ++iter)
			{
				if (*iter == cKey)
				{
					bFind = true;
					break;
				}
			}

			if (!bFind)
				m_KeyList.push_back(cKey);
		}

		PBindAction BindAction(const string& strName, KEY_STATE eKeyState,
			void(*pFunc)(float))
		{
			PBindAction	pBind = FindAction(strName);

			if (!pBind)
			{
				pBind = new _tagBindAction;

				m_mapAction.insert(make_pair(strName, pBind));
			}

			ACTION_KEY_FUNCTION_TYPE	eType;

			switch (eKeyState)
			{
			case KEY_PRESS:
				eType = AT_PRESS;
				break;
			case KEY_PUSH:
				eType = AT_PUSH;
				break;
			case KEY_RELEASE:
				eType = AT_RELEASE;
				break;
			}

			pBind->strName = strName;
			pBind->bFunctionBind[eType] = true;
			pBind->func[eType] = bind(pFunc, placeholders::_1);
			pBind->iKeyState |= eKeyState;

			return pBind;
		}

		template <typename T>
		PBindAction BindAction(const string& strName, KEY_STATE eKeyState,
			T* pObj, void(T::*pFunc)(float))
		{
			PBindAction	pBind = FindAction(strName);

			if (!pBind)
			{
				pBind = new _tagBindAction;

				m_mapAction.insert(make_pair(strName, pBind));
			}

			ACTION_KEY_FUNCTION_TYPE	eType;

			switch (eKeyState)
			{
			case KEY_PRESS:
				eType = AT_PRESS;
				break;
			case KEY_PUSH:
				eType = AT_PUSH;
				break;
			case KEY_RELEASE:
				eType = AT_RELEASE;
				break;
			}

			pBind->strName = strName;
			pBind->bFunctionBind[eType] = true;
			pBind->iKeyState |= eKeyState;
			pBind->func[eType] = bind(pFunc, pObj, placeholders::_1);

			return pBind;
		}

	DECLARE_SINGLE(CInput)
};

PUN_END