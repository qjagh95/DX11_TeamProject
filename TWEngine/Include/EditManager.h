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
	CScene*				m_pScene;
	class CGizmo*		m_pXGizmo;
	class CGizmo*		m_pYGizmo;
	class CGizmo*		m_pZGizmo;
	CGameObject*		m_pObject;
	class CCollider*    m_pSelectCollider;
	CGameObject*		m_pXGizmoObj;
	CGameObject*		m_pYGizmoObj;
	CGameObject*		m_pZGizmoObj;
	CGameObject*		m_pFreeCamObj;
	CGameObject*		m_pNavObject;
	class CArm*			m_pArm;
	class CEditTest*	m_pEditTest;
	class CLandScape*	m_LandScape;
	CAnimation*			m_pAnimation;
	class CDecal*	    m_pDecal;
	bool				m_isGizmoClick;
	bool				m_bNaviEditorMode;
	bool				m_bSelectNaviMove;
	vector<string>		m_vecstrObjList;
	vector<struct _tagBoneKeyFrame*> m_vecDivideFrame;
	std::vector<std::string> m_vecObjCollTag;
public:
	void SetFreeCamObj(CGameObject* _pObj);
	void SetArm(class CArm* _pArm);
public:
	void GetObjFromCollTag();
	const std::vector<std::string>* GetVecObjCollTag() const
	{
		return &m_vecObjCollTag;
	}

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

	void PrivateEditObjSettingLayer();

	// 오브젝트
public:
	// Navigation
	bool IsNaviEditorMode() const { return m_bNaviEditorMode; }
	void SetNaviEditorMode(bool _bMode){ m_bNaviEditorMode = _bMode;}
	void SetBrushSize(int _fSize);
	void SetBrushXSize(int _fSize);
	void SetBrushZSize(int _fSize);

	void DeleteNavigationMesh();
	void SetNaviMove(bool _bEnable) { m_bSelectNaviMove = _bEnable; }
	bool GetNaviMove() const { return m_bSelectNaviMove; }
	///////////////////////////////////////////////////////////////////////
	void GetSelectLayerObjList(string _strLayerTag, vector<string>* _pVec);
	void SetActiveObject(const string _strObjectTag, const string _strLayerTag);
	void SetActiveObject(class CGameObject* _pObject);
	vector<string> GetActiveObjectInfo();
	void CreateObject(const string _strObjectTag, const string _strLayerTag, bool _isChild = false);
	void DeleteObject(const string _strObjectTag, const string _strLayerTag);
	void DeleteAllObject();
	void ChangeObjectTag(const string _strObjectTag);
	void ChangeObjectInLayer(const string _strLayerTag);
	void AddComponent(string& _strCompTag);
	void AddColliderOBB(float dCenterX, float dCenterY, float dCenterZ, float dLengthX, float dLengthY, float dLengthZ,
		int iColliderID, const std::string& _strTag, const std::string& _strCollTypeTag, const std::string& _strExceptTypeTag , bool _bMeshScale);
	void AddColliderSphere(float dCenterX, float dCenterY, float dCenterZ, float fRadius,
		int iColliderID, const std::string& _strTag, const std::string& _strCollTypeTag, const std::string& _strExceptTypeTag, bool _bMeshScale);
	void SetLocalScale(double _dX, double _dY, double _dZ);
	void SetLocalRotate(double _dX, double _dY, double _dZ);
	void SetLocalPosition(double _dX, double _dY, double _dZ);
	void SetWorldScale(double _dX, double _dY, double _dZ);
	void SetWorldRotate(double _dX, double _dY, double _dZ);
	void SetWorldPosition(double _dX, double _dY, double _dZ);
	void SetWorldPivot(double _dX, double _dY, double _dZ);
	bool FindRenderComponent();
	bool FindLightComponent();
	void SetGizmoClick(bool _isFlag);
	bool IsGizmoCheckClick();
	string GetMeshName(string _strTag);
	void AddChild(const string _strObjectTag, const string _strLayerTag);
	string GetParentTag();
	vector<Vector3> GetLocalTransform(const string _strObjectTag, const string _strLayerTag, int _eType);
	vector<Vector3> GetWorldTransform(const string _strObjectTag, const string _strLayerTag, int _eType);
	vector<Vector3> GetWorldPivot(string _strObjectTag, string _strLayerTag);
	Vector3 GetChildWorldPosition(const string _strParentTag, const string _strLayerTag);
	void SetMouseWheel(short _sMouseWheel);
	void SetGizmoEnable(bool _bEnable);
	void SetPickingColliderEnable(bool _bEnable);
	void SetColliderEnable(bool _bEnable);

	bool CreateLandScape(int _iX , int _iZ);
	int GetNaviSelectIndex() const;
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

	bool SaveNavFile(const string& FullPath);
	bool LoadNavFile(const string& FullPath);

	int GetLightType();
	float GetLightRange();
	vector<Vector4> GetSpecular();
	void CreateDefaultGlobalLight();
	vector<string> GetGlobalLightList();

	CLandScape * GetLandScape() const { return m_LandScape; }
	/////////////////////////////Collider/////////////////////////
	bool FindActiveCollider(const std::string& _strTag);
	Vector3 GetOBBLength();
	float GetSphereRadius();
	Vector3 GetOBBRelativeCenter();
	Vector3 GetSphereCenter();
	int		GetCollType() const;
	void SetSphereColliderInfo(float dCenterX, float dCenterY, float dCenterZ, float fRadius , bool _bMeshScale);
	void SetOBB3DColliderInfo(float dCenterX, float dCenterY, float dCenterZ, float dLengthX, float dLengthY, float dLengthZ, bool _bMeshScale);
	//////////////////SetStageSection///////////////////////////////////////
	void SetStageSection(STAGE_SECTION_TYPE _eType);

	/////////////////Decal/////////////////////////////////////////////////
	void AddDecalComponent();
	void AddDecalDiffuseTex(const std::string& _strTag , const std::wstring& _strFileName);
	void AddDecalNormalTex(const std::string& _strTag, const std::wstring& _strFileName);
	void AddDecalSpecularTex(const std::string& _strTag, const std::wstring& _strFileName);
	const string& GetDecalDiffuseTag();
	const string& GetDecalNormalTag();
	const string& GetDecalSpecularTag();
	private:
	std::string m_strDecalDiffuseTag;
	std::string m_strDecalNormalTag;
	std::string m_strDecalSpecularTag;
};

PUN_END