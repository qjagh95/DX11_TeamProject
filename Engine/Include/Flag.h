#pragma once

namespace PUN
{
	enum COMPONENT_TYPE
	{
		CT_TRANSFORM,
		CT_RENDERER,
		CT_CAMERA,
		CT_ARM,
		CT_FREECAMERA,
		CT_MATERIAL,
		CT_ANIMATION2D,
		CT_ANIMATION,
		CT_COLLIDER,
		CT_UI,
		CT_TEXT,
		CT_SOUND,
		CT_STAGE2D,
		CT_TILE2D,
		CT_NAVI2D,
		CT_LIGHT,
		CT_LANDSCAPE,
		CT_VOLUMEFOG,
		CT_DECAL,
		CT_PARTICLE,
		CT_GIZMO,
		CT_END
	};

	enum AXIS
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_END
	};

	enum SHADER_TYPE
	{
		ST_VERTEX,
		ST_PIXEL,
		ST_GEOMETRY,
		ST_COMPUTE,
		ST_END
	};

	enum CBUFFER_SHADER_TYPE
	{
		CST_VERTEX = 0X1,
		CST_PIXEL = 0X2,
		CST_COMPUTE = 0X4,
		CST_GEOMETRY = 0X8
	};

	enum BUFFER_TYPE
	{
		BT_BUFFER,
		BT_TEXTURE2D,
		BT_END
	};

	enum CAMERA_TYPE
	{
		CT_PERSPECTIVE,
		CT_ORTHO
	};

	enum RENDER_STATE
	{
		RS_BLEND,
		RS_RASTERIZER,
		RS_DEPTHSTENCIL,
		RS_END
	};

	enum GAME_MODE
	{
		GM_2D,
		GM_3D
	};

	enum ANIMATION2D_TYPE
	{
		A2D_ATLAS,
		A2D_FRAME
	};

	enum ANIMATION_OPTION
	{
		AO_LOOP,
		AO_ONCE_RETURN,
		AO_ONCE_DESTROY,
		AO_ONCE_LAST
	};

	enum COLLIDER_TYPE
	{
		CT_RECT,
		CT_POINT,
		CT_OBB,
		CT_PIXEL,
		CT_SPHERE,
		CT_OBB3D,
		CT_RAY
	};

	enum COLLISION_CALLBACK_TYPE
	{
		CCT_ENTER,
		CCT_STAY,
		CCT_LEAVE,
		CCT_END
	};

	enum UI_TYPE
	{
		UT_PANEL,
		UT_BUTTON,
		UT_BAR,
		UT_NUMBER,
		UT_TAB,
		UT_SLOT
	};

	enum BUTTON_STATE
	{
		BS_DISABLE,
		BS_NORMAL,
		BS_MOUSEON,
		BS_CLICK,
		BS_END
	};

	enum BAR_DIR
	{
		BD_LEFT,
		BD_RIGHT,
		BD_UP,
		BD_DOWN
	};

	enum TRANSFORM_PARENT_FLAG
	{
		TPF_SCALE = 0X1,
		TPF_ROT = 0X2,
		TPF_POS = 0X4
	};

	enum STAGE2D_TILE_TYPE
	{
		S2D_NONE,
		S2D_RECT,
		S2D_ISOMETRIC
	};

	enum RENDER_GROUP
	{
		RG_LANDSCAPE,
		RG_NORMAL,
		RG_DECAL,
		RG_FOG,
		RG_ALPHA1,
		RG_ALPHA2,
		RG_ALPHA3,
		RG_UI,
		RG_HUD,
		RG_END,
		RG_LIGHT,
		RG_GIZMO
		//RG_LIGHT를 RG_END보다 뒤에 넣은 이유는
		//RG_LIGHT속성은 RenderManager의 m_tLightGroup에 들어가야하는지 여부만 체크하기 위함이다.
		//RenderManager에 적어놓았듯이 렌더러가 있다면(빛이면서 동시에 그려질 오브젝트라면) 
		//NORMAL렌더그룹에 포함시켜준다.
	};

	enum ASTAR_NODE_DIR
	{
		AND_LT,
		AND_T,
		AND_RT,
		AND_R,
		AND_RB,
		AND_B,
		AND_LB,
		AND_L,
		AND_END
	};

	// Object Collider
	enum OBJECT_COLLISION_TYPE
	{
		OCT_PLAYER,
		OCT_MONSTER,
		OCT_SKILL,
		OCT_NONE
	};

	// Character Tab State
	enum CHARACTER_TAB_STATE
	{
		CTS_NORMAL,
		CTS_CLICK,
		CTS_MOUSEON,
		CTS_DISABLE,
		CTS_END
	};

	// Equip Tab State
	enum EQUIP_TAB_STATE
	{
		ETS_NORMAL,
		ETS_CLICK,
		ETS_MOUSEON,
		ETS_DISABLE,
		ETS_END
	};

	// Inven Tab State
	enum INVEN_TAB_STATE
	{
		ITS_NORMAL,
		ITS_CLICK,
		ITS_MOUSEON,
		ITS_DISABLE,
		ITS_END
	};

	// Club Tab State
	enum CLUB_TAB_STATE
	{
		CLTS_NORMAL,
		CLTS_CLICK,
		CLTS_MOUSEON,
		CLTS_DISABLE,
		CLTS_END
	};

	// Option Tab State
	enum OPTION_TAB_STATE
	{
		OTS_NORMAL,
		OTS_CLICK,
		OTS_MOUSEON,
		OTS_DISABLE,
		OTS_END
	};

	// Team Tab State
	enum TEAM_TAB_STATE
	{
		TTS_NORMAL,
		TTS_CLICK,
		TTS_MOUSEON,
		TTS_DISABLE,
		TTS_END
	};

	// Combo Tab State
	enum COMBO_TAB_STATE
	{
		COTS_NORMAL,
		COTS_CLICK,
		COTS_MOUSEON,
		COTS_DISABLE,
		COTS_END
	};

	// Quick Slot Buttn State
	enum QUICK_SLOT_BUTTON_STATE
	{
		QS_NORMAL,
		QS_CLICK,
		QS_MOUSEON,
		QS_DISABLE,
		QS_END
	};

	// End Button
	enum END_BUTTON_STATE
	{
		EBS_DISABLE,
		EBS_NORMAL,
		EBS_MOUSEON,
		EBS_CLICK,
		EBS_END
	};

	// Light Type
	enum LIGHT_TYPE
	{
		LT_DIR,
		LT_POINT,
		LT_SPOT,
		LT_END
	};

	enum SKEY_TYPE
	{
		SKEY_SHIFT,
		SKEY_CONTROL,
		SKEY_ALT,
		SKEY_END
	};

	enum ACTION_KEY_FUNCTION_TYPE
	{
		AT_PRESS,
		AT_PUSH,
		AT_RELEASE,
		AT_END
	};

	enum KEY_STATE
	{
		KEY_PRESS = 0x1,
		KEY_PUSH = 0x2,
		KEY_RELEASE = 0x4
	};

	enum MOUSE_STATE
	{
		MS_LBUTTON,
		MS_RBUTTON,
		MS_MBUTTON,
		MS_END
	};

	enum SOUND_TYPE
	{
		ST_EFFECT,
		ST_BGM,
		ST_3D,
		ST_MAX,
	};

	enum GPU_RESOURCE_TYPE
	{
		GRT_BUFFER,
		GRT_BUFFER2,
		GRT_TEXTURE2D,
		GRT_TEXTURE3D,
		GRT_FOR_BLOOM
	};

	enum CS_FILTER_TYPE
	{
		CFT_DOWNSCALE,
		CFT_HDR,
		CFT_BLUR,
		CFT_MOTIONBLUR,
		CFT_ADAPTATION,
		CFT_BLOOM,
		CFT_END
	};


	enum FAVORITE_STATE
	{
		STATE_DEPTH_DISABLE,
		STATE_CULL_NONE,
		STATE_ACC_BLEND,
		STATE_DEPTH_GRATOR,
		STATE_DEPTH_LESS,
		STATE_FRONT_CULL,
		STATE_BACK_CULL,
		STATE_WIRE_FRAME,
		STATE_ZERO_BLEND,
		STATE_ALL_BLEND,
		STATE_ALPHA_BLEND,
		STATE_DEPTH_READ_ONLY,
		STATE_END
	};

	enum FAVORITE_TARGET
	{
		TARGET_ALBEDO,
		TARGET_DEPTH,
		TARGET_NORMAL,
		TARGET_TANGENT,
		TARGET_BINORMAL,
		TARGET_DS_DEPTH,
		TARGET_DS_NORMAL,
		TARGET_SSAO,
		TARGET_BACK,
		TARGET_SKY,
		TARGET_FOG_DEPTH,
		TARGET_ACC_DIFF,
		TARGET_ACC_SPC,
		TARGET_BLEND,
		TARGET_FINAL,
		TARGET_SHADOWMAP,
		TARGET_STARLIGHT_SCOPE,
		TARGET_SHADOW,
		TARGET_SHADOW_TEX,
		TARGET_DS_SHADOW,
		TARGET_END
	};

	enum FAVORITE_SHADER
	{
		SHADER_ACC_DIR,
		SHADER_ACC_POINT,
		SHADER_ACC_SPOT,
		SHADER_BLEND,
		SHADER_FOG_FRONT,
		SHADER_FOG_BACK,
		SHADER_FOG_COLOR,
		SHADER_FULL_SCREEN,
		SHADER_DS_GBUFFER,
		SHADER_DS_SHADOW,
		SHADER_SSAO,
		SHADER_FINAL_PASS,
		SHADER_SHADOW,
		SHADER_SHADOW_TEX,
		SHADER_STARLIGHT_SCOPE,
		SHADER_LAND_EDITOR,
		SHADER_OBJ_EDITOR,
		SHADER_WIRE_FRAME,
		SHADER_END
	};

	enum GIZMO_TYPE
	{
		GT_X,
		GT_Y,
		GT_Z,
		GT_END
	};

	enum BT_ROOT_CHILD_TYPE
	{
		BT_NONE,
		BT_SELECTOR,
		BT_SEQUENCE,
		BT_ACTION,
	};

	enum BT_ACTION_TYPE
	{
		ACTION_NONE,
		ACTION_SUCCED,
		ACTION_RUNNING,
		ACTION_FALSE,
	};

	enum STAGE_SECTION_TYPE
	{
		SST_S1_R0,
		SST_S1_R1,
		SST_S1_R2,
		SST_S1_R3,
		SST_S1_R4,
		SST_S1_R5,
		SST_S1_R6,
		SST_S1_R7,
		SST_S1_R8,
		SST_S1_RG
	};

	enum COLLIDER_ID
	{
		CI_NONE,
	};
}