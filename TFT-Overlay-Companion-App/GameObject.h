#pragma once
#include <string>
#include "Utils.h"
#include <nlohmann/json.hpp>

// just try to read this for now, then add TraitObject and ShopUnitObject
class GameObject {

public:	
	void LoadFromMem(DWORD64 baseAddr, HANDLE hProcess, const std::map<std::string, uint64_t>& offsets);
	void ReadBaseStats(DWORD64 CharDataBaseAddr, HANDLE hProcess);
	void ReadBuffs(DWORD64 buffBaseAddr, HANDLE hProcess);

	friend void to_json(nlohmann::json& j, const GameObject& obj) {
		j = nlohmann::json{ 
			{"networkId", obj.networkId},
			{"name", obj.name},
			{"position", obj.worldPosition},
			{"bounding_box", obj.box},

			{"starLevel", obj.starLevel},
			{"currentAttackRange", obj.currentAttackRange},

			{"currentHealth", obj.health},
			{"totalHealth", obj.totalHealth},

			{"currentMana", obj.mana},
			{"totalMana", obj.maxMana},

			{"totalAttackDamage", obj.totalAttackDamage},
			{"bonusAttackDamagePercent", obj.bonusAttackDamagePercent},

			{"bonusAbilityPower", obj.bonusAbilityPower},

			{"totalArmor", obj.totalArmor},
			{"bonusArmor", obj.bonusArmor},

			{"totalMagicResist", obj.totalMagicResist},
			{"bonusMagicResist", obj.bonusMagicResist},

			{"bonusAttackSpeed", obj.bonusAttackSpeed},

			{"totalCritChance", obj.totalCritChance},

			{"totalCritDamage", obj.totalCritDamage},

			{"baseHealth", obj.baseHealth},
			{"baseMana", obj.baseMana},
			{"baseAttackDamage", obj.baseAttackDamage},
			{"baseArmor", obj.baseArmor},
			{"baseMagicResist", obj.baseMagicResist},
			{"baseAttackSpeed", obj.baseAttackSpeed},
		};
	}

public:
	DWORD64 address;	// for debug
	short networkId;
	//std::string fullName;
	std::string name;

	Vector3 worldPosition;
	int board;
	BoundingBox box;

	float health;
	float totalHealth;
	float mana;
	float maxMana;

	float scale;
	// stat box
	float currentAttackRange;

	float totalAttackDamage;
	float bonusAttackDamagePercent;

	float totalAbilityPower;
	float bonusAbilityPower;

	float totalArmor;
	float bonusArmor;

	float totalMagicResist;
	float bonusMagicResist;

	float totalAttackSpeed;
	float bonusAttackSpeed;

	float totalCritChance;
	float bonusCritChance;

	float totalCritDamage;
	float bonusCritDamage;

	// base data
	float hpBarHeight;
	float baseHealth;
	float baseMana;
	float baseAttackDamage;
	float baseAbilityPower = 100;
	float baseArmor;
	float baseMagicResist;
	float baseAttackSpeed;
	float baseGameplayRadius;	// multiply this with scale to box width

	// tft
	int cost;
	int starLevel;
};