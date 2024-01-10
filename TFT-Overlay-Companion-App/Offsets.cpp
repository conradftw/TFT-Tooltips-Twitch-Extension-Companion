#include "Offsets.h"

// Offsets for 13.24b
Offsets::Offsets() {};

int Offsets::LocalPlayer = 0x223A0B8;		// 13.24b	48 8b 15 ? ? ? ? 48 85 d2 74 ? 8b 4b
int Offsets::LolGameTime = 0x2226D48;		// 13.24b	f3 0f 5c 35 ? ? ? ? 0f 28 f8
int Offsets::AttackableUnitManager = 0x221C660;	// 13.24b	48 8b 0d ? ? ? ? 44 0f 28 54 24
int Offsets::ViewProjMatrices = 0x2284710;	// 13.24b	48 8D 0D ? ? ? ? 0F 10 00
int Offsets::ZoomInstance = 0x2221C28;		// 13.24b	48 8b 05 ? ? ? ? c3 cc cc cc cc cc cc cc cc 48 89 5c 24 ? 57 48 83 ec ? 48 8b d9
int Offsets::ZoomInstance_MaxZoom = 0x28;	// 13.24b	look for 2250.0 float

// HUD Instance
int Offsets::HudInstance = 0x221C510;			// 13.24b	48 8b 0d ? ? ? ? 0f 29 7c 24 ? 48 8b 51
int Offsets::HudInstance_Camera = 0x18;			// 13.24b	
int Offsets::HudInstance_CameraX = 0x10;		// 13.24b
int Offsets::HudInstance_CameraY = 0x18;		// 13.24b

int Offsets::HudInstance_Camera_Roll = 0x1AC;	// 13.24b
int Offsets::HudInstance_Camera_Yaw = 0x1B0;	// 13.24b
int Offsets::HudInstance_Camera_Pitch = 0x1B4;	// 13.24b

int Offsets::HudInstance_CameraZoom = 0x2C0;	// 13.24b	float 2250 usually

// Renderer
int Offsets::Renderer = 0x228D1C0;			// 13.24b	48 83 3d ? ? ? ? ? 0f 84 ? ? ? ? 48 8b 4e
int Offsets::RendererWidth = 0xC;			// 13.24b	floats for resolution
int Offsets::RendererHeight = 0x10;			// 13.24b	floats for resolution

// <TYPE>Managers and <TYPE>Lists
int Offsets::ObjectManager = 0x21EFC00;		// 13.22	48 8b 0d ? ? ? ? e8 ? ? ? ? 48 8b d8 48 85 c0 75 ? 48 83 c4 ? 5b c3 48 8b 00 48 8b cb ff 50 ? 4c 8d 05 ? ? ? ? 48 8b d3 48 8b c8 48 83 c4 ? 5b e9 ? ? ? ? cc
int Offsets::AiManager = 0x36F0;			// 13.19
int Offsets::HeroList = 0x221C620;			// 13.24b	48 8B 05 ? ? ? ? 45 33 E4 0F 57 C0
int Offsets::MinionList = 0x221F590;		// 13.24b	48 8B 0D ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B C8
int Offsets::pList = 0x8;
int Offsets::ListSize = 0x10;;

// GameObject fields
int Offsets::ObjIndex = 0x10;				// 13.24b
int Offsets::ObjTeam = 0x3C;				// 13.24b
int Offsets::ObjName1 = 0x38A8;				// 13.24b
int Offsets::ObjName2 = 0x60;				// 13.24b  can be a pointer to string now with name change update
int Offsets::ObjName3 = 0x3888;				// 13.24b
int Offsets::ObjLvl = 0x4048;				// 13.24b
int Offsets::ObjPos = 0x220;				// 13.24b
int Offsets::ObjHealth = 0x1088;			// 13.24b
int Offsets::ObjMaxHealth = 0x10A0;			// 13.24b
int Offsets::ObjMana = 0x370;				// 13.24b
int Offsets::ObjMaxMana = 0x388;			// 13.24b
int Offsets::ObjScale = 0x16CC;				// 13.24b
int Offsets::ObjAttackDamage = 0x16B4;				// 13.24b		// same as base in TFT i think
int Offsets::ObjBonusAttackDamage = 0x1620;				// 13.24b
int Offsets::ObjBonusAttackDamagePercentage = 0x1624;				// 13.22	// use this to multiply the base attack to get the display attack dmg in tft
int Offsets::ObjAbilityPower = 0x1630;				// 13.24b		// default 100 ap
int Offsets::ObjBonusAbilityPower = 0x0;				// 13.21b
int Offsets::ObjCritChance = 0x16D8;				// 13.24b		// crit chance looks like its also 16D8 in tft
int Offsets::ObjCritDamage = 0x16C8;				// 13.24b
int Offsets::ObjBonusAttackSpeed = 0x1684;				// 13.24b
int Offsets::ObjArmor = 0x16DC;					// 13.24b
int Offsets::ObjBonusArmor = 0x16E0;				// 13.24b
int Offsets::ObjMagicResist = 0x16E4;				// 13.24b
int Offsets::ObjBonusMagicResist = 0x16E8;				// 13.24b
int Offsets::ObjCurrentRange = 0x16FC;				// 13.24b

