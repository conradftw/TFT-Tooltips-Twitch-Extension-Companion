#pragma once
#pragma once

// Offsets for 13.22
class Offsets {
public:
	Offsets();

	static int LocalPlayer;
	static int LolGameTime;
	static int AttackableUnitManager;
	static int ViewProjMatrices;
	static int ZoomInstance;
	static int ZoomInstance_MaxZoom;
	static int ObjUnderMouse;

	// HUD Instance
	static int HudInstance;
	static int HudInstance_Camera;
	static int HudInstance_CameraX;
	static int HudInstance_CameraY;

	static int HudInstance_Camera_Roll;
	static int HudInstance_Camera_Yaw;
	static int HudInstance_Camera_Pitch;

	static int HudInstance_CameraZoom;

	// Minimap
	static int Minimap;
	static int MinimapInfo1;
	static int MinimapInfo2;
	static int MinimapInfo3;
	static int MinimapPos;
	static int MinimapSize;

	// Renderer
	static int Renderer;
	static int RendererWidth;
	static int RendererHeight;

	// <TYPE>Managers and <TYPE>Lists
	static int ObjectManager;
	static int AiManager;
	static int HeroList;
	static int MinionList;
	static int pList;
	static int ListSize;

	// GameObject fields
	static int ObjIndex;
	static int ObjTeam;
	static int ObjName1;
	static int ObjName2;
	static int ObjName3;
	static int ObjLvl;
	static int ObjPos;
	static int ObjHealth;
	static int ObjMaxHealth;
	static int ObjMana;
	static int ObjMaxMana;
	static int ObjScale;
	static int ObjAttackDamage;
	static int ObjBonusAttackDamage;
	static int ObjBonusAttackDamagePercentage;
	static int ObjAbilityPower;
	static int ObjBonusAbilityPower;
	static int ObjCritChance;
	static int ObjCritDamage;
	static int ObjBonusAttackSpeed;
	static int ObjCurrentRange;

	static int ObjArmor;
	static int ObjBonusArmor;
	static int ObjMagicResist;
	static int ObjBonusMagicResist;

	// LoL Strings
	static int LolStrLength;
	static int LolStrMaxLength;

	// Buffs
	static int BuffManager;
	static int BuffArrayStart;
	static int BuffArrayEnd;

	// UnitInfo
	static int CharData;
	static int CharDataData;
	static int HPBarHeight;
	static int BaseHealth;
	static int BaseMana;
	static int BaseAttackDamage;
	static int BaseArmor;
	static int BaseMagicResist;
	static int BaseAttackSpeed1;
	static int BaseAttackSpeed2;
	static int BaseRange;

	// TFT
	static int ShopBase;
	static int ShopBase1;
	static int ShopBase2;
	static int ShopBase3;
	static int ShopBase4_RollCost;

	static int ShopUnit;
	static int ShopUnitCost;
	static int ShopUnitName;
	static int ShopUnitNameLength;

	static int ShopUnitSpacing;

	// HUD Shop
	static int HudShopBase;
	static int HudShopBase1;
	static int HudShopBase2;
	static int HudShopFirstUnit;
	static int HudShopUnitCost;
	static int HudShopUnitName;
	static int HudShopUnitNameLength;
	static int HudShopUnitSpacing;

	static int PhaseStr;				// Planning, Combat, .combat +48 +A0 +0 +A8 +0
	static int PhaseStr1;
	static int PhaseStr2;
	static int PhaseStr3;
	static int PhaseStr4;
	static int PhaseStr5;

	static int isCombatPhase;		// 13.22
	static int isCombatPhase1;		// 13.22
	static int isCombatPhase2;		// Combat = 1, Planning = 0

	static int StageStr;				// eg: 2-1, 4-2, etc roundD = [0x330,0x0,0xA8,0x0]
	static int StageStr1;
	static int StageStr2;
	static int StageStr3;
	static int StageStr4;

	static int TraitsList;
	static int TraitsList1;
	static int TraitObj;
	static int TraitObjStr;
	static int TraitObjStrLength;
	static int TraitCount;

	static int TFTGameTime;
	static int TFTGameTime1;
	static int TFTGameTime2;
};