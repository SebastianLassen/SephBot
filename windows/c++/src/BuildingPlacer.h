#pragma once

#include <BWAPI.h>
#include <vector>
#include <map>

#include "BuildingData.h"

class BuildingPlacer
{

	Grid<int>	m_reserveMap;


public:

	BuildingPlacer();

	BWAPI::TilePosition		getBuildingLocation(const Building & b);
	BWAPI::TilePosition		getBuildingLocationNear(const Building & b, int buildDist);

};

