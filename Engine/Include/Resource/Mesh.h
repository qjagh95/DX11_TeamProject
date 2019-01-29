#pragma once
#include "../Ref.h"

PUN_BEGIN

typedef struct PUN_DLL _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	void*			pData;
	int				iCount;
	int				iSize;
	D3D11_USAGE		eUsage;

	_tagVertexBuffer() :
		pBuffer(nullptr),
		pData(nullptr),
		iCount(0),
		iSize(0),
		eUsage(D3D11_USAGE_DEFAULT)
	{
	}
}VertexBuffer, *PVertexBuffer;

typedef struct PUN_DLL _tagIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	void*			pData;
	int				iCount;
	int				iSize;
	D3D11_USAGE		eUsage;
	DXGI_FORMAT		eFmt;

	_tagIndexBuffer() :
		pBuffer(nullptr),
		pData(nullptr),
		iCount(0),
		iSize(0),
		eUsage(D3D11_USAGE_DEFAULT),
		eFmt(DXGI_FORMAT_R16_UINT)
	{
	}
}IndexBuffer, *PIndexBuffer;

typedef struct PUN_DLL _tagMeshContainer
{
	VertexBuffer				tVB;
	vector<IndexBuffer>			vecIB;
	D3D11_PRIMITIVE_TOPOLOGY	ePrimitive;
}MeshContainer, *PMeshContainer;

class PUN_DLL CMesh :
	public CRef
{
	friend class CResourcesManager;

private:
	CMesh();
	~CMesh();

private:
	vector<PMeshContainer>	m_vecMeshContainer;
	string					m_strShaderKey;
	string					m_strInputLayoutKey;
	Vector3					m_vCenter;
	float					m_fRadius;
	Vector3					m_vMin;
	Vector3					m_vMax;
	Vector3					m_vLength;

public:
	Vector3 GetCenter()	const;
	Vector3	GetMin()	const;
	Vector3	GetMax()	const;
	Vector3	GetLength()	const;
	float	GetRadius()	const;

public:
	string GetShaderKey()	const;
	string GetInputLayoutKey()	const;
	size_t GetContainCount()	const;
	size_t GetSubsetCount(int iContainer)	const;

public:
	bool CreateMesh(const string& strName, const string& strShaderKey,
		const string& strLayoutKey, void* pVtx, int iVtxCount, int iVtxSize,
		D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive,
		void* pIdx = nullptr, int iIdxCount = 0, int iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R16_UINT);
	void Render();
	void Render(int iContainer, int iSubset);
	void UpdateVertexBuffer(void* pData, int iContainer = 0);

private:
	bool CreateVertexBuffer(void* pData, int iCount, int iSize, D3D11_USAGE eUsage);
	bool CreateIndexBuffer(void* pData, int iCount, int iSize, D3D11_USAGE eUsage, DXGI_FORMAT eFmt);
};

PUN_END