//��� : ��ȯ

// ���� ȿ�� 
void SetInvincibility(bool _isFlag);
bool GetInvincibility();
void SetEnvincibilityTime(float _time);

// �̼� ���� ȿ��
void  SetAccelState(bool _isFlag);
bool  GetAccelState();
void  SetAccelSpeed(float _accSpeed);
float GetAccelSpeed();
void  SetAccelDuration(float _time);
float GetAccelDuration();

// ü��
void  SetMaxHP();
int   GetMaxHP();
void  RecoveryHP(int _hp);
int   GetHP();