#pragma once

#define PUN_BEGIN	namespace PUN{
#define PUN_END		}
#define PUN_USING	using namespace PUN;

#ifdef PUN_EXPORT
#define PUN_DLL __declspec(dllexport) // 메모리 정렬도 해준다
#else
#define PUN_DLL __declspec(dllimport)
#endif // PUN_EXPORT

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }

template <typename T>
void Safe_Delete_VecList(T& p)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	p.clear();
}


template <typename T>
void Safe_Delete_Map(T& p)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}

	p.clear();
}

#define	DECLARE_SINGLE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst(); \
		static void DestroyInst();\
	private:\
		Type();\
		~Type();


#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst = nullptr;\
	Type* Type::GetInst()\
	{\
		if (!m_pInst)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	void Type::DestroyInst()\
	{\
		SAFE_DELETE(m_pInst);\
	}

#define GET_SINGLE(Type)	Type::GetInst()
#define DESTROY_SINGLE(Type)	Type::DestroyInst()

#define DEVICE	CDevice::GetInst()->GetDevice()
#define CONTEXT	CDevice::GetInst()->GetContext()
#define SWAPCHAIN	CDevice::GetInst()->GetSwapChain()
#define _RESOLUTION	CDevice::GetInst()->GetResolution()

#define WINDOWHANDLE	CCore::GetInst()->GetWindowHandle()
#define WINDOWINSTANCE	CCore::GetInst()->GetWindowInstance()

#define	KEYPRESS(key)	CInput::GetInst()->KeyPress(key)
#define	KEYPUSH(key)	CInput::GetInst()->KeyPush(key)
#define	KEYUP(key)		CInput::GetInst()->KeyUp(key)

#define NULLPTR nullptr

#define CLASS_IN_SINGLE(Type)	\
	private:\
		static Type* Instance;\
	public: \
		static Type* Get();\
		static Type* GetInst();\
		static void Delete();\
	private: \
		Type();\
		~Type();

#define SINGLETON_VAR_INIT(Type) Type* Type::Instance = NULLPTR; \
	Type* Type::Get()\
	{\
		if (Instance == NULLPTR)\
			Instance = new Type;\
		return Instance;\
	}\
	Type* Type::GetInst()\
	{\
		if (Instance == NULLPTR)\
			Instance = new Type;\
		return Instance;\
	}\
	void Type::Delete()\
	{\
		SAFE_DELETE(Instance);\
	}

#define TrueAssert(Var) assert(!(Var))