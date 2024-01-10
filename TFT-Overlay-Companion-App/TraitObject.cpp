#include <iostream>

#include "TraitObject.h"
#include "Utils.h"
#include "Offsets.h"

void TraitObject::LoadFromMem(DWORD64 baseAddr, HANDLE hProcess, const std::map<std::string, uint64_t> &offsets) {
	if (baseAddr == 0) return;

	DWORD64 traitObj = Memory::ReadAddress(hProcess, baseAddr + offsets.at("TraitObj"), "TraitObj");
	
	if (traitObj) {
		DWORD64 traitStrPtr = Memory::ReadAddress(hProcess, traitObj + offsets.at("TraitObjStr"), "TraitStringPtr");

		if (traitStrPtr) {
			int traitStrLength{};
			Memory::ReadStructure(hProcess, traitObj + offsets.at("TraitObjStrLength"), &traitStrLength, sizeof(int), "TraitStrLength");

			traitName = Memory::ReadString(hProcess, traitStrPtr, traitStrLength);
			Memory::ReadStructure(hProcess, baseAddr + offsets.at("TraitCount"), &count, sizeof(count), "TraitCount");
		}
	}
}
