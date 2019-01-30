#pragma once

#define	ROOT_PATH	"RootPath"
#define	SHADER_PATH	"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define	SOUND_PATH	"SoundPath"
#define	DATA_PATH	"DataPath"

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
#define SHADOW_SHADER	"ShadowShader"
#define	STANDARD_NORMAL_COLOR_SHADER	"StandardNormalColorShader"

// ========== Input Layout Key =============
#define	POS_COLOR_LAYOUT		"PosColorLayout"
#define	POS_NORMAL_COLOR_LAYOUT		"PosNormalColorLayout"
#define	POS_UV_LAYOUT		"PosUVLayout"
#define	POS_LAYOUT		"PosLayout"

// ========== RenderState Key =============
#define	ALPHA_BLEND			"AlphaBlend"
#define ACC_BLEND			"AccBlend"
#define	DEPTH_DISABLE		"DepthDisable"

enum SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_END
};