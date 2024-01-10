#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

class TraitObject {
public:
	std::string traitName;
	int count = 0;

public:
	void LoadFromMem(DWORD64 baseAddr, HANDLE hProcess, const std::map<std::string, uint64_t>& offsets);
	friend void to_json(nlohmann::json& j, const TraitObject& obj) {
		j = nlohmann::json{ {"traitName", obj.traitName}, {"count", obj.count} };
	}
	//void to_json(nlohmann::json& j, const TraitObject& obj);
};