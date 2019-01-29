#pragma once
#include "Component.h"

PUN_BEGIN

enum TILE2D_OPTION
{
	T2D_NORMAL,
	T2D_NOMOVE
};

enum TILE2D_FIND_LIST
{
	TFL_NONE,
	TFL_OPEN,
	TFL_CLOSE
};

class PUN_DLL CTile2D :
	public CComponent
{
	friend class CGameObject;

private:
	CTile2D();
	CTile2D(const CTile2D& tile2D);
	~CTile2D();

private:
	TILE2D_OPTION		m_eTileOption;
	STAGE2D_TILE_TYPE	m_eTileType;
	class CShader*		m_pShader;
	class CMesh*		m_pMesh;
	ID3D11InputLayout*	m_pLayout;
	bool				m_bTileLine;
	TILE2D_FIND_LIST	m_eFindList;
	float				m_fG; // ±×·¡ÇÁ
	float				m_fH; // ÈÞ¸®½ºÆ½
	float				m_fTotal;// ÃÑ °£¼±ºñ¿ë
	int					m_iIndex;
	int					m_iParent;
	int					m_iAdjNode[AND_END];
	bool				m_bFind;

public:
	TILE2D_OPTION		GetTile2DOption()	const;
	STAGE2D_TILE_TYPE	GetTileType()	const;
	TILE2D_FIND_LIST	GetTileFindList()	const;
	int					GetAdjNodeIndex(ASTAR_NODE_DIR eDir)	const;
	float	GetG()	const;
	float	GetH()	const;
	float	GetTotal()	const;
	int		GetIndex()	const;
	int		GetParent()	const;
	void	EnableFind();

public:
	void SetTileOption(TILE2D_OPTION eOption);
	void SetTileType(STAGE2D_TILE_TYPE eType);
	void SetTileFindList(TILE2D_FIND_LIST eTFL);
	void OnTile(bool bTile);
	void ClearFindSet();
	void SetAdjTile(int iIndex, ASTAR_NODE_DIR eDir);
	void SetParent(int iParent);
	void SetG(float fG);
	void SetH(float fH);
	void Total();
	void SetIndex(int iIndex);

public:
	virtual void Start();
	virtual void AfterClone();
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);	
	virtual CTile2D* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

PUN_END