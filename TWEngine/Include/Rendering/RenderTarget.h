#pragma once

PUN_BEGIN

class PUN_DLL CRenderTarget
{
	friend class CRenderManager;

private:
	CRenderTarget();
	~CRenderTarget();

private:
	ID3D11Texture2D*			m_pTargetTex;
	ID3D11ShaderResourceView*	m_pTargetSRV;
	ID3D11RenderTargetView*		m_pTargetView;
	ID3D11Texture2D*			m_pDepthTex;
	ID3D11DepthStencilView*		m_pDepthView;

	ID3D11RenderTargetView*		m_pOldTargetView;
	ID3D11DepthStencilView*		m_pOldDepthView;
	float						m_fClearColor[4];

	class CShader*				m_pShader;
	class CShader*				m_pFullScreenShader;
	class CMesh*				m_pMesh;
	class CSampler*				m_pSampler;
	ID3D11InputLayout*			m_pLayout;
	class CDepthState*			m_pDepthState;
	Vector3						m_vPos;
	Vector3						m_vScale;
	bool						m_bDrawDebug;

public:
	ID3D11RenderTargetView* GetRenderTargetView()	const;
	ID3D11DepthStencilView* GetDepthView()	const;
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pTargetSRV; }

public:
	bool CreateRenderTarget(DXGI_FORMAT eTargetFmt,
		const Vector3& vPos, const Vector3& vScale,
		DXGI_FORMAT eDepthFmt = DXGI_FORMAT_UNKNOWN);
	void SetClearColor(float fClearColor[4]);
	void SetClearColor(const Vector4& vColor);
	void ONDrawDebug(bool bDraw);
	void ClearTarget();
	void SetTarget();
	void ResetTarget();
	void Render(float fTime);
	void RenderFullScreen();
	void SetShader(int iRegister);
	void ResetShader(int _iRegister);
	void Save(const TCHAR* _pFileName, const std::string& _strPathKey);
};

PUN_END