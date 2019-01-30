#include "EngineHeader.h"
#include "Mesh.h"
#include "../Device.h"

PUN_USING

CMesh::CMesh()
{
}


CMesh::~CMesh()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		SAFE_RELEASE(m_vecMeshContainer[i]->tVB.pBuffer);
		SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->tVB.pData);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIB[j].pBuffer);
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIB[j].pData);
		}

		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();
}

Vector3 CMesh::GetCenter() const
{
	return m_vCenter;
}

Vector3 CMesh::GetMin() const
{
	return m_vMin;
}

Vector3 CMesh::GetMax() const
{
	return m_vMax;
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

float CMesh::GetRadius() const
{
	return m_fRadius;
}

string CMesh::GetShaderKey() const
{
	return m_strShaderKey;
}

string CMesh::GetInputLayoutKey() const
{
	return m_strInputLayoutKey;
}

size_t CMesh::GetContainCount() const
{
	return m_vecMeshContainer.size();
}

size_t CMesh::GetSubsetCount(int iContainer) const
{
	return m_vecMeshContainer[iContainer]->vecIB.size();
}

bool CMesh::CreateMesh(const string & strName, const string & strShaderKey, const string & strLayoutKey,
	void * pVtx, int iVtxCount, int iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pIdx, int iIdxCount, int iIdxSize,
	D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt)
{
	SetTag(strName);
	m_strShaderKey = strShaderKey;
	m_strInputLayoutKey = strLayoutKey;

	PMeshContainer	pContainer = new MeshContainer;

	pContainer->ePrimitive = ePrimitive;

	m_vecMeshContainer.push_back(pContainer);

	if (!CreateVertexBuffer(pVtx, iVtxCount, iVtxSize, eVtxUsage))
		return false;

	if (pIdx)
	{
		if (!CreateIndexBuffer(pIdx, iIdxCount, iIdxSize, eIdxUsage, eFmt))
			return false;
	}

	return true;
}

void CMesh::Render()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[i]->ePrimitive);

		UINT	iStride = m_vecMeshContainer[i]->tVB.iSize;
		UINT	iOffset = 0;
		CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->tVB.pBuffer, &iStride, &iOffset);

		if (m_vecMeshContainer[i]->vecIB.empty())
		{
			CONTEXT->Draw(m_vecMeshContainer[i]->tVB.iCount, 0);
		}

		else
		{
			for (size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j)
			{
				CONTEXT->IASetIndexBuffer(m_vecMeshContainer[i]->vecIB[j].pBuffer,
					m_vecMeshContainer[i]->vecIB[j].eFmt, 0);
				CONTEXT->DrawIndexed(m_vecMeshContainer[i]->vecIB[j].iCount, 0, 0);
			}
		}

	}
}

void CMesh::Render(int iContainer, int iSubset)
{
	CONTEXT->IASetPrimitiveTopology(m_vecMeshContainer[iContainer]->ePrimitive);

	UINT	iStride = m_vecMeshContainer[iContainer]->tVB.iSize;
	UINT	iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &m_vecMeshContainer[iContainer]->tVB.pBuffer, &iStride, &iOffset);

	if (m_vecMeshContainer[iContainer]->vecIB.empty())
	{
		CONTEXT->Draw(m_vecMeshContainer[iContainer]->tVB.iCount, 0);
	}

	else
	{
		CONTEXT->IASetIndexBuffer(m_vecMeshContainer[iContainer]->vecIB[iSubset].pBuffer,
			m_vecMeshContainer[iContainer]->vecIB[iSubset].eFmt, 0);
		CONTEXT->DrawIndexed(m_vecMeshContainer[iContainer]->vecIB[iSubset].iCount, 0, 0);
	}
}

void CMesh::UpdateVertexBuffer(void * pData, int iContainer)
{
	if (iContainer < 0 || iContainer >= m_vecMeshContainer.size())
		assert(false);

	PMeshContainer	pContainer = m_vecMeshContainer[iContainer];

	memcpy(pContainer->tVB.pData, pData,
		pContainer->tVB.iSize * pContainer->tVB.iCount);

	switch (pContainer->tVB.eUsage)
	{
	case D3D11_USAGE_DEFAULT:
		CONTEXT->UpdateSubresource(pContainer->tVB.pBuffer,
			0, nullptr, pData, 0, 0);
		break;
	case D3D11_USAGE_DYNAMIC:
	{
		

		D3D11_MAPPED_SUBRESOURCE	tMap;
		CONTEXT->Map(pContainer->tVB.pBuffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &tMap);

		memcpy(tMap.pData, pData, pContainer->tVB.iSize * pContainer->tVB.iCount);

		CONTEXT->Unmap(pContainer->tVB.pBuffer, 0);

		
	}
	break;
	}
}

bool CMesh::CreateVertexBuffer(void * pData, int iCount, int iSize, D3D11_USAGE eUsage)
{
	PMeshContainer	pContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	pContainer->tVB.iCount = iCount;
	pContainer->tVB.iSize = iSize;
	pContainer->tVB.eUsage = eUsage;
	pContainer->tVB.pData = new char[iCount * iSize];

	memcpy(pContainer->tVB.pData, pData, iCount * iSize);

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iCount * iSize;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = eUsage;

	if (eUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tResource = {};
	tResource.pSysMem = pContainer->tVB.pData;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&tDesc, &tResource, &pContainer->tVB.pBuffer)))
		return false;

	char*	pVertices = (char*)pData;
	Vector3	vPos;
	memcpy(&vPos, pVertices, sizeof(Vector3));

	m_vMin = vPos;
	m_vMax = vPos;

	for (int i = 1; i < iCount; ++i)
	{
		memcpy(&vPos, pVertices + iSize * i, sizeof(Vector3));
		if (m_vMin.x > vPos.x)
			m_vMin.x = vPos.x;

		if (m_vMin.y > vPos.y)
			m_vMin.y = vPos.y;

		if (m_vMin.z > vPos.z)
			m_vMin.z = vPos.z;

		if (m_vMax.x < vPos.x)
			m_vMax.x = vPos.x;

		if (m_vMax.y < vPos.y)
			m_vMax.y = vPos.y;

		if (m_vMax.z < vPos.z)
			m_vMax.z = vPos.z;
	}

	m_vCenter = (m_vMin + m_vMax) / 2.f;
	m_vLength = m_vMax - m_vMin;
	m_fRadius = m_vLength.Length() / 2.f;

	return true;
}

bool CMesh::CreateIndexBuffer(void * pData, int iCount, int iSize, D3D11_USAGE eUsage, DXGI_FORMAT eFmt)
{
	PMeshContainer	pContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	IndexBuffer	tIB;

	tIB.iCount = iCount;
	tIB.iSize = iSize;
	tIB.eUsage = eUsage;
	tIB.pData = new char[iCount * iSize];
	tIB.eFmt = eFmt;

	memcpy(tIB.pData, pData, iCount * iSize);

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iCount * iSize;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDesc.Usage = eUsage;

	if (eUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tResource = {};
	tResource.pSysMem = tIB.pData;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&tDesc, &tResource, &tIB.pBuffer)))
		return false;

	pContainer->vecIB.push_back(tIB);

	return true;
}
