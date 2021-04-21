#include "BuildingPlacer.h"
#include "MapTools.h"
#include "Global.h"
#include "Tools.h"

BuildingPlacer::BuildingPlacer()
{
	p_reserveMap = Grid<int>(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight(), 0);
}


void BuildingPlacer::addBuildingPosition(const Building& b)
{
	p_buildingPositions.push_back(b);
}

void BuildingPlacer::removeBuildingPosition(BWAPI::TilePosition pos)
{
	for (auto& pos : p_buildingPositions)
	{
		auto& it = std::find(p_buildingPositions.begin(), p_buildingPositions.end(), pos);

		if (it != p_buildingPositions.end())
		{
			p_buildingPositions.erase(it);
		}
	}
}


std::vector<BWAPI::TilePosition>& BuildingPlacer::getBuildingPositions()
{
	std::vector<BWAPI::TilePosition> buildingPositions;
	for (auto& b : p_buildingPositions)
	{
		if (!(b.self == nullptr || !b.self->getType().isBuilding() || b.self->getHitPoints() <= 0))
		{
			buildingPositions.push_back(b.position);
		}
	}
	return buildingPositions;
}


bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition pos, const Building& b, int buildDist)
{
	BWAPI::UnitType type = b.type;

	int width = b.type.tileWidth();
	int height = b.type.tileHeight();

	int startx = pos.x - buildDist;
	int starty = pos.y - buildDist;
	int endx = pos.x + width + buildDist;
	int endy = pos.y + height + buildDist;


	if (startx < 0 || starty < 0 || endx > BWAPI::Broodwar->mapWidth() || endx < pos.x + width || endy > BWAPI::Broodwar->mapHeight())
	{
		return false;
	}

	for (int x = startx; x < endx; x++)
	{
		for (int y = starty; y < endy; y++)
		{
			if (!b.type.isRefinery())
			{
				//if (!buildable(b, x, y) || m_reserveMap.get(x, y) || ((b.type != BWAPI::UnitTypes::Protoss_Photon_Cannon) && isInResourceBox(x, y)))
				if (!buildable(b, x, y))
				{
					return false;
				}
			}
		}
	}

	return true;

}

bool BuildingPlacer::buildable(const Building& b, int x, int y) const
{
	BWAPI::TilePosition tp(x, y);

	//returns true if this tile is currently buildable, takes into account units on tile
	if (!BWAPI::Broodwar->isBuildable(x, y))
	{
		return false;
	}

	for (auto& unit : BWAPI::Broodwar->getUnitsOnTile(x, y))
	{
		if ((b.builderUnit != nullptr) && (unit != b.builderUnit))
		{
			return false;
		}
	}

	if (!tp.isValid())
	{
		return false;
	}

	return true;
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

	std::vector<BWAPI::TilePosition>& buildingPositions = getBuildingPositions();

	if (p_buildingPositions.empty())
	{
		if (canBuildHereWithSpace(b.position, b, buildDist))
		{
			return b.position;
		}
	}

	for (size_t i(0); i < buildingPositions.size(); i++)
	{
		if (canBuildHereWithSpace(buildingPositions[i], b, buildDist))
		{
			// NEED TO OFFSET AS THE TILEPOSITIONS ARE THE EXACT POSITIONS OF BUILDINGS
			return buildingPositions[i];
		}
	}

	return BWAPI::TilePositions::None;
}

