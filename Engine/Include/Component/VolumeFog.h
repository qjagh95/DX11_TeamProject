#pragma once
#include "Component.h"

enum FOG_PASS
{
	FOG_FIRST,
	FOG_SECOND
};

PUN_BEGIN

class CMesh;
class CShader;
class CRenderState;
class CSampler;

class PUN_DLL CVolumeFog :
	public CComponent
{
public:
	CVolumeFog();
	CVolumeFog(const CVolumeFog& pCom);
	~CVolumeFog();

private:
	CMesh*				m_pMesh;
	ID3D11InputLayout*	m_pLayOut;

	int					m_iPass;
	float				m_fDensityLevel;
	FogCBuffer			m_tFogCBuffer;

public:
	void SetMesh(const string& strMeshName);
	void SetFogColor(const Vector4& vColor);
	void SetDensity(float fDensity);

public:
	virtual bool Init();
	virtual void Render(float fTime);
	virtual CVolumeFog* Clone();

	void UpdateData();
};

PUN_END