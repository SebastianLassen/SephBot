#include "Map.h"


Map::Map()
{

}

void Map::Initialize()
{
	p_bases.clear();

    BWEM::Map::Instance().Initialize();
    BWEM::Map::Instance().EnableAutomaticPathAnalysis();
    bool startingLocationsOK = BWEM::Map::Instance().FindBasesForStartingLocations();
    assert(startingLocationsOK);

    if (useMapPrinter)
    {
        BWEM::utils::MapPrinter::Initialize(&BWEM::Map::Instance());
        BWEM::utils::printMap(BWEM::Map::Instance());      // will print the map into the file <StarCraftFolder>bwapi-data/map.bmp
        BWEM::utils::pathExample(BWEM::Map::Instance());   // add to the printed map a path between two starting locations
    }

    // Initialize the bases
    for (const auto& area : BWEM::Map::Instance().Areas())
    {
        
        for (const auto& base : area.Bases())
        {
            auto newBase = new Base(p_bases.size() + 1, base.Location(), &base);
            if (base.Starting())
            {
                newBase->isStartLocation(true);
            }
            if (base.Location() == BWAPI::Broodwar->self()->getStartLocation())
            {
                newBase->getOwner(BWAPI::Broodwar->self());
            }

            p_bases.push_back(newBase);
        }
    }
}

BWAPI::TilePosition Map::getEnemyStartLocation()
{
    for (auto& base : p_bases)
    {
        if (base->getOwner() == BWAPI::Broodwar->enemy() && base->isStartLocation())
        {
            return base->getDepotLocation();
        }
    }
}

BWAPI::TilePosition Map::getSelfStartLocation()
{
    for (auto& base : p_bases)
    {
        if (base->getOwner() == BWAPI::Broodwar->self() && base->isStartLocation())
        {
            return base->getDepotLocation();
        }
    }
}