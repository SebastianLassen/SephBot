#include "MapTools.h"
#include "Global.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <array>

// constructor for MapTools
MapTools::MapTools()
{
    
}

void MapTools::onStart()
{
    m_width             = BWAPI::Broodwar->mapWidth();
    m_height            = BWAPI::Broodwar->mapHeight();
    m_walkable          = Grid<int>(m_width, m_height, 1);
    m_buildable         = Grid<int>(m_width, m_height, 0);
    m_depotBuildable    = Grid<int>(m_width, m_height, 0);
    m_lastSeen          = Grid<int>(m_width, m_height, 0);

    // Set the boolean grid data from the Map
    for (int x(0); x < m_width; ++x)
    {
        for (int y(0); y < m_height; ++y)
        {
            m_buildable.set(x, y, canBuild(x, y));
            m_depotBuildable.set(x, y, canBuild(x, y));
            m_walkable.set(x, y, m_buildable.get(x,y) || canWalk(x, y));
        }
    }

    // set tiles that static resources are on as unbuildable
    for (auto & resource : BWAPI::Broodwar->getStaticNeutralUnits())
    {
        if (!resource->getType().isResourceContainer())
        {
            continue;
        }

        const int tileX = resource->getTilePosition().x;
        const int tileY = resource->getTilePosition().y;

        for (int x=tileX; x<tileX+resource->getType().tileWidth(); ++x)
        {
            for (int y=tileY; y<tileY+resource->getType().tileHeight(); ++y)
            {
                m_buildable.set(x, y, false);

                // depots can't be built within 3 tiles of any resource
                for (int rx=-3; rx<=3; rx++)
                {
                    for (int ry=-3; ry<=3; ry++)
                    {
                        if (!BWAPI::TilePosition(x+rx, y+ry).isValid())
                        {
                            continue;
                        }

                        m_depotBuildable.set(x+rx, y+ry, 0);
                    }
                }
            }
        }
    }

    p_bases.clear();
    p_chokePoints.clear();

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
                myMain = newBase;
            }

            p_bases.push_back(newBase);
        }

        for (const auto& choke : area.ChokePoints())
        {
            auto newChoke = new ChokePoints(choke);

            p_chokePoints.insert(newChoke);
        }
    }

   myNatural = setSelfNaturalExpansion(myMain->getDepotLocation());
    
   myMainChokePoint = computeMainChoke(myMain, myNatural);

}

void MapTools::onFrame()
{
    for (int x=0; x<m_width; ++x)
    {
        for (int y=0; y<m_height; ++y)
        {
            if (isVisible(x, y))
            {
                // Should be (x, y, m_frame);
                m_lastSeen.set(x, y, BWAPI::Broodwar->getFrameCount());
            }
        }
    }

    //m_frame++;

    if (m_drawMap)
    {
        draw();
    }
}

void MapTools::toggleDraw()
{
    m_drawMap = !m_drawMap;
}

bool MapTools::isExplored(const BWAPI::TilePosition & pos) const
{
    return isExplored(pos.x, pos.y);
}

bool MapTools::isExplored(const BWAPI::Position & pos) const
{
    return isExplored(BWAPI::TilePosition(pos));
}

bool MapTools::isExplored(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY)) { return false; }

    return BWAPI::Broodwar->isExplored(tileX, tileY);
}

bool MapTools::isVisible(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY)) { return false; }

    return BWAPI::Broodwar->isVisible(BWAPI::TilePosition(tileX, tileY));
}

bool MapTools::isPowered(int tileX, int tileY) const
{
    return BWAPI::Broodwar->hasPower(BWAPI::TilePosition(tileX, tileY));
}

bool MapTools::isValidTile(int tileX, int tileY) const
{
    return tileX >= 0 && tileY >= 0 && tileX < m_width && tileY < m_height;
}

bool MapTools::isValidTile(const BWAPI::TilePosition & tile) const
{
    return isValidTile(tile.x, tile.y);
}

bool MapTools::isValidPosition(const BWAPI::Position & pos) const
{
    return isValidTile(BWAPI::TilePosition(pos));
}

bool MapTools::isBuildable(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY))
    {
        return false;
    }

    return m_buildable.get(tileX, tileY);
}

bool MapTools::isBuildable(const BWAPI::TilePosition & tile) const
{
    return isBuildable(tile.x, tile.y);
}

void MapTools::printMap() const
{
    std::stringstream ss;
    for (int y(0); y < m_height; ++y)
    {
        for (int x(0); x < m_width; ++x)
        {
            ss << isWalkable(x, y);
        }

        ss << "\n";
    }

    std::ofstream out("map.txt");
    out << ss.str();
    out.close();
}

