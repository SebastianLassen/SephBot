#include "BaseManager.h"
#include "MapTools.h"

Base::Base(int baseID, BWAPI::TilePosition depotLocation, const BWEM::Base* bwemBase)
    : p_baseID(baseID)
    , p_depotLocation(depotLocation)
    , bwemBase(bwemBase)
{
    for (auto& minerals : bwemBase->Minerals())
    {
        p_minerals.push_back(minerals->Unit());
        p_mineralsPos.push_back(minerals->Unit()->getPosition());
    }

    for (auto& geyser : bwemBase->Geysers())
    {
        p_geysers.push_back(geyser->Unit());
        p_geysersPos.push_back(geyser->Unit()->getPosition());
    }
}

const std::vector<BWAPI::Unit>& Base::getMinerals() const
{
    return p_minerals;
}

const std::vector<BWAPI::Unit>& Base::getGeysers() const
{
    return p_geysers;
}

const std::vector<BWAPI::Position>& Base::getMineralsPos() const
{
    return p_mineralsPos;
}

const std::vector<BWAPI::Position>& Base::getGeyserPos() const
{
    return p_geysersPos;
}

BWAPI::Player Base::getOwner(BWAPI::Player owner = nullptr)
{
    if (owner)
    {
        p_owner = owner;
    }
    return p_owner;
}

int	Base::minerals() const
{
    int sum = 0;
    for (auto mineral : bwemBase->Minerals())
    {
        sum += mineral->Amount();
    }

    return sum;
}

int	Base::gas() const
{
    int sum = 0;
    for (auto geyser : bwemBase->Geysers())
    {
        sum += geyser->Amount();
    }

    return sum;
}

bool Base::isStartLocation(bool startLocation)
{
    if (startLocation)
    {
        p_isStartLocation = true;
    }
    return p_isStartLocation;
}


/*
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
*/

