enum INTERACT_STATE
{
	IS_NONE,
	IS_HIDE_IN_BED,
	IS_HIDING_BED,
	IS_GET_OUT_BED,
};

int m_iRotDir;
float m_fAnimPlayTime;
float m_fDestRotY;
float m_fDestRotX;
float m_fRotX;
float m_fRotY;
float m_fAccRotX;
float m_fAccRotY;
bool	m_bOnGrass;
INTERACT_STATE m_eInteractState;
Vector3 m_vTargetDir;
CGameObject* m_pCameraObj;
CTransform* m_pCameraTr;
PLAYER_STATUS m_eTempPlayerState;
class CLocker *m_pHidingLocker;