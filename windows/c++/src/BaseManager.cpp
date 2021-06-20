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

BWAPI::Player Base::getOwner(BWAPI::Player owner)
{
    if (owner)
    {
        p_owner = owner;
    }
    return p_owner;
}

const int Base::getID(const Base& base) const
{
    return base.p_baseID;
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

