#pragma once
#include "Component.h"

PUN_BEGIN

class CMesh;
class CNavigationMesh;
class PUN_DLL CLandScape : public CComponent
{
	friend class CGameObject;

protected:
	CLandScape();
	CLandScape(const CLandScape& landscape);
	virtual ~CLandScape();

private:
	unsigned int			m_iNumX;
	unsigned int			m_iNumZ;
	vector<Vertex3DColor>	m_vecVtx;
	vector<int>				m_vecIdx;
	vector<Vector3>			m_vecFaceNormal;
	LandScapeCBuffer		m_tCBuffer;
	CNavigationMesh*		m_pNavMesh;
	CMesh* m_Mesh;
	bool m_isMove;

	NaviLandCBuffer m_tNaviCBuffer;
	GridCBuffer m_tGridCBuffer;

public:
	bool CreateLandScape(const string& strName,
		unsigned int iNumX, unsigned int iNumZ,
		const string& strTexKey = "",
		const TCHAR* pTexName = nullptr, const TCHAR* pTexNormal = nullptr,
		const TCHAR* pTexSpecular = nullptr,
		const char* pFileName = nullptr, const string& strPathKey = TEXTURE_PATH);

	bool AddSplatDifTexture(const string& strName, const vector<const TCHAR*>& pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool AddSplatNrmTexture(const string& strName, const vector<const TCHAR*>& pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool AddSplatSpcTexture(const string& strName, const vector<const TCHAR*>& pFileName,
		const string& strPathName = TEXTURE_PATH);
	bool AddSplatAlphaTexture(const string& strName, const vector<const TCHAR*>& pFileName,
		const string& strPathName = TEXTURE_PATH);

	CNavigationMesh* GetNaviMesh() const { return m_pNavMesh; }

	void SetBrushPos(const Vector3& Pos) { m_tNaviCBuffer.BrushPos = Pos; }
	void SetBrushRange(int Range) { m_tNaviCBuffer.BrushRange = Range; }
	void SetBrushColor(const Vector4& Color) { m_tNaviCBuffer.BrushColor = Color; }
	void SetGridColor(const Vector4& Color) { m_tGridCBuffer.GridColor = Color; }
	void GridShow(bool Value) { m_tGridCBuffer.isVisibleGrid = Value; }
	void SetGridLineSize(int LineSize) { m_tGridCBuffer.GridLineSize = LineSize; }

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLandScape* Clone();

	void SaveLandScape(const string& FullPath);
	void LoadLandScape(const string& FullPath);

private:
	void ComputeNormal();
	void ComputeTangent();
	void ChangeFlag(float DeltaTime);

	void CheckSelectIndex(float DeltaTime);
	void Click();
};

PUN_END
