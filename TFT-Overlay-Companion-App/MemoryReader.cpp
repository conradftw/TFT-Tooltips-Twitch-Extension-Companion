#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>

#include "MemoryReader.h"
#include "Utils.h"
#include <cpr/cpr.h>

MemoryReader::MemoryReader() {
	std::string offsetsUrl = "https://raw.githubusercontent.com/conradftw/TFT_Data/main/set10/memoryReader/offsets/latest.json";
	cpr::Response offsetsResponse = cpr::Get(cpr::Url{ offsetsUrl });

	if (offsetsResponse.status_code != 200) {
		std::cout << offsetsResponse.status_code << std::endl; 
		throw WinApiException("Could not fetch offsets from Github.");
	}

	nlohmann::json offsetsJson{ nlohmann::json::parse(offsetsResponse.text) };

	for (auto it = offsetsJson.begin(); it != offsetsJson.end(); ++it) {
		std::string offsetName = it.key();
		uint64_t offsetValue = std::stoull((std::string)it.value(), nullptr, 0);
	
		offsets.insert({ offsetName, offsetValue });
	}

	std::string whitelistUrl = "https://raw.githubusercontent.com/conradftw/TFT_Data/main/set10/memoryReader/whitelist/latest.json";
	cpr::Response whitelistResponse = cpr::Get(cpr::Url{ whitelistUrl });

	if (whitelistResponse.status_code != 200) {
		std::cout << whitelistResponse.status_code << std::endl;
		throw WinApiException("Could not fetch offsets from Github.");
	}

	nlohmann::json whitelistJson{ nlohmann::json::parse(whitelistResponse.text) };

	std::vector<std::string> whitelistArray = whitelistJson["whitelistedUnits"];

	for (const std::string& str : whitelistArray) {
		whitelistedNames.insert(str);
	}
}

void MemoryReader::HookToProcess() {
	// Find League window
	hWindow = FindWindowA("RiotWindowClass", NULL);
	if (hWindow == NULL) {
		throw WinApiException("League Window Not Found");
	}
	//std::cout << "hWindow is: " << hWindow << "\n";

	// Get League PID
	GetWindowThreadProcessId(hWindow, &pid);
	if (pid == NULL) {
		throw WinApiException("Unable to get League PID");
	}
	//std::cout << "PID is: " << pid << "\n";

	// Get League handle
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (hProcess == NULL) {
		throw WinApiException("Unable to get handle to League");
	}
	//std::cout << "hProcess is: " << hProcess << "\n";

	// Get League base address
	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		moduleBaseAddr = (DWORD_PTR)hMods[0];
	}
	else {
		throw WinApiException("Unable to get base address of League");
	}
	//std::cout << std::hex << "League Base Address is: " << moduleBaseAddr << "\n";
}

bool MemoryReader::IsHookedToProcess() {
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD64 event = WaitForSingleObject(process, 0);

	CloseHandle(process);

	return event == WAIT_TIMEOUT;
}

bool MemoryReader::hasGameStarted() {
	DWORD64 timerAddr = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("TFTGameTime"), "TFT Timer");

	if (timerAddr != NULL) {
		DWORD64 timerAddr1 = Memory::ReadAddress(hProcess, timerAddr + offsets.at("TFTGameTime1"), "TFT Timer1");
		
		if (timerAddr1 != NULL) {
			float gameTime{};
			Memory::ReadStructure(hProcess, timerAddr1 + offsets.at("TFTGameTime2"), &gameTime, sizeof(gameTime), "GameTime");
			gameStartedFlag = gameTime > 0.2f;
		}
	}

	return gameStartedFlag;
}

void MemoryReader::MakeSnapshot(MemSnapshot& snapshot) {
	if (!moduleBaseAddr) return;

	if (hasGameStarted()) {
		ReadRenderer(snapshot);
		ReadCamera(snapshot);
		ReadStageAndRound(snapshot);
		ReadMinions(snapshot);
		ReadTraits(snapshot);
		ReadHudShop(snapshot);
	}
}

void MemoryReader::ReadRenderer(MemSnapshot& snapshot) {
	if (moduleBaseAddr == 0) return;

	DWORD64 rendererAddr = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("Renderer"), "Renderer");

	if (rendererAddr == 0) return;
	
	snapshot.renderer->LoadFromMem(rendererAddr, offsets, moduleBaseAddr, hProcess);
}

void MemoryReader::ReadCamera(MemSnapshot& snapshot) {
	if (moduleBaseAddr == 0) return;

	DWORD64 hudInstanceAddr = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("HudInstance"), "HUD_Instance");

	if (hudInstanceAddr == 0) return;

	DWORD64 camera = Memory::ReadAddress(hProcess, hudInstanceAddr + offsets.at("HudInstance_Camera"), "HUD_Instance Camera");

	if (camera == 0) return;

	Vector2 cameraPos{};
	Memory::ReadStructure(hProcess, camera + offsets.at("HudInstance_CameraX"), &cameraPos.x, sizeof(cameraPos.x), "Camera X");
	Memory::ReadStructure(hProcess, camera + offsets.at("HudInstance_CameraY"), &cameraPos.y, sizeof(cameraPos.y), "Camera Y");

	float camDirection{};

	Memory::ReadStructure(hProcess, camera + offsets.at("HudInstance_Camera_Yaw"), &camDirection, sizeof(camDirection), "Camera Direction");
	snapshot.cameraDirection = std::abs(camDirection);

	snapshot.currentBoard = Board::GetSquare(Board::GetRow(cameraPos.y), Board::GetColumn(cameraPos.x));
}

