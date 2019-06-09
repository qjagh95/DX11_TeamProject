#pragma once
#include "EngineHeader.h"
#include "Flag.h"

enum USERCOMPONENT_TYPE
{
	UT_NONE = PUN::CT_END,
	UT_PLAYER,
	UT_MONSTER,
	UT_BULLET,
	UT_EFFECT,
	UT_ITEM,
	UT_ICON,
	UT_INVENTORY,
	UT_DOCXINVEN,
	UT_KEYINVEN,
	UT_HANDYCAM,
	UT_EVENT,
	UT_HIDABLE,
	UT_DOOR,
	UT_EVENTBOX,
	UT_JUMPOVER,
	UT_END
};

// Icon	Type
enum ICON_TYPE
{
	IT_NONE = UT_END,
	IT_BATTERY,
	IT_MEDICALKIT,
	IT_DAEMA,
	IT_LUNCHBOX,
	IT_TABLET,
	IT_KEY,
	IT_BIGICON,
	IT_DOCXICON,
	IT_KEYICON,
	IT_END
};

// Document Type
enum DOCUMENT_TYPE
{
	DT_PHONEMESSAGE = IT_END,
	DT_SILENTMESSAGE,
	DT_DOCXGENERATOR,
	DT_DOCXGONGJI,
	DT_MSGTH,
	DT_MSGPLANA,
	DT_DOCXTEST,
	DT_MSGBQ,
	DT_MSGPL,
	DT_END
};

// Key_Type
enum KEY_TYPE
{
	KT_MASTER = DT_END,
	KT_CENTER,
	KT_END
};

// Inventory State
enum INVENTORY_STATE
{
	INS_NORMAL,
	INS_CLICK,
	INS_MOUSEON,
	INS_DISABLE,
	INS_END
};

enum TRACE_STATE
{
	TS_IDLE,
	TS_TRACE,
	TS_MAX,
};

// Handycam State
enum HANDYCAM_STATE
{
	HS_NORMAL,
	HS_BATTERYOUT,
	HS_BATTERYCHANGE,
	HS_END
};

// DocxInven State
enum DOCXINVEN_STATE
{
	DS_NORMAL,
	DS_CLICK,
	DS_MOUSEON,
	DS_DISABLE,
	DS_END
};

enum USER_COLLIDER_ID
{
	UCI_NONE = PUN::CI_NONE,
	UCI_DOOR,
	UCI_PARKOUR,
	UCI_BED,
	UCI_LOCKER,
	UCI_PLAYER_HIT,
	UCI_PLAYER_INTERACT,
	UCI_PLAYER_RAY,
	UCI_ENEMY_ATTACK,
	UCI_ENEMY_INTERACT,
	UCI_ENEMY_HIT,
	UCI_ITEM_BATTERY,
	UCI_ITEM_DAMBAE,
	UCI_ITEM_LUNCHBOX,
	UCI_ITEM_TABLET,
	UCI_ITEM_FAK,
	UCI_ITEM_KEY,
	UCI_DOCUMENT,
	UCI_DUCT,
	UCI_SECTION,
	UCI_GENERATOR,
	UCI_BULLET,
	UCI_WALL,
	UCI_GRASS,
	UCI_ELEVATOR,
	UCI_END
};

enum USER_COLLIDER_DOCXID
{
	UCI_DOC_SILENT = UCI_END,
	UCI_DOC_GENERATOR,
	UCI_DOC_GONGJI,
	UCI_DOC_MESSAGETH,
	UCI_DOC_MESSAGEPLANA,
	UCI_DOC_MESSAGETEST,
	UCI_DOC_MESSAGEBQ,
	UCI_DOC_MESSAGEPL,
	UCI_DOC_END
};

enum DOOR_TYPE
{
	DOOR_NORMAL,
	DOOR_STAGE,
	DOOR_LOCKER,
	DOOR_HEAVY
};

enum FOOTSTEP_ENVIRONMENT
{
	FTSE_DEFAULT,
	FTSE_BLOOD,
	FTSE_CARPET,
	FTSE_CONCRETE,
	FTSE_CRUNCH,
	FTSE_DEEPWATER,
	FTSE_GRASS,
	FTSE_LADDERSQUEAK,
	FTSE_LADDER,
	FTSE_MATL,
	FTSE_MATH,
	FTSE_SCRAPESQUEEZE,
	FTSE_SMALLWATER,
	FTSE_THINWOOD,
	FTSE_WOOD,
	FTSE_WATER
};

enum HIDABLE_TYPE
{
	HT_NONE,
	HT_LOCKER,
	HT_BED
};