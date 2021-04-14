#include "BuildingPlacer.h"
#include "MapTools.h"
#include "Tools.h"

BuildingPlacer::BuildingPlacer()
{
	m_reserveMap = Grid<int>(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight(), 0);
}


BWAPI::TilePosition	BuildingPlacer::getBuildingLocation(const Building& b)
{
	int numPylons = Tools::CountUnitsOfType(BWAPI::UnitTypes::Protoss_Pylon, BWAPI::Broodwar->self()->getUnits());

	if (b.type.requiresPsi() && numPylons == 0)
	{
		return BWAPI::TilePositions::None;
	}

	if (b.type.isRefinery())
	{
		// return getRefineryPosition(); 
	}

	/*
	if (b.type.isResourceDepot())
	{
		FIND EXPANSION POSITION, MIGHT BE FROM MAPTOOLS
	}
	*/

	int distance = Tools::getBuildingLocationDistance(b);

	return getBuildingLocationNear(b, distance);
}

BWAPI::TilePosition BuildingPlacer::getBuildingLocationNear(const Building& b, int buildDist)
{
	/*
	Need an overall map grid or something that keeps track of 
	what has been placed or something.


	
	*/


	return BWAPI::TilePositions::None;
}
