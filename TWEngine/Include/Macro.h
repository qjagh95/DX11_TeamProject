#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define	SHADOW_WIDTH	2048
#define	SHADOW_HEIGHT	2048

#define	ROOT_PATH							"RootPath"
#define	SHADER_PATH							"ShaderPath"
#define	TEXTURE_PATH						"TexturePath"
#define	SOUND_PATH							"SoundPath"
#define	DATA_PATH							"DataPath"
#define	MESH_PATH							"MeshPath"
#define	MESH_DATA_PATH						"MeshDataPath"

// ========== Default Sampler Key ==========
#define	SAMPLER_LINEAR						"Linear"
#define SAMPLER_POINT						"Point"

// ========== Shader Key ========
#define	BAR_SHADER							"BarShader"
#define SSAO_SHADER							"SSAOShader"
#define SHADOW_SHADER						"ShadowShader"
#define	BUTTON_SHADER						"ButtonShader"
#define	COLLIDER_SHADER						"ColliderShader"
#define	FULLSCREEN_SHADER					"FullScreenShader"
#define DS_GBUFFER_SHADER					"DownScaledGBufferShader"
#define	LIGHT_BLEND_SHADER					"LightBlendShader"
#define	STANDARD_TEX_SHADER					"StandardTexShader"
#define	STANDARD_BUMP_SHADER				"StandardBumpShader"
#define LIGHT_DIR_ACC_SHADER				"LightDirAccShader"
#define	STANDARD_COLOR_SHADER				"StandardColorShader"
#define LIGHT_SPOT_ACC_SHADER				"LightSpotAccShader"
#define LIGHT_POINT_ACC_SHADER				"LightPointAccShader"
#define	STANDARD_TEX_STATIC_SHADER			"StandardTexStaticShader"
#define	STANDARD_NORMAL_COLOR_SHADER		"StandardNormalColorShader"
#define	STANDARD_BUMP_ANIM_SHADER			"StandardBumpAnimShader"
#define	STANDARD_TEX_NORMAL_SHADER			"StandardTexNormalShader"
#define	STANDARD_TEX_NORMAL_ANIM_SHADER		"StandardTexNormalAnimShader"
#define LANDSCAPE_SHADER					"LandScapeShader"
#define DECAL_SHADER						"DecalShader"
#define PARTICLE_SHADER						"ParticleShader"
#define SHADOWMAP_SHADER					"ShadowShader"
#define STARLIGHTSCOPE_SHADER				"StarLightScope"
#define NAVI_EDITOR_SHADER					"NaviEditorShader"
#define LAND_NAVI_EDITOR_SHADER				"LandNaviEditorShader"
#define SHADOWTEX_SHADER					"ShadowTexureShader"
#define DOWNSCALE_SHADOW_SHADER				"DownScaleShadowShader"

#define VOLUME_FOG_FIRST_SHADER				"VolumeFogFirstShader"
#define VOLUME_FOG_SECOND_SHADER			"VolumeFogSecondShader"
#define VOLUME_FOG_BACK_SHADER				"VolumeFogBackShader"

#define FINAL_PASS_SHADER					"HDRShader"
#define ADAPTATION_SHADER					"AdaptationShader"
#define BLOOM_SHADER						"BloomShader"

#define HDR_COMPUTE_SHADER					"HDRComputeShader"
#define HDR_SECOND_COMPUTE_SHADER			"HDRSecondComputeShader"
#define ADAPT_COMPUTE_SHADER				"AdaptComputeShader"
#define ADAPT_SECOND_COMPUTE_SHADER			"AdaptSecondComputeShader"
#define BLOOM_COMPUTE_SHADER				"BloomComputeShdader"

#define MOTION_BLUR_SHADER					"MotionBlurShader"
#define DOWNSCALE_SHADER					"DownScaleShader"
#define HORIZONTAL_BLUR_SHADER				"HorizontalBlur"
#define VERTICAL_BLUR_SHADER				"VerticalBlur"

#define BLOOM_HORIZONTAL_BLUR_SHADER		"BloomHorizontalBlur"
#define BLOOM_VERTICAL_BLUR_SHADER			"BloomVerticalBlur"

// ========== Input Layout Key =============
#define	POS_COLOR_LAYOUT					"PosColorLayout"
#define	POS_NORMAL_COLOR_LAYOUT				"PosNormalColorLayout"
#define	POS_UV_LAYOUT						"PosUVLayout"
#define	POS_LAYOUT							"PosLayout"
#define	VERTEX3D_LAYOUT						"Vertex3DLayout"

// ========== RenderState Key =============
#define	ALPHA_BLEND							"AlphaBlend"
#define ACC_BLEND							"AccBlend"
#define	DEPTH_DISABLE						"DepthDisable"
#define CULL_NONE							"CullNone"
#define ZERO_BLEND							"ZeroBlend"
#define ALL_BLEND							"AllBlend"
#define WIRE_FRAME							"WireFrame"
#define DEPTH_GRATOR						"DepthGrator"
#define DEPTH_LESS							"DepthLess"
#define FRONT_CULL							"FrontCull"
#define BACK_CULL							"BackCull"
#define	DEPTH_LESSEQUAL						"DepthLessEqual"

// ========== Volume Mesh Key=============
#define SPHERE_VOLUME						"SphereVolum"
#define CAPSUL_VOLUME						"CapsulVolum"
#define CORN_VOLUME							"CornVolum"
#define CYLINDER_VOLUME						"CylinderVolum"

#define	SAFE_DELETE(p)			if(p)	{ delete p; p = nullptr; }
#define	SAFE_RELEASE(p)			if(p)	{ p->Release(); p = nullptr; }
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

#define GET_SINGLE(Type)		Type::GetInst()
#define DESTROY_SINGLE(Type)	Type::DestroyInst()

#define DEVICE					CDevice::GetInst()->GetDevice()
#define CONTEXT					CDevice::GetInst()->GetContext()
#define SWAPCHAIN				CDevice::GetInst()->GetSwapChain()
#define _RESOLUTION				CDevice::GetInst()->GetResolution()

#define WINDOWHANDLE			CCore::GetInst()->GetWindowHandle()
#define WINDOWINSTANCE			CCore::GetInst()->GetWindowInstance()

#define	KEYPRESS(key)			CInput::GetInst()->KeyPress(key)
#define	KEYPUSH(key)			CInput::GetInst()->KeyPush(key)
#define	KEYUP(key)				CInput::GetInst()->KeyUp(key)

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