// LoL Strings
int Offsets::LolStrLength = 0x10;
int Offsets::LolStrMaxLength = 0x18;

// Buffs
int Offsets::BuffManager = 0x2810;		// 13.24b add this to localplayer, do not dereference
int Offsets::BuffArrayStart = 0x0;
int Offsets::BuffArrayEnd = 0x8;

// UnitInfo
int Offsets::CharData = 0x35E0; 		// 13.24b pointer to heap
int Offsets::CharDataData = 0x28;		// 13.24b another pointer to heap
int Offsets::HPBarHeight = 0xC4;		// 13.24b
int Offsets::BaseHealth = 0xB0;			// 13.24b
int Offsets::BaseMana = 0x11C;			// 13.24b
int Offsets::BaseAttackDamage = 0x218;		// 13.24b
int Offsets::BaseArmor = 0x220;		// 13.24b
int Offsets::BaseMagicResist = 0x228;		// 13.24b
int Offsets::BaseAttackSpeed1 = 0x254;		// 13.24b
int Offsets::BaseAttackSpeed2 = 0x258;		// 13.24b
int Offsets::BaseRange = 0x250;		// 13.24b

// TFT
int Offsets::ShopBase = 0x2250CC8;		// 13.24b	48 8b 0d ? ? ? ? e8 ? ? ? ? 48 8b 0d ? ? ? ? e8 ? ? ? ? 48 8b 0d ? ? ? ? e8 ? ? ? ? 48 8b 0d ? ? ? ? 48 8b 35
//			48 8b 0d ? ? ? ? e8 ? ? ? ? 48 85 c0 74 ? 48 8b 08
int Offsets::ShopBase1 = 0x10;
int Offsets::ShopBase2 = 0x0;
int Offsets::ShopBase3 = 0x18;				// ShopList starts here
int Offsets::ShopBase4_RollCost = 0x214;
int Offsets::ShopUnit = 0x0;
int Offsets::ShopUnitCost = 0xC;			// if unit is bought, cost becomes 0
int Offsets::ShopUnitName = 0x8;			// pointer to shop unit name; if unit is bought, name is "Sold"
int Offsets::ShopUnitNameLength = 0x10;			// Length of name
int Offsets::ShopUnitSpacing = 0x68;		// Every 0x68 is a new pointer to a shop obj

//THIS ONE WORKS IN SPECTATOR MODE!!!
int Offsets::HudShopBase = 0x2250D08;	// 13.24b	48 8b 05 ? ? ? ? 33 f6 48 89 9c 24
int Offsets::HudShopBase1 = 0x2B8;
int Offsets::HudShopBase2 = 0xEF8;
int Offsets::HudShopFirstUnit = 0x308;
int Offsets::HudShopUnitCost = 0x14;			// if unit is bought, cost becomes 0, do not deref, add to addr of unit
int Offsets::HudShopUnitName = 0x8;			// pointer to shop unit name; if unit is bought, name is "Sold"
int Offsets::HudShopUnitNameLength = 0x10;			// Length of name
int Offsets::HudShopUnitSpacing = 0x350;		// Every 0x68 is a new pointer to a shop obj

int Offsets::PhaseStr = 0x2291F08;		// 13.24b	48 8b 15 ? ? ? ? eb ? 48 81 ff
int Offsets::PhaseStr1 = 0x48;
int Offsets::PhaseStr2 = 0xA0;
int Offsets::PhaseStr3 = 0x0;
int Offsets::PhaseStr4 = 0xA8;
int Offsets::PhaseStr5 = 0x0;

int Offsets::isCombatPhase = 0x22458D0;		// 13.22
int Offsets::isCombatPhase1 = 0x78;		// 13.22
int Offsets::isCombatPhase2 = 0x28C;		// Combat = 1, Planning = 0

int Offsets::StageStr = 0x2250D10;		// 13.24b	48 8b 0d ? ? ? ? 48 8d 54 24 ? e8 ? ? ? ? f3 0f 10 54 24
int Offsets::StageStr1 = 0x330;
int Offsets::StageStr2 = 0x0;
int Offsets::StageStr3 = 0xA8;
int Offsets::StageStr4 = 0x0;

int Offsets::TraitsList = 0x2250D28;	// 13.24b 	48 8b 1d ? ? ? ? e8 ? ? ? ? 0f b6 d0
int Offsets::TraitsList1 = 0xA0;
int Offsets::TraitObj = 0x138;
int Offsets::TraitObjStr = 0x8;
int Offsets::TraitObjStrLength = 0x10;
int Offsets::TraitCount = 0x140;

int Offsets::TFTGameTime = 0x2241A48;	// 13.24b	48 8b 05 ? ? ? ? 48 8b 40 ? 48 85 c0 74 ? 0f b6 00 88 02 48 8b c2 c3 48 8b 05 ? ? ? ? 0f b6 88
int Offsets::TFTGameTime1 = 0x28;
int Offsets::TFTGameTime2 = 0x3C;
