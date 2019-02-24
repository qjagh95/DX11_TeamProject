#pragma once

#define	ROOT_PATH	"RootPath"
#define	SHADER_PATH	"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define	SOUND_PATH	"SoundPath"
#define	DATA_PATH	"DataPath"
#define	MESH_PATH	"MeshPath"

// ========== Default Sampler Key ==========
#define	SAMPLER_LINEAR	"Linear"
#define SAMPLER_POINT   "Point"

// ========== Shader Key ========
#define	STANDARD_COLOR_SHADER	"StandardColorShader"
#define	STANDARD_TEX_SHADER	"StandardTexShader"
#define	STANDARD_TEX_STATIC_SHADER	"StandardTexStaticShader"
#define	COLLIDER_SHADER	"ColliderShader"
#define	BUTTON_SHADER	"ButtonShader"
#define	BAR_SHADER		"BarShader"
#define	FULLSCREEN_SHADER	"FullScreenShader"
#define LIGHT_DIR_ACC_SHADER "LightDirAccShader"
#define LIGHT_POINT_ACC_SHADER "LightPointAccShader"
#define LIGHT_SPOT_ACC_SHADER "LightSpotAccShader"
#define	LIGHT_BLEND_SHADER  "LightBlendShader"
#define SHADOW_SHADER	"ShadowShader"
#define	STANDARD_NORMAL_COLOR_SHADER	"StandardNormalColorShader"
#define	STANDARD_BUMP_SHADER		"StandardBumpShader"
#define	STANDARD_BUMP_ANIM_SHADER		"StandardBumpAnimShader"
#define	STANDARD_TEX_NORMAL_SHADER		"StandardTexNormalShader"
#define	STANDARD_TEX_NORMAL_ANIM_SHADER		"StandardTexNormalAnimShader"
#define LANDSCAPE_SHADER					"LandScapeShader"
// ========== Input Layout Key =============
#define	POS_COLOR_LAYOUT		"PosColorLayout"
#define	POS_NORMAL_COLOR_LAYOUT		"PosNormalColorLayout"
#define	POS_UV_LAYOUT		"PosUVLayout"
#define	POS_LAYOUT		"PosLayout"
#define	VERTEX3D_LAYOUT	"Vertex3DLayout"

// ========== RenderState Key =============
#define	ALPHA_BLEND			"AlphaBlend"
#define ACC_BLEND			"AccBlend"
#define	DEPTH_DISABLE		"DepthDisable"
#define CULL_NONE			"CullNone"
#define ZERO_BLEND "ZeroBlend"
#define ALL_BLEND "AllBlend"
#define WIRE_FRAME "WireFrame"
#define DEPTH_GRATOR "DepthGrator"
#define DEPTH_LESS "DepthLess"
#define FRONT_CULL "FrontCull"
#define BACK_CULL "BackCull"
#define	DEPTH_LESSEQUAL		"DepthLessEqual"

// ========== Volume Mesh Key=============
#define SPHERE_VOLUME "SphereVolum"
#define CAPSUL_VOLUME "CapsulVolum"
#define CORN_VOLUME "CornVolum"
#define CYLINDER_VOLUME "CylinderVolum"

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