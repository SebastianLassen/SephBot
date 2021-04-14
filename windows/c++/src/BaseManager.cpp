#include "BaseManager.h"
#include "MapTools.h"

BaseManager::BaseManager(int baseID, const std::vector<BWAPI::Unit> & resources)
	: p_baseID(baseID)
{
    p_isPlayerStartLocation[BWAPI::Broodwar->self()] = false;
    p_isPlayerStartLocation[BWAPI::Broodwar->enemy()] = false;
    p_isPlayerOccupying[BWAPI::Broodwar->self()] = false;
    p_isPlayerOccupying[BWAPI::Broodwar->enemy()] = false;

    for (auto& resource : resources)
    {
        if (resource->getType().isMineralField())
        {
            p_minerals.push_back(resource);
            p_mineralsPos.push_back(resource->getPosition());
        }
        else
        {
            p_geysers.push_back(resource);
            p_geysersPos.push_back(resource->getPosition());
        }
    }
}


const std::vector<BWAPI::Unit>& BaseManager::getMinerals() const
{
    return p_minerals;
}

const std::vector<BWAPI::Unit>& BaseManager::getGeysers() const
{
    return p_geysers;
}

const std::vector<BWAPI::Position>& BaseManager::getMineralsPos() const
{
    return p_mineralsPos;
}

const std::vector<BWAPI::Position>& BaseManager::getGeyserPos() const
{
    return p_geysersPos;
}

const BWAPI::TilePosition& BaseManager::getDepotLocation() const
{
    return p_depotLocation;
}

bool BaseManager::isStartLocation() const
{
    return p_isStartLocation;
}

bool BaseManager::isPlayerOccupying(BWAPI::Player player) const
{
    return p_isPlayerOccupying.at(player);
}

bool BaseManager::isPlayerStartLocation(BWAPI::Player player) const
{
    return p_isPlayerStartLocation.at(player);
}