bool MapTools::isDepotBuildableTile(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY))
    {
        return false;
    }

    return m_depotBuildable.get(tileX, tileY);
}

bool MapTools::isWalkable(int tileX, int tileY) const
{
    if (!isValidTile(tileX, tileY))
    {
        return false;
    }

    return m_walkable.get(tileX, tileY);
}

bool MapTools::isWalkable(const BWAPI::TilePosition & tile) const
{
    return isWalkable(tile.x, tile.y);
}

int MapTools::width() const
{
    return m_width;
}

int MapTools::height() const
{
    return m_height;
}

void MapTools::drawTile(int tileX, int tileY, const BWAPI::Color & color) const
{
    const int padding   = 2;
    const int px        = tileX*32 + padding;
    const int py        = tileY*32 + padding;
    const int d         = 32 - 2*padding;

    BWAPI::Broodwar->drawLineMap(px,     py,     px + d, py,     color);
    BWAPI::Broodwar->drawLineMap(px + d, py,     px + d, py + d, color);
    BWAPI::Broodwar->drawLineMap(px + d, py + d, px,     py + d, color);
    BWAPI::Broodwar->drawLineMap(px,     py + d, px,     py,     color);
}

bool MapTools::canWalk(int tileX, int tileY) const
{
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            if (!BWAPI::Broodwar->isWalkable(tileX*4 + i, tileY*4 + j))
            {
                return false;
            }
        }
    }

    return true;
}

bool MapTools::canBuild(int tileX, int tileY) const
{
    return BWAPI::Broodwar->isBuildable(BWAPI::TilePosition(tileX, tileY));
}

void MapTools::draw() const
{
    const BWAPI::TilePosition screen(BWAPI::Broodwar->getScreenPosition());
    const int sx = screen.x;
    const int sy = screen.y;
    const int ex = sx + 20;
    const int ey = sy + 15;

    for (int x = sx; x < ex; ++x)
    {
        for (int y = sy; y < ey; y++)
        {
            const BWAPI::TilePosition tilePos(x,y);
            if (!tilePos.isValid()) { continue; }

            if (true)
            {
                BWAPI::Color color = isWalkable(x, y) ? BWAPI::Color(0, 255, 0) : BWAPI::Color(255, 0, 0);
                if (isWalkable(x, y) && !isBuildable(x, y)) { color = BWAPI::Color(255, 255, 0); }
                if (isBuildable(x, y) && !isDepotBuildableTile(x, y)) { color = BWAPI::Color(127, 255, 255); }
                drawTile(x, y, color);
            }
        }
    }

    const char red = '\x08';
    const char green = '\x07';
    const char white = '\x04';
    const char yellow = '\x03';

    BWAPI::Broodwar->drawBoxScreen(0, 0, 200, 100, BWAPI::Colors::Black, true);
    BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Huge);
    BWAPI::Broodwar->drawTextScreen(10, 5, "%cMap Legend", white);
    BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Default);
    BWAPI::Broodwar->drawTextScreen(10, 30, "%cRed: ", red);
    BWAPI::Broodwar->drawTextScreen(60, 30, "%cCan't Walk or Build", white);
    BWAPI::Broodwar->drawTextScreen(10, 45, "%cGreen:", green);
    BWAPI::Broodwar->drawTextScreen(60, 45, "%cCan Walk and Build", white);
    BWAPI::Broodwar->drawTextScreen(10, 60, "%cYellow:", yellow);
    BWAPI::Broodwar->drawTextScreen(60, 60, "%cResource Tile, Can't Build", white);
    BWAPI::Broodwar->drawTextScreen(10, 75, "Teal:");
    BWAPI::Broodwar->drawTextScreen(60, 75, "%cCan't Build Depot", white);
   
}

Base* MapTools::setSelfNaturalExpansion(BWAPI::TilePosition startLocation)
{
    Base* findingNatural = nullptr;
    int bestDist = INT_MAX;
    int dist = 0;

    
    for (auto& base : p_bases)
    {
        if (base->getDepotLocation() == startLocation) { continue; }
        if (base->gas() == 0) { continue; }
        
        BWEM::Map::Instance().GetPath(BWAPI::Position (startLocation), 
                                      BWAPI::Position (base->getDepotLocation()), 
                                      &dist);

        if (dist == -1 || dist > bestDist) { continue; }

        bestDist = dist;
        findingNatural = base;

    }
    
    return findingNatural;
}

void MapTools::setEnemyStartLocation(BWAPI::TilePosition pos)
{
    for (auto& base : p_bases)
    {
        if (base->getDepotLocation() == pos && base->isStartLocation())
        {
            base->getOwner(BWAPI::Broodwar->enemy());
            enemyMain = base;
        }
    }
}

