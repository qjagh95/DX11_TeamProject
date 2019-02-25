#pragma once
PUN_BEGIN

class PUN_DLL CSceneManager
{
private:
	class CScene*	m_pScene;
	class CScene*	m_pNextScene;
	class CStage2D*	m_pStage;
	bool			m_bChange;

public:
	class CScene* GetScene()	const;
	class CScene * GetSceneNonCount() const;
	class CScene* GetNextScene()	const;
	class CScene * GetNextSceneNonCount() const;

public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void Render(float fTime);

private:
	int ChangeScene();

public:
	void AddLayer(const string& strTag, int iZOrder, bool bCurrent = true);
	void ChangeLayerZOrder(const string& strTag, int iZOrder, bool bCurrent = true);
	class CLayer* FindLayer(const string& strTag, bool bCurrent = true);
	class CGameObject* FindObject(const string& strTag);
	void CreateNextScene(bool bChange = true , const std::string& _strTag = "");
	void ChangeEnable();

public:
	template <typename T>
	bool AddSceneComponent(const string& strTag, bool bCurrent = true)
	{
		if (bCurrent)
			return m_pScene->AddSceneComponent<T>(strTag);

		return m_pNextScene->AddSceneComponent<T>(strTag);
	}

	DECLARE_SINGLE(CSceneManager)
};

PUN_END