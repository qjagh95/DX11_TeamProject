#include "EngineHeader.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "../Device.h"

PUN_USING

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{
	m_iInputSize = 0;
}

CShaderManager::~CShaderManager()
{
	unordered_map<string, PCBuffer>::iterator	iter;
	unordered_map<string, PCBuffer>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
		SAFE_DELETE(iter->second);
	}

	m_mapCBuffer.clear();

	Safe_Release_Map(m_mapInputLayout);
	Safe_Release_Map(m_mapShader);
}

bool CShaderManager::Init()
{
	char*	pEntry[ST_END] = {};	

	pEntry[ST_VERTEX] = (char*)"StandardColorVS";
	pEntry[ST_PIXEL] = (char*)"StandardColorPS";
	if (!LoadShader(STANDARD_COLOR_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	AddInputElement((char*)"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);

	if (!CreateInputLayout(POS_COLOR_LAYOUT, STANDARD_COLOR_SHADER))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardTexVS";
	pEntry[ST_PIXEL] = (char*)"StandardTexPS";
	if (!LoadShader(STANDARD_TEX_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardTexStaticVS";
	pEntry[ST_PIXEL] = (char*)"StandardTexStaticPS";
	if (!LoadShader(STANDARD_TEX_STATIC_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"UIVS";
	pEntry[ST_PIXEL] = (char*)"ButtonPS";
	if (!LoadShader(BUTTON_SHADER, TEXT("UI.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"UIVS";
	pEntry[ST_PIXEL] = (char*)"BarPS";
	if (!LoadShader(BAR_SHADER, TEXT("UI.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"FullScreenVS";
	pEntry[ST_PIXEL] = (char*)"FullScreenPS";
	if (!LoadShader(FULLSCREEN_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"LightDirVS";
	pEntry[ST_PIXEL] = (char*)"LightAccPS";
	if (!LoadShader(LIGHT_DIR_ACC_SHADER, TEXT("Light.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"LightPointVS";
	pEntry[ST_PIXEL] = (char*)"LightAccPS";
	if (!LoadShader(LIGHT_POINT_ACC_SHADER, TEXT("Light.fx"), pEntry))
		return false;

	AddInputElement((char*)"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);

	if (!CreateInputLayout(POS_LAYOUT, LIGHT_POINT_ACC_SHADER))
		return false;

	pEntry[ST_VERTEX] = (char*)"LightPointVS";
	pEntry[ST_PIXEL] = (char*)"LightAccPS";
	if (!LoadShader(LIGHT_SPOT_ACC_SHADER, TEXT("Light.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"LightDirVS";
	pEntry[ST_PIXEL] = (char*)"LightBlendPS";
	if (!LoadShader(LIGHT_BLEND_SHADER, TEXT("Light.fx"), pEntry))
		return false;


	AddInputElement((char*)"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 8);

	if (!CreateInputLayout(POS_UV_LAYOUT, STANDARD_TEX_SHADER))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardNormalColorVS";
	pEntry[ST_PIXEL] = (char*)"StandardNormalColorPS";
	if (!LoadShader(STANDARD_NORMAL_COLOR_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	AddInputElement((char*)"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);

	if (!CreateInputLayout(POS_NORMAL_COLOR_LAYOUT, STANDARD_NORMAL_COLOR_SHADER))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardBumpVS";
	pEntry[ST_PIXEL] = (char*)"StandardBumpPS";
	if (!LoadShader(STANDARD_BUMP_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardBumpAnimVS";
	pEntry[ST_PIXEL] = (char*)"StandardBumpPS";
	if (!LoadShader(STANDARD_BUMP_ANIM_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardTexNormalVS";
	pEntry[ST_PIXEL] = (char*)"StandardBumpPS";
	if (!LoadShader(STANDARD_TEX_NORMAL_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"StandardTexNormalAnimVS";
	pEntry[ST_PIXEL] = (char*)"StandardBumpPS";
	if (!LoadShader(STANDARD_TEX_NORMAL_ANIM_SHADER, TEXT("Standard.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"LandScapeVS";
	pEntry[ST_PIXEL] = (char*)"LandScapePS";
	if (!LoadShader(LANDSCAPE_SHADER, TEXT("LandScape.fx"), pEntry))
		return false;

	pEntry[ST_VERTEX] = (char*)"SkyVS";
	pEntry[ST_PIXEL] = (char*)"SkyPS";
	if (!LoadShader("Sky", TEXT("Sky.fx"), pEntry))
		return false;

//#ifdef _DEBUG
	pEntry[ST_VERTEX] = (char*)"ColliderVS";
	pEntry[ST_PIXEL] = (char*)"ColliderPS";
	if (!LoadShader(COLLIDER_SHADER, TEXT("Collider.fx"), pEntry))
		return false;

//#endif	// _DEBUG

	pEntry[ST_VERTEX] = (char*)"Vertex3DVS";
	pEntry[ST_PIXEL] = (char*)"Vertex3DPS";
	if (!LoadShader("Vertex3D", TEXT("Share.fx"), pEntry))
		return false;

	AddInputElement((char*)"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 8);
	AddInputElement((char*)"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);
	AddInputElement((char*)"BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
	AddInputElement((char*)"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 16);

	if (!CreateInputLayout(VERTEX3D_LAYOUT, "Vertex3D"))
		return false;

	// 상수버퍼 만들기
	CreateCBuffer("Transform", 0, sizeof(TransformCBuffer),
		CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Material", 1, sizeof(Material), CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Component", 2, sizeof(ComponentCBuffer),
		CST_VERTEX | CST_PIXEL);

	CreateCBuffer("Collider", 8, sizeof(Vector4),
		CST_VERTEX | CST_PIXEL);
	//CreateCBuffer("Animation2D", 8, sizeof(Animation2DCBuffer),
	//	CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Button", 9, sizeof(ButtonCBuffer), CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Bar", 9, sizeof(BarCBuffer), CST_VERTEX | CST_PIXEL);
	CreateCBuffer("Light", 3, sizeof(LightInfo), CST_VERTEX | CST_PIXEL);
	CreateCBuffer("PublicCBuffer", 5, sizeof(PublicCBuffer), CST_VERTEX | CST_PIXEL);

	return true;
}

bool CShaderManager::LoadShader(const string & strName,
	const TCHAR * pFileName, char * pEntry[ST_END],
	const string & strPathKey)
{
	CShader*	pShader = FindShader(strName);

	if (pShader)
	{
		SAFE_RELEASE(pShader);
		return true;
	}

	pShader = new CShader;

	if (!pShader->LoadShader(strName, pFileName, pEntry, strPathKey))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	m_mapShader.insert(make_pair(strName, pShader));

	return true;
}

CShader * CShaderManager::FindShader(const string & strName)
{
	unordered_map<string, CShader*>::iterator	iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CShader * CShaderManager::FindShaderNonCount(const string & strName)
{
	unordered_map<string, CShader*>::iterator	iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}


void CShaderManager::AddInputElement(char * pSemantic, int iIdx,
	DXGI_FORMAT eFmt, int iSize, int iInputSlot,
	D3D11_INPUT_CLASSIFICATION eInputClass, int iInstanceStepRate)
{
	D3D11_INPUT_ELEMENT_DESC	tDesc = {};

	tDesc.SemanticName = pSemantic;
	tDesc.SemanticIndex = iIdx;
	tDesc.Format = eFmt;
	tDesc.AlignedByteOffset = m_iInputSize;
	tDesc.InputSlot = iInputSlot;
	tDesc.InputSlotClass = eInputClass;
	tDesc.InstanceDataStepRate = iInstanceStepRate;

	m_iInputSize += iSize;

	m_vecInputDesc.push_back(tDesc);

}

bool CShaderManager::CreateInputLayout(const string & strName,
	const string & strShaderKey)
{
	CShader*	pShader = FindShader(strShaderKey);

	if (!pShader)
		return false;

	ID3D11InputLayout*	pLayout = FindInputLayout(strName);

	if (pLayout)
		return false;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(&m_vecInputDesc[0],
		(UINT)m_vecInputDesc.size(), pShader->GetVSCode(),
		pShader->GetVSCodeSize(), &pLayout)))
		return false;

	SAFE_RELEASE(pShader);

	m_iInputSize = 0;

	m_vecInputDesc.clear();

	m_mapInputLayout.insert(make_pair(strName, pLayout));

	return true;
}

ID3D11InputLayout * CShaderManager::FindInputLayout(const string & strName)
{
	unordered_map<string, ID3D11InputLayout*>::iterator	iter = m_mapInputLayout.find(strName);

	if (iter == m_mapInputLayout.end())
		return nullptr;

	return iter->second;
}

bool CShaderManager::CreateCBuffer(const string & strName,
	int iRegister, int iSize, int iShaderType)
{
	PCBuffer	pBuffer = FindCBuffer(strName);

	if (pBuffer)
		return false;

	pBuffer = new CBuffer;

	pBuffer->iSize = iSize;
	pBuffer->iShaderType = iShaderType;
	pBuffer->iRegister = iRegister;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr,
		&pBuffer->pBuffer)))
		return false;

	m_mapCBuffer.insert(make_pair(strName, pBuffer));

	return true;
}

bool CShaderManager::UpdateCBuffer(const string & strName,
	void * pData)
{
	PCBuffer	pBuffer = FindCBuffer(strName);

	if (!pBuffer)
		return false;

	

	D3D11_MAPPED_SUBRESOURCE	tMap = {};
	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD,
		0, &tMap);

	memcpy(tMap.pData, pData, pBuffer->iSize);

	CONTEXT->Unmap(pBuffer->pBuffer, 0);

	

	if (pBuffer->iShaderType & CST_VERTEX)
		CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iShaderType & CST_PIXEL)
		CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	return true;
}

PCBuffer CShaderManager::FindCBuffer(const string & strName)
{
	unordered_map<string, PCBuffer>::iterator	iter = m_mapCBuffer.find(strName);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}