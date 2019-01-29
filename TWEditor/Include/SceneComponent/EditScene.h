#pragma once

#include "Scene/SceneComponent.h"
#include "GameObject.h"
#include "Component/Stage2D.h"
#include "Component/Transform.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

PUN_USING

class CEditScene :
	public CSceneComponent
{
public:
	CEditScene();
	~CEditScene();

public:
	CGameObject*	m_pTileObj;
	CTile2D*		m_pTile;
	CTransform*		m_pTileTr;
	bool			m_bTile;

public:
	virtual bool Init();
	virtual int Input(float fTime);
	virtual int Update(float fTime);
};

