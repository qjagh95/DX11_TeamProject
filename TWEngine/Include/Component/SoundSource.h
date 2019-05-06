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
		void Play(std::string& strKey, bool bLoop = false);
		void PauseClip(int idxKey = 0);
		void PauseClip(std::string& strKey);

		void StopClip(int idxKey = 0, bool bImmediate = true);
		void StopClip(std::string& strKey, bool bImmediate = true);
		int GetClipState(std::string& strKey) const;
		int GetClipState(int idxKey = 0) const;

		bool LoadSounds(const vector<std::string>& vecStr);
		bool LoadSounds(const char** arrStr, int iCnt);
		bool LoadSounds(const char** arrStrName, const TCHAR** arrFilePath, int iCnt, const string& PathKey = SOUND_PATH);
		bool LoadSounds(const vector<std::string>& vecStrName, const vector<wstring>& vecFilePath, const string& PathKey = SOUND_PATH);
		bool LoadSound(const string& strName, const TCHAR* pFileName, const string& strPathKey = SOUND_PATH);
		bool LoadSound(int iIndex, const string& strName, const TCHAR* pFileName, const string& strPathKey = SOUND_PATH);

		float GetRadius() const;

		void SetVolume(int iClipIdx, float vol);
		void SetPitch(int iClipIdx, float pitch);//Pitch�� -1.f ~ 1f������ ����(���� ����)
		void SetVolume(std::string strClip, float vol);
		void SetPitch(std::string strClip, float pitch);
		void SetRadius(float rad);

		DirectX::AudioEmitter& GetEmitterInfo();
	private:
		int m_iChannelCnt;
		std::unordered_map<std::string, size_t> m_mapSndInstKey;
		std::vector<std::string> m_vecSndStr;
		
		float m_fVolume;
		float m_fPitch;
		std::vector<shared_ptr<DirectX::SoundEffectInstance>> m_vecsPtrSound;
		std::vector<shared_ptr<DirectX::SoundEffect>> m_vecsPtrSound_Data; //�̰� �־�� Forget�� ����Ǿ ������Ʈ�� ���尡 �����ְ� ��
		DirectX::AudioEmitter m_tAudioEmitter;
	};

}