ChokePoints* MapTools::computeMainChoke(Base* myMain, Base* myNatural)
{
    if (!myMain || !myNatural) { return nullptr; }

    ChokePoints* bestChoke = nullptr;

    auto mainArea    = myMain->getArea();
    auto naturalArea = myNatural->getArea();
    auto areaCheck1 = std::make_pair(mainArea, naturalArea);
    auto areaCheck2 = std::make_pair(naturalArea, mainArea);

    for (auto& elem : p_chokePoints)
    {
        auto& areasOfChoke = elem->choke->GetAreas();

        if (areasOfChoke == areaCheck1 || areasOfChoke == areaCheck2)
        {
            bestChoke = elem;
            break;
        }
    }

    return bestChoke;

}

bool MapTools::isEnemyBaseFound(bool found)
{
    if (found)
    {
        enemyBaseFound = true;
    }
    return enemyBaseFound;
}

BWAPI::TilePosition MapTools::getEnemyStartLocation()
{
    if (enemyMain)
    {
        return enemyMain->getDepotLocation();
    }
    
    return BWAPI::TilePositions::None;
}

BWAPI::TilePosition MapTools::getSelfStartLocation()
{
    if (myMain)
    {
        return myMain->getDepotLocation();
    }

    return BWAPI::TilePositions::None;
}

BWAPI::Position MapTools::scoutClosestStartingLocation(BWAPI::Unit unit, std::map<int, BWAPI::TilePosition>& locationMap)
{
    if (!unit) { return BWAPI::Positions::None; }

    BWAPI::Position bestPos = BWAPI::Position(locationMap.begin()->second);

    Global::Map().setEnemyStartLocation(locationMap.begin()->second);

    if (BWAPI::Broodwar->isExplored(locationMap.begin()->second))
    {
        std::cout << "\nStart location at: " << locationMap.begin()->second 
                  << " With a distance in pixels " << locationMap.begin()->first
                  << "\nFrom our start location wasn't the enemy's starting location " << "\n";

        locationMap.erase(locationMap.begin());
        bestPos = BWAPI::Position(locationMap.begin()->second);

        std::cout << "\nMoving to start location: " << locationMap.begin()->second
                  << " with a distance in pixels of: " << locationMap.begin()->first << "\n";

    }

    return bestPos;
    
}

std::vector<Base*>& MapTools::getAllBases()
{
    return p_bases;
}

std::vector<Base*> MapTools::getMyBases()
{
    std::vector<Base*> p_myBases;

    for (auto& base : p_bases)
    {
        if (base->getOwner() == BWAPI::Broodwar->self())
        {
            p_myBases.push_back(base);
        }
    }

    return p_myBases;
}

std::vector<Base*> MapTools::getEnemyBases()
{
    std::vector<Base*> p_enemyBases;

    for (auto& base : p_bases)
    {
        if (base->getOwner() == BWAPI::Broodwar->enemy())
        {
            p_enemyBases.push_back(base);
        }
    }

    return p_enemyBases;
}

Base* MapTools::getMainBase()
{
    return myMain;
}

Base* MapTools::getNaturalBase()
{
    return myNatural;
}

Base* MapTools::getEnemyMain()
{
    return enemyMain;
}

Base* MapTools::getEnemyNatural()
{
    return enemyNatural;
}

ChokePoints* MapTools::getMainChoke()
{
    return myMainChokePoint;
}

ChokePoints* MapTools::getNaturalChoke()
{
    return myNaturalChokePoint;
}

ChokePoints* MapTools::getEnemyMainChoke()
{
    return enemyMainChokePoint;
}

ChokePoints* MapTools::getEnemyNaturalChoke()
{
    return enemyNaturalChokePoint;
}

void MapTools::drawTestTiles()
{
    BWAPI::TilePosition depotPos;

    for (auto& u : BWAPI::Broodwar->self()->getUnits())
    {
        if (u->getType().isResourceDepot())
        {
            depotPos = u->getTilePosition();
            break;
        }
    }

    const int sx = depotPos.x + 2;
    const int sy = depotPos.y + 2;
    
    const int ex = depotPos.x + 10;
    const int ey = depotPos.y + 5;

    for (int x = sx; x < ex; ++x)
    {
        for (int y = sy; y < ey; y++)
        {
            const BWAPI::TilePosition tilePos(x, y);
            if (!tilePos.isValid()) { continue; }

            if (true)
            {
                drawTile(x, y, BWAPI::Colors::Blue);
            }
        }
    }

}