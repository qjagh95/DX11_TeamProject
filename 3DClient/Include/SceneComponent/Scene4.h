#pragma once
#include <Scene/SceneComponent.h>

PUN_USING

class CScene4 : public CSceneComponent
{
public:
	CScene4();
	~CScene4();

public:
	void Start()					override;
	bool Init()						override;
	int  Input(float _fTime)		override;
	int  Update(float _fTime)		override;
	int  LateUpdate(float _fTime)	override;
	void Collision(float _fTime)	override;
	void Render(float _fTime)		override;

public:
	void LoadMap();
	void CreateEntryPoint();
	void CreateCorridor();
};