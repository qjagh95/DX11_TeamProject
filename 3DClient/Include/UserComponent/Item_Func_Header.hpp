//담당 : 태환

// 무적 효과 
void SetInvincibility(bool _isFlag);
bool GetInvincibility();
void SetEnvincibilityTime(float _time);

// 이속 증가 효과
void  SetAccelState(bool _isFlag);
bool  GetAccelState();
void  SetAccelSpeed(float _accSpeed);
float GetAccelSpeed();
void  SetAccelDuration(float _time);
float GetAccelDuration();

// 체력
void  SetMaxHP();
int   GetMaxHP();
void  RecoveryHP(int _hp);
int   GetHP();

void ChangeRayAnim(const string& strName);
class CKeyInven* GetKeyInven();
class CDocxInven* GetDocxInven();
void HitCamEffL(float fTime, float fCurrTime, float fEndTime, PUN::CTransform* pCam);
void HitCamEffR(float fTime, float fCurrTime, float fEndTime, PUN::CTransform* pCam);