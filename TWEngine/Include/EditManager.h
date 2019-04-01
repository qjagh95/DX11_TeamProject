#pragma once

using namespace std;

PUN_BEGIN

class CScene;
class CAnimation;
class CGameObject;
class PUN_DLL CEditManager
{
private:
	CEditManager();
	~CEditManager();

private:
	static CEditManager* m_pInst;

public:
	static CEditManager* GetInst()
	{
		if (m_pInst == nullptr)
		{
			m_pInst = new CEditManager;
		}
		return m_pInst;
	}
	static void DestroyInst()
	{
		SAFE_DELETE(m_pInst);
	}

private:
	CScene*			m_pScene;
	CGameObject*	 m_pObject;
	class CGizmo*	 m_pXGizmo;
	class CGizmo*	 m_pYGizmo;
	class CGizmo*	 m_pZGizmo;
	CGameObject*	 m_pXGizmoObj;
	CGameObject*	 m_pYGizmoObj;
	CGameObject*	 m_pZGizmoObj;

	class CEditTest* m_pEditTest;
	vector<string>	m_vecstrObjList;
	CAnimation*		m_pAnimation;
	vector<struct _tagBoneKeyFrame*> m_vecDivideFrame;

private:
	enum eTransformType
	{
		TT_SCALE,
		TT_ROTATE,
		TT_POSITION,
		TT_MAX,
	};

public:
	bool Init();

// 레이어
public:
	void GetLayerList(vector<string>* _pVec);

// 오브젝트
public:
	void GetSelectLayerObjList(string _strLayerTag, vector<string>* _pVec);
	void SetActiveObject(const string _strObjectTag, const string _strLayerTag);
	void CreateObject(const string _strObjectTag, const string _strLayerTag);
	void DeleteObject(const string _strObjectTag, const string _strLayerTag);
	void DeleteAllObject();
	void ChangeObjectTag(const string _strObjectTag);
	void ChangeObjectInLayer(const string _strLayerTag);
	void AddComponent(string& _strCompTag);
	void ActiveObjectSetScale(double _dX, double _dY, double _dZ);
	void ActiveObjectSetRotate(double _dX, double _dY, double _dZ);
	void ActiveObjectSetPosition(double _dX, double _dY, double _dZ);
	vector<Vector3> GetWorldTransform(const string _strObjectTag, const string _strLayerTag, int _eType);

// 메시
public:
	void GetMeshNameList(vector<string>* _pVec);
	void SetMesh(const string& _strMeshTag);

// 애니메이션
public:
	void LoadClipFromFullPath(const wstring& _strFullPath);
	void GetClipNameList(vector<string>* _vecstrClipList);
	void SetDivideKeyFrame();
	void DeleteDivideKeyFrame();
	void DeleteClip(const string& _strKey);
	bool ModifyClip(const string& strKey, const string& strChangeKey, int iOption,
					int iStartFrame, int iEndFrame, float fPlayTime);
	bool AddClip(const string& strKey, int iOption,
				 int iStartFrame, int iEndFrame, float fPlayTime);
	void ChangeClip(const string& _strKey);
	void ClipSaveFromFullPath(const string& _strFullPath);
};

PUN_END