#pragma once

#include "Component.h"

PUN_BEGIN

class BinaryWrite;
class BinaryRead;
class PUN_DLL CRenderer :
	public CComponent
{
	friend class CGameObject;

private:
	CRenderer();
	CRenderer(const CRenderer& renderer);
	virtual ~CRenderer();

private:
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;
	ID3D11InputLayout*	m_pLayout;
	string				m_strLayoutKey;
	class CMaterial*	m_pMaterial;
	class CRenderState*	m_pRenderState[RS_END];
	unordered_map<string, PRendererCBuffer>	m_mapCBuffer;
	ComponentCBuffer	m_tComponentCBuffer;
	bool				m_b2DRenderer;
	ID3D11ShaderResourceView**  m_pBoneTex;

public:
	void SetBoneTexture(ID3D11ShaderResourceView** _pBoneTex);

public:
	void Enable2DRenderer();
	void SetMesh(class CMesh* pMesh);
	void SetMesh(const string& strKey);
	void SetMesh(const string& strKey, const TCHAR* pFileName,
		const string& strPathKey = MESH_DATA_PATH);

	void SetMeshFromFullPath(const string& strKey, const TCHAR* pFullPath);
	void SetShader(class CShader* pShader);
	void SetShader(const string& strKey);
	void SetInputLayout(const string& strKey);
	void SetRenderState(const string& strName);
	bool CreateRendererCBuffer(const string& strName, int iSize);
	void UpdateRendererCBuffer(const string& strName, void* pData,
		int iSize);
	void CheckComponent();
	void DeleteComponentCBuffer(CComponent* pComponent);
	void UpdateAnimationType(int iType);

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
	void UpdateTransform();
};

PUN_END