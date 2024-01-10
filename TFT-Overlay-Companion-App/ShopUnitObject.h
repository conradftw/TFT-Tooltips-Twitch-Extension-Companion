#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

class ShopUnitObject {
public:
	void LoadFromMem(DWORD64 baseAddr, HANDLE hProcess);
	friend void to_json(nlohmann::json& j, const ShopUnitObject& obj) {
		j = nlohmann::json{ {"unitName", obj.unitName}, {"cost", obj.cost} };
	}

public:
	std::string unitName;
	int cost;
	int rarity;
};

