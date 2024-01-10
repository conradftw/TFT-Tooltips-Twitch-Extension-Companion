#pragma once

#include <set>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

#include "GameObject.h"
#include "Renderer.h"
#include "TraitObject.h"
#include "ShopUnitObject.h"

/*
	Struct representing the gamestate, which currently includes:
	1. Renderer: used to access ViewProjMatrix and WorldToScreen function
	2. Camera: determines what the current screen of the player is
*/

struct MemSnapshot {
	std::vector<std::shared_ptr<GameObject>> units;	// alive units on current screen (bench + board for both players)
	std::vector<std::shared_ptr<GameObject>> items;	// TFTItem objects (everything that can go on the item bench)

	std::vector<std::shared_ptr<TraitObject>> traits;
	std::vector<std::shared_ptr<ShopUnitObject>> shopUnits;
	std::unique_ptr<Renderer> renderer = std::unique_ptr<Renderer>(new Renderer());

	int stage = -1;
	int round = -1;
	int currentBoard = -1;
	bool isCombatPhase = false;
	float cameraDirection;

	friend void to_json(nlohmann::json& j, const MemSnapshot& ms) {
		//std::cout << "Called insides MemSnapshots's to_json\n";
		j = nlohmann::json{
			{ "width", ms.renderer->width },
			{ "height", ms.renderer->height  },
			{ "stage", ms.stage },
			{ "round", ms.round },
			{ "currentBoard", ms.currentBoard },
			{ "isCombatPhase", ms.isCombatPhase },
			{ "traits", ms.traits },
			{ "shopUnits", ms.shopUnits },
			{ "units", ms.units }
		};
	}
};