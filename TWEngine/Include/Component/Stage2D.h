#pragma once
#include "Component.h"

PUN_BEGIN

class PUN_DLL CStage2D :
	public CComponent
{
	friend class CGameObject;
	friend class CStageManager;

private:
	CStage2D();
	CStage2D(const CStage2D& stage2D);
	~CStage2D();

private:
	vector<class CGameObject*>	m_vecTileObj;
	vector<class CTransform*>	m_vecTileTr;
	vector<class CTile2D*>		m_vecTile;
	UINT						m_iNumX;
	UINT						m_iNumY;
	Vector3						m_vTileScale;
	Vector3						m_vStartPos;
	Vector3						m_vWorldSize;
	STAGE2D_TILE_TYPE			m_eTileType;
	bool						m_bTileLineOn;
	int							m_iStartX;
	int							m_iStartY;
	int							m_iEndX;
	int							m_iEndY;
	int							m_iFlag;

public:
	STAGE2D_TILE_TYPE	GetTileType()	const;
	UINT				GetTileNumX()	const;
	UINT				GetTileNumY()	const;
	Vector3				GetTileScale()	const;
	class CTile2D*		GetTile(const Vector3& vPos)	const;
	class CTile2D*		GetTile(int iIndex)	const;
	size_t				GetTileCount()	const;

public:
	void SetWorldSize(Vector3 vWorldSize);
	void SetTileType(STAGE2D_TILE_TYPE eType);
	void SetTileOption(const Vector3& vPos, enum TILE2D_OPTION eOption);
	void OnTile(bool bTile);
	void ClearFindSet();
	void SetVisible(bool bVisible);
	void SetVisible();

public:
	void CreateTile(UINT iNumX, UINT iNumY, const Vector3& vStart,
		const Vector3& vTileScale, STAGE2D_TILE_TYPE eType,
		const string& strKey = "", const TCHAR* pFileName = nullptr,
		const string& strPathKey = TEXTURE_PATH);

private:
	int GetAdjTileIndex(int x, int y, ASTAR_NODE_DIR eDir, STAGE2D_TILE_TYPE eType);

private:
	void CreateRect(const Vector3& vStart, const Vector3& vTileScale,
		const string& strKey = "", const TCHAR* pFileName = nullptr,
		const string& strPathKey = TEXTURE_PATH);
	void CreateIsometric(const Vector3& vStart, const Vector3& vTileScale,
		const string& strKey = "", const TCHAR* pFileName = nullptr,
		const string& strPathKey = TEXTURE_PATH);
public:
	int GetTileIndex(const Vector3& vPos)	const;

private:
	int GetRectTileIndex(const Vector3& vPos)	const;
	int GetIsometricTileIndex(const Vector3& vPos)	const;
	int GetIsometricTileIndexX(const Vector3& vPos)	const;
	int GetIsometricTileIndexY(const Vector3& vPos)	const;

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CStage2D* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

PUN_END