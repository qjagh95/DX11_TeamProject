#pragma once

#include "Component.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
class CAnimation;
class PUN_DLL CRenderer :
	public CComponent
{
	friend class CGameObject;

private:
	CRenderer();
	CRenderer(const CRenderer& renderer);
	virtual ~CRenderer();

private:
	bool				m_b2DRenderer;
	bool				m_bDecalEnable;
	bool				m_bDontRenderMat;
	bool				m_bAlphaEnable;
	string				m_strLayoutKey;
	string				m_BoneName;
	Matrix				m_matBone;
	Vector3				m_BoneRot;

	bool				m_isRotFirstCheck;
	Vector3				m_RotFirstCheck;
	Vector3				m_RotLastCheck;

	bool				m_isPosFirstCheck;
	Vector3				m_PosFirstCheck;
	Vector3				m_PosLastCheck;

	CAnimation*			m_pAnimation;
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;
	class CMaterial*	m_pMaterial;
	class CRenderState*	m_pRenderState[RS_END];
	ID3D11InputLayout*	m_pLayout;
	ID3D11ShaderResourceView**  m_pBoneTex;

	TransformCBuffer	m_tTransformCBuffer;
	ComponentCBuffer	m_tComponentCBuffer;

	unordered_map<string, PRendererCBuffer>	m_mapCBuffer;

public:
	void SetBoneTexture(ID3D11ShaderResourceView** _pBoneTex);

public:
	bool IsShadowRender() const;
	void DontRenderMat(bool bSwitch);
	void Enable2DRenderer();
	void SetMesh(class CMesh* pMesh);
	void SetMesh(const string& strKey);
	void SetMesh(const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = MESH_DATA_PATH);
	void RenderShadow(float fTime);
	void SetAnimation(CAnimation* animation) { m_pAnimation = animation;}

	void SetRotCheckStart(bool Value) { m_isRotFirstCheck = Value; }
	void SetPosCheckStart(bool Value) { m_isPosFirstCheck = Value; }
	Vector3 GetDiffrentRot() const;
	float GetDiffrentPosX() const;
	float GetDiffrentPosY() const;
	float GetDiffrentPosZ() const;
	Vector3 GetDiffrentPos() const;

	class CMesh* GetMesh() const;
	Vector3 GetMeshLength() const;
	Vector3 GetModelMoveDist();

	void SetMeshFromFullPath(const string& strKey, const TCHAR* pFullPath);
	void SetShader(class CShader* pShader);
	void SetShader(const string& strKey);
	void SetInputLayout(const string& strKey);
	void SetRenderState(const string& strName);
	void SetDecalEnable(bool bEnable);
	void SetAlphaEnable(bool bEnable);
	bool CreateRendererCBuffer(const string& strName, int iSize);
	void UpdateRendererCBuffer(const string& strName, void* pData, int iSize);
	void CheckComponent();
	void DeleteComponentCBuffer(CComponent* pComponent);
	void UpdateAnimationType(int iType);
	void SetSelectBoneName(const string& BoneName) { m_BoneName = BoneName; }
	string GetSelectBoneName() const { return m_BoneName; }
	Matrix GetSelectBoneMatrix() const { return m_matBone; }
	Vector3 GetBoneRot() const { return m_BoneRot; }

private:
	PRendererCBuffer FindRendererCBuffer(const string& strName);

public:
	void Save(BinaryWrite* _pInstBW);
	void Load(BinaryRead*  _pInstBR);

public:
	virtual void AfterClone();
	virtual bool Init();
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CRenderer* Clone();

private:
	void RenderNaviEditorMode(float fTime);
	void UpdateTransform();
	void UpdateShadowTransform();
	void SetBumpScale(float fScale, float fTime);
	void SetBumpScaleNone();
};

PUN_END