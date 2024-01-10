#include <iostream>

#include "GameObject.h"
#include "Offsets.h"

void GameObject::LoadFromMem(DWORD64 baseAddr, HANDLE hProcess, const std::map<std::string, uint64_t>& offsets) {
	if (baseAddr == 0) return;

	char buffer[0x4000];
	Memory::ReadStructure(hProcess, baseAddr, buffer, sizeof(buffer), "GameObject buffer");

	memcpy(&networkId, &buffer[offsets.at("ObjIndex")], sizeof(networkId));

	memcpy(&health, &buffer[offsets.at("ObjHealth")], sizeof(health));	// health doesn't go to 0 when a unit dies hmmmm
	memcpy(&totalHealth, &buffer[offsets.at("ObjMaxHealth")], sizeof(totalHealth));
	memcpy(&mana, &buffer[offsets.at("ObjMana")], sizeof(mana));
	memcpy(&maxMana, &buffer[offsets.at("ObjMaxMana")], sizeof(maxMana));
	memcpy(&scale, &buffer[offsets.at("ObjScale")], sizeof(scale));
	memcpy(&currentAttackRange, &buffer[offsets.at("ObjCurrentRange")], sizeof(currentAttackRange));
	memcpy(&bonusAttackDamagePercent, &buffer[offsets.at("ObjBonusAttackDamagePercentage")], sizeof(bonusAttackDamagePercent));
	memcpy(&bonusAbilityPower, &buffer[offsets.at("ObjAbilityPower")], sizeof(bonusAbilityPower));
	memcpy(&totalArmor, &buffer[offsets.at("ObjArmor")], sizeof(totalArmor));
	memcpy(&bonusArmor, &buffer[offsets.at("ObjBonusArmor")], sizeof(bonusArmor));
	memcpy(&totalMagicResist, &buffer[offsets.at("ObjMagicResist")], sizeof(totalMagicResist));
	memcpy(&bonusMagicResist, &buffer[offsets.at("ObjBonusMagicResist")], sizeof(bonusMagicResist));
	memcpy(&bonusAttackSpeed, &buffer[offsets.at("ObjBonusAttackSpeed")], sizeof(bonusAttackSpeed));
	memcpy(&totalCritChance, &buffer[offsets.at("ObjCritChance")], sizeof(totalCritChance));
	memcpy(&totalCritDamage, &buffer[offsets.at("ObjCritDamage")], sizeof(totalCritDamage));

	DWORD64 charDataBaseAddr{};
	memcpy(&charDataBaseAddr, &buffer[offsets.at("CharData")], sizeof(charDataBaseAddr));

	DWORD64 charDataDataBaseAddr = Memory::ReadAddress(hProcess, charDataBaseAddr + offsets.at("CharDataData"), "CharDataData");
	if (charDataDataBaseAddr == 0) return;

	DWORD64 buffStartPtr{};
	DWORD64 buffEndPtr{};
	memcpy(&buffStartPtr, &buffer[offsets.at("BuffManager") + offsets.at("BuffArrayStart")], sizeof(buffStartPtr));
	memcpy(&buffEndPtr, &buffer[offsets.at("BuffManager") + offsets.at("BuffArrayEnd")], sizeof(buffEndPtr));

	char charDataBuff[0x300];
	Memory::ReadStructure(hProcess, charDataDataBaseAddr, charDataBuff, sizeof(charDataBuff), "Reading CharData");

	memcpy(&hpBarHeight, &charDataBuff[offsets.at("HPBarHeight")], sizeof(hpBarHeight));
	memcpy(&baseHealth, &charDataBuff[offsets.at("BaseHealth")], sizeof(baseHealth));
	memcpy(&baseMana, &charDataBuff[offsets.at("BaseMana")], sizeof(baseMana));
	memcpy(&baseAttackDamage, &charDataBuff[offsets.at("BaseAttackDamage")], sizeof(baseAttackDamage));
	memcpy(&baseArmor, &charDataBuff[offsets.at("BaseArmor")], sizeof(baseArmor));
	memcpy(&baseMagicResist, &charDataBuff[offsets.at("BaseMagicResist")], sizeof(baseMagicResist));
	memcpy(&baseAttackSpeed, &charDataBuff[offsets.at("BaseAttackSpeed")], sizeof(baseAttackSpeed));

	const std::uintptr_t buffArrayStart = buffStartPtr;
	const std::uintptr_t buffArrayEnd = buffEndPtr;

	if (buffStartPtr != NULL && buffEndPtr != NULL) {
		for (std::uintptr_t buffPtr = buffArrayStart; buffPtr < buffArrayEnd; buffPtr += 0x10) {	// couldn't this be a problem?
			DWORD64 buffObj = Memory::ReadAddress(hProcess, buffPtr, "BuffObj");
			if (buffObj) {
				DWORD64 buffNamePointer1 = Memory::ReadAddress(hProcess, buffObj + 0x10, "buffNamePointer1");
				if (buffNamePointer1) {
					DWORD64 buffNamePointer2 = Memory::ReadAddress(hProcess, buffNamePointer1 + 0x8, "buffNamePointer2");

					if (buffNamePointer2) {
						char buffName[128];
						Memory::ReadStructure(hProcess, buffNamePointer2, buffName, sizeof(buffName), "Buff Name");
						buffName[127] = '\0';

						std::string buffStr(buffName);

						if (buffStr == "tft_unitbuff_star1" || buffStr == "tft_unitbuff_star2" || buffStr == "tft_unitbuff_star3") {
							starLevel = buffStr.back() - '0';
							break;
						}
					}
				}
			}
		}
	}
}
