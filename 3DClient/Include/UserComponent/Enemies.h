#pragma once
#include <Component/UserComponent.h>
#include <Component/Animation.h>
#include <Component/SoundSource.h>

enum ENEMY_DATA_TYPE {
	EDT_NONE,
	EDT_INT,
	EDT_FLOAT,
	EDT_STRING,
	EDT_SOUND,
	EDT_MESH,
	EDT_ANIM,
	EDT_BONE,
	EDT_SOCKET,
	EDT_WEAPON
};

enum ENEMY_WEAPON_TYPE 
{
	EWT_NONE,
	EWT_PIPE,
	EWT_KNIFE
};

enum ENEMY_AI_STATE
{
	EAS_NONE,
	EAS_IDLE, //기본상태
	EAS_ALERTED = 2, //플레이어 존재 의심(어딘지 모를수 있음)
	EAS_COMBAT = 4, // 전투 상태(플레이어 상대)
	EAS_FLEE = 8, //도망가기(혹시몰라 맨듬)
	EAS_HAVETARGET = 0x10, //타겟(플레이어 찾음)
	EAS_LOSTTARGET = 0x20, //놓침(플레이어 못찾음)
	EAS_SEARCHING = 0x40, //수색(침대, 락커 등 뒤지기)
	EAS_HP_NORMAL = 0x80, //헬스 좋음
	EAS_HP_LOW = 0x100, //빨피
	EAS_HAVE_WEAPON = 0x200, //무기(파이프, 나이프)
	EAS_GETHIT_ANIM = 0x400,
	EAS_ATTACKING = 0x800,
	EAS_DYING = 0x1000,
	EAS_DEAD = 0x2000
};

class Enemies : public PUN::CUserComponent {
	friend class PUN::CGameObject;
protected:
	Enemies();
	Enemies(const Enemies& enem);
	~Enemies();

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	void AfterClone();

	Enemies *Clone();

private:
	static std::wstring m_strEnemDataPath[5];
	std::string m_strIdleSndKey			;
	std::string m_strAngerSndKey		;
	std::string m_strAttackSndKey		;
	std::string m_strHitSndKey			;
	std::string m_strLostSndKey			;
	std::string m_strDeathSndKey		;
	std::string m_strDefaultAnimKey		;
	float		m_fVisionDistance		;
	float		m_fHearingDistance		;
	float		m_fWalkSpeed			;
	float		m_fStrafeSpeed			;
	float		m_fSprintSpeed			;
	float		m_fSprintTimeBuf		;
	float		m_fAttackTime			;
	float		m_fAttackTimeBuf		;
	float		m_fSprintTime			;
	float		m_fAtk					;
	float		m_fHP					;
	float		m_fCurrHp				;
	float		m_fHPLowThreshold		;
	float		m_fDyingAnimSpeed		;
	int			m_iEnemState			;
	PUN::CAnimation *m_pAnim;
	PUN::CSoundSource *m_pSound;
	ENEMY_WEAPON_TYPE m_eWeap;

public:
	static PUN::CGameObject *GetInstance(const std::string& strName, int iType, class PUN::CLayer* pLayer, bool bDestroy = false);
	bool LoadData(const std::wstring strPath);
	int GetState() const;
	bool IsIdle() const;
	bool IsAlerted() const;
	bool InCombat() const;
	bool IsFleeing() const;
	bool GotTarget() const;
	bool HasLostTarget() const;
	bool IsSearchingPlayer() const;
	bool IsHealthEnough();
	bool IsHealthLow();
	bool HasWeapon() const;

	void FindEnemy(float fTime);
	void OnIdle(float fTime);
	void OnCombatStart();
	void OnCombat(float fTime);
	void OnSearchStart();
	void OnSearching(float fTime);
	void OnSearchFail();
	void OnSearchSuccess();
	void GetHit(float fDamage);
	void OnGettingHit(float fTime);
	void Attack(float fDamage);
	void OnAttacking(float fTime);

	void AttackSndCallback(float fTime);
	void FootStepSndCallback(float fTime);
	void FootStepRunCallback(float fTime);
};