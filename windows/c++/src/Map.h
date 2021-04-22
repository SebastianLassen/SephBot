#pragma once

#include "Common.h"

#include "BWEM 1.4.1/src/bwem.h"
#include "BaseManager.h"
#include <windows.h>

class Map
{
	
	std::vector<Base*>	p_bases;


	bool	useMapPrinter = false;

public:

	Map();
	void Initialize();

	BWAPI::TilePosition getEnemyStartLocation();
	BWAPI::TilePosition getSelfStartLocation();


};