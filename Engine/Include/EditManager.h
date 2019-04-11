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
	class CArm*		 m_pArm;
	CGameObject*	 m_pFreeCamObj;
	class CEditTest* m_pEditTest;
	vector<string>	m_vecstrObjList;
	CAnimation*		m_pAnimation;
	vector<struct _tagBoneKeyFrame*> m_vecDivideFrame;

	bool			m_bNaviEditorMode;
public:
	void SetFreeCamObj(CGameObject* _pObj);
	void SetArm(class CArm* _pArm);
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
	void Render(float _fTime);
	// 레이어
public:
	void GetLayerList(vector<string>* _pVec);

	// 오브젝트
public:
	bool IsNaviEditorMode() const { return m_bNaviEditorMode; }

	void GetSelectLayerObjList(string _strLayerTag, vector<string>* _pVec);
	void SetActiveObject(const string _strObjectTag, const string _strLayerTag);
	void SetActiveObject(class CGameObject* _pObject);
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
	void SetMouseWheel(short _sMouseWheel);
	// 메시
public:
	void GetMeshNameList(vector<string>* _pVec);
	void SetMesh(const string& _strMeshTag);
	void ChangeCameraTarget(bool _bTarget);
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
	///////////////////////////Light////////////////////////////////////////
public:
	void AddLightComponent();

	void SetLightType(LIGHT_TYPE eType);
	void SetLightDiffuse(const Vector4& _vDiffuse);
	void SetLightAmbient(const Vector4& _vAmbient);
	void SetLightSpcular(const Vector4& _vSpcular);

	void SetLightSpcularR(float r);
	void SetLightSpcularG(float g);
	void SetLightSpcularB(float b);
	void SetLightSpcularW(float w);

	void SetLightDirX(float x);
	void SetLightDirY(float y);
	void SetLightDirZ(float z);
	void SetLightRange(float Range);
};

PUN_END