void MemoryReader::ReadStageAndRound(MemSnapshot& snapshot) {
	if (moduleBaseAddr == 0) return;

	DWORD64 stageStrBase = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("StageStr"), "StageStrBase");

	if (!stageStrBase) return;

	if (stageStrBase) {
		DWORD64 stageStr1 = Memory::ReadAddress(hProcess, stageStrBase + offsets.at("StageStr1"), "StageStr1");

		if (stageStr1) {
			DWORD64 stageStr2 = Memory::ReadAddress(hProcess, stageStr1 + offsets.at("StageStr2"), "StageStr2");

			if (stageStr2) {
				DWORD64 stageStr3 = Memory::ReadAddress(hProcess, stageStr2 + offsets.at("StageStr3"), "StageStr3");

				if (stageStr3) {
					std::string longestStageString = "99-99"; 
					std::string stageStr = Memory::ReadString(hProcess, stageStr3 + offsets.at("StageStr4"), longestStageString.length());

					std::string delimiter = "-"; 
					size_t pos = stageStr.find(delimiter);

					if (pos != std::string::npos && pos + 1 < stageStr.length()) {
						std::string stage = stageStr.substr(0, pos);
						std::string round = stageStr.substr(pos + 1);

						snapshot.stage = std::stoi(stage);
						snapshot.round = std::stoi(round);
					}
				}
			}
		}
	}
}

std::string MemoryReader::ReadMinionName(DWORD64 minionAddr) {
	if (minionAddr == 0) return "ERROR";

	std::string minionName;
	int nameSize{};
	Memory::ReadStructure(hProcess, minionAddr + offsets.at("ObjName1") + offsets.at("LolStrLength"), &nameSize, sizeof(nameSize), "Minion Name Size");

	char nameBuffer[32];

	if (nameSize > 15) {
		DWORD64 namePtr = Memory::ReadAddress(hProcess, minionAddr + offsets.at("ObjName1"), "Minion Name Pointer");

		if (namePtr == 0) return "ERROR";

		//Memory::ReadStructure(hProcess, namePtr, nameBuffer, sizeof(nameBuffer), "Minion Name, more than 15 chars");
		minionName = Memory::ReadString(hProcess, namePtr, nameSize);
	}
	else {
		//Memory::ReadStructure(hProcess, minionAddr + offsets.at("ObjName1"), nameBuffer, sizeof(nameBuffer), "Minion Name, less than 15 chars");
		minionName = Memory::ReadString(hProcess, minionAddr + offsets.at("ObjName1"), nameSize);
	}
	//return std::string(nameBuffer);
	return minionName;
}

Vector3 MemoryReader::GetMinionPosition(DWORD64 minionAddr) {
	Vector3 minionPos;
	Memory::ReadStructure(hProcess, minionAddr + offsets.at("ObjPos"), &minionPos, sizeof(minionPos), "Minion Position");

	return minionPos;
}

