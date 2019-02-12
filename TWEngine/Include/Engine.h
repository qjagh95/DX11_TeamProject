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
#define	STANDARD_BUMP_SHADER			"StandardBumpShader"

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

// ========== Volume Mesh Key=============
#define SPHERE_VOLUME "SphereVolum"
#define CAPSUL_VOLUME "CapsulVolum"
#define CORN_VOLUME "CornVolum"
#define CYLINDER_VOLUME "CylinderVolum"

enum SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_END
};

#include <dinput.h>
#pragma comment(lib , "dinput8")
#pragma comment(lib, "dxguid")
