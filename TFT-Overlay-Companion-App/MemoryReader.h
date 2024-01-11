#pragma once

#include "Windows.h"
#include <string>
#include <set>
#include <nlohmann/json.hpp>
#include "MemSnapshot.h"

class MemoryReader {
	
public:
	MemoryReader();

	void HookToProcess();

	bool IsHookedToProcess();

	void MakeSnapshot(MemSnapshot& snapshot);

	bool gameStartedFlag = false;

private: 
	HWND hWindow = NULL;

	DWORD pid = 0;
	
	HANDLE hProcess = NULL;

	DWORD_PTR moduleBaseAddr = 0;

private:
	//std::set<int> blacklistedObjects;
	std::set<std::string> skippedNames;

	std::map<std::string, uint64_t> offsets;
	std::set<std::string> whitelistedNames;

	std::ofstream seenOutputFile;

	void ReadStageAndRound(MemSnapshot& snapshot);
	void ReadRenderer(MemSnapshot& snapshot);
	void ReadCamera(MemSnapshot& snapshot);
	void ReadPlayers(MemSnapshot& snapshot);	// dont know how to get unit owner so idk if this is useful for now
	void ReadMinions(MemSnapshot& snapshot);	// minions include: units, items, augment, tomes, anvils 
	void ReadUnits(MemSnapshot& snapshot);
	void ReadBuffs(MemSnapshot& snapshot);
	void ReadBaseStats(MemSnapshot& snapshot);	// should be called in ReadUnits
	void ReadTraits(MemSnapshot& snapshot);

	std::string ReadMinionName(DWORD64 minionAddr);
	Vector3 GetMinionPosition(DWORD64 minionAddr);
	BoundingBox GetBoundingBox(MemSnapshot& snapshot, Vector3 minionPos);

	//void ReadAugments();

	bool hasGameStarted();

	void ReadHudShop(MemSnapshot& snapshot);

};