void MemoryReader::ReadMinions(MemSnapshot& snapshot) {
	if (moduleBaseAddr == 0) return;

	snapshot.units.clear();
	snapshot.items.clear();

	if (snapshot.currentBoard == -1) return;

	DWORD64 minionListAddr = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("MinionList"), "pMinionList");
	DWORD64 minionList = Memory::ReadAddress(hProcess, minionListAddr + offsets.at("pList"), "MinionList");

	if (minionListAddr == 0 || minionList == 0) return;

	unsigned int minionListSize{};
	Memory::ReadStructure(hProcess, minionListAddr + offsets.at("ListSize"), &minionListSize, sizeof(minionListSize), "MinionList Size");

	for (unsigned int i = 0; i < minionListSize; i++) {
		DWORD64 minionAddr = Memory::ReadAddress(hProcess, minionList + (i * sizeof(DWORD64)), "Minion");

		if (minionAddr == 0) return;

		std::shared_ptr<GameObject> minionObj = std::shared_ptr<GameObject>(new GameObject());
		
		std::string minionName = MemoryReader::ReadMinionName(minionAddr);

		Vector3 minionPos = MemoryReader::GetMinionPosition(minionAddr);
		int board = Board::GetSquare(Board::GetRow(minionPos.z), Board::GetColumn(minionPos.x));

		if (whitelistedNames.find(minionName) != whitelistedNames.end() && board == snapshot.currentBoard ) {
			minionObj->name = minionName;
			minionObj->worldPosition = minionPos;

			minionObj->LoadFromMem(minionAddr, hProcess, offsets);

			float starLevelSizeMultipliers[] = { 1.f, 1.1f, 1.2f, 1.2f };
			float boundingBoxMultiplier = minionObj->scale * starLevelSizeMultipliers[minionObj->starLevel - 1];
			float boundingBoxDefaultWidth = 65.f;

			float topLeftCornerX = minionObj->worldPosition.x - (boundingBoxDefaultWidth * std::cos(snapshot.cameraDirection) * boundingBoxMultiplier);
			float topLeftCornerY = minionObj->worldPosition.y + (minionObj->hpBarHeight * boundingBoxMultiplier);
			float topLeftCornerZ = minionObj->worldPosition.z + (boundingBoxDefaultWidth * std::cos(snapshot.cameraDirection) * boundingBoxMultiplier);

			float bottomRightCornerX = minionObj->worldPosition.x + (boundingBoxDefaultWidth * std::cos(snapshot.cameraDirection) * boundingBoxMultiplier);
			float bottomRightCornerY = minionObj->worldPosition.y;
			float bottomRightCornerZ = minionObj->worldPosition.z - (boundingBoxDefaultWidth * std::sin(snapshot.cameraDirection) * boundingBoxMultiplier);

			Vector2 topLeftCorner = snapshot.renderer->WorldToScreen({ topLeftCornerX, topLeftCornerY, topLeftCornerZ });
			Vector2 bottomRightCorner = snapshot.renderer->WorldToScreen({ bottomRightCornerX, bottomRightCornerY, bottomRightCornerZ });

			minionObj->box = BoundingBox(topLeftCorner, bottomRightCorner);

			snapshot.units.push_back(minionObj);
		}
	}
}

void MemoryReader::ReadTraits(MemSnapshot& snapshot) {
	if (moduleBaseAddr == 0) return;

	snapshot.traits.clear();
	unsigned int MAX_TRAITS_SHOWN = 9;

	DWORD64 traitsBaseAddr = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("TraitsList"), "TraitsBase");
	
	if (traitsBaseAddr) {
		DWORD64 traitsList = Memory::ReadAddress(hProcess, traitsBaseAddr + offsets.at("TraitsList1"), "TraitsList");
		
		if (traitsList) {
			for (unsigned int i = 0; i < MAX_TRAITS_SHOWN; i++) {
				DWORD64 traitAddr = Memory::ReadAddress(hProcess, traitsList + (DWORD64)(i * 0x8), "Trait");

				if (traitAddr == 0) return;

				std::shared_ptr<TraitObject> traitObj = std::shared_ptr<TraitObject>(new TraitObject());
				traitObj->LoadFromMem(traitAddr, hProcess, offsets);

				if (traitObj->count) {
					snapshot.traits.push_back(traitObj);
				}
			}
		}
	}
}


void MemoryReader::ReadHudShop(MemSnapshot& snapshot) {
	if (moduleBaseAddr == 0) return;

	snapshot.shopUnits.clear();

	DWORD64 shopBaseAddr = Memory::ReadAddress(hProcess, moduleBaseAddr + offsets.at("HudShopBase"), "ShopBase");

	if (shopBaseAddr == 0) return;

	DWORD64 shop1 = Memory::ReadAddress(hProcess, shopBaseAddr + offsets.at("HudShopBase1"), "ShopBase1");

	if (shop1 == 0) return;

	DWORD64 shopList = Memory::ReadAddress(hProcess, shop1 + offsets.at("HudShopBase2"), "ShopBase2");

	if (shopList == 0) return;

	unsigned int SHOP_SLOTS = 5;
	for (unsigned int i = 0; i < SHOP_SLOTS; i++) {
		DWORD64 shopUnit = Memory::ReadAddress(hProcess, shopList + offsets.at("HudShopFirstUnit") + (DWORD64)(i * offsets.at("HudShopUnitSpacing")), "ShopUnit");

		if (shopUnit) {
			std::shared_ptr<ShopUnitObject> shopUnitObj = std::shared_ptr<ShopUnitObject>(new ShopUnitObject());
			unsigned int unitNameLength;
			Memory::ReadStructure(hProcess, shopUnit + offsets.at("HudShopUnitNameLength"), &unitNameLength, sizeof(unitNameLength), "UnitNameLength");

			DWORD64 shopNameAddress = Memory::ReadAddress(hProcess, shopUnit + offsets.at("HudShopUnitName"), "ShopNameAddress");

			if (shopNameAddress) {
				shopUnitObj->unitName = Memory::ReadString(hProcess, shopNameAddress, unitNameLength);
				Memory::ReadStructure(hProcess, shopList + offsets.at("HudShopFirstUnit") + (DWORD64)(i * offsets.at("HudShopUnitSpacing")) + offsets.at("HudShopUnitCost"), &shopUnitObj->cost, sizeof(shopUnitObj->cost), "ShopUnitCost");

				snapshot.shopUnits.push_back(shopUnitObj);
			}
		}
	}
}