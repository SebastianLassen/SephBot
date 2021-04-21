#pragma once

#include "Common.h"
#include "Grid.hpp"

#include "BuildingData.h"

class BuildingPlacer
{

	Grid<int>							p_reserveMap;
	std::vector<Building>				p_buildingPositions;


public:

	BuildingPlacer();

	void					addBuildingPosition(const Building& b);
	void					removeBuildingPosition(BWAPI::TilePosition pos);

	std::vector<BWAPI::TilePosition> &	getBuildingPositions();

	bool					canBuildHereWithSpace(BWAPI::TilePosition pos, const Building& b, int buildDist);
	bool					buildable(const Building& b, int x, int y) const;
	BWAPI::TilePosition		getBuildingLocation(const Building & b);
	BWAPI::TilePosition		getBuildingLocationNear(const Building & b, int buildDist);


};

