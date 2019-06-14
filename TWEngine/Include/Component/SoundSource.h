#pragma once
#include "Component.h"

namespace PUN 
{
	//���� : GameObject�� �ھƳִ� ����� ���� 3D ����� �ϰڽ��ϴ�.
	
	class PUN_DLL CSoundSource :
		public PUN::CComponent
	{
		friend class PUN::CGameObject;

	protected:
		CSoundSource();
		CSoundSource(const CSoundSource& src);
		~CSoundSource();

	public:
		virtual bool Init();
		virtual void AfterClone();
		virtual int Update(float fTime);
		virtual int LateUpdate(float fTime);
		virtual void Collision(float fTime);
		virtual void Render(float fTime);
		virtual CSoundSource *Clone();

	public:
		void Play(int idxKey = 0, bool bLoop = false);
		void Play(string& strKey, bool bLoop = false);
		void PauseClip(int idxKey = 0);
		void PauseClip(string& strKey);

		void StopClip(int idxKey = 0, bool bImmediate = true);
		void StopClip(string& strKey, bool bImmediate = true);
		int GetClipState(string& strKey) const;
		int GetClipState(int idxKey = 0) const;

		bool LoadSounds(const vector<string>& vecStr);
		bool LoadSounds(const char** arrStr, int iCnt);
		bool LoadSounds(const char** arrStrName, const TCHAR** arrFilePath, int iCnt, const string& PathKey = SOUND_PATH);
		bool LoadSounds(const vector<string>& vecStrName, const vector<wstring>& vecFilePath, const string& PathKey = SOUND_PATH);
		bool LoadSound(const string& strName, const TCHAR* pFileName, const string& strPathKey = SOUND_PATH);
		bool LoadSound(int iIndex, const string& strName, const TCHAR* pFileName, const string& strPathKey = SOUND_PATH);

		float GetRadius() const;

		void SetVolume(int iClipIdx, float vol);
		void SetPitch(int iClipIdx, float pitch);//Pitch�� -1.f ~ 1f������ ����(���� ����)
		void SetVolume(string strClip, float vol);
		void SetPitch(string strClip, float pitch);
		void SetRadius(float rad);

		AudioEmitter& GetEmitterInfo();
	private:
		int m_iChannelCnt;
		unordered_map<string, size_t> m_mapSndInstKey;
		vector<string> m_vecSndStr;
		
		float m_fVolume;
		float m_fPitch;
		vector<shared_ptr<SoundEffectInstance>> m_vecsPtrSound;
		vector<shared_ptr<SoundEffect>> m_vecsPtrSound_Data; //�̰� �־�� Forget�� ����Ǿ ������Ʈ�� ���尡 �����ְ� ��
		AudioEmitter m_tAudioEmitter;
	};

}