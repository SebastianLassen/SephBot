#pragma once

#include "Grid.hpp"

#include "Common.h"
#include "BWEM 1.4.1/src/bwem.h"
#include "BaseManager.h"
#include "ChokePoints.h"
#include <windows.h>


class MapTools
{
    friend class Global;
    
    Grid<int>   m_walkable;       // whether a tile is buildable (includes static resources)          
    Grid<int>   m_buildable;      // whether a tile is buildable (includes static resources)
    Grid<int>   m_depotBuildable; // whether a depot is buildable on a tile (illegal within 3 tiles of static resource)
    Grid<int>   m_lastSeen;       // the last time any of our units has seen this position on the map
    int         m_width = 0;
    int         m_height = 0;
    int         m_frame = 0;
    bool        m_drawMap = false;

    bool                    enemyBaseFound = false;

    bool canBuild(int tileX, int tileY) const;
    bool canWalk(int tileX, int tileY) const;
    void printMap() const;

    std::vector<Base*>	p_bases;
    std::set<ChokePoints*> p_chokePoints;

    Base*   myMain = nullptr;
    Base*   myNatural = nullptr;
    Base*   enemyMain = nullptr;
    Base*   enemyNatural = nullptr;

    ChokePoints* myMainChokePoint = nullptr;
    ChokePoints* myNaturalChokePoint = nullptr;
    ChokePoints* enemyMainChokePoint = nullptr;
    ChokePoints* enemyNaturalChokePoint = nullptr;

    bool	useMapPrinter = false;

public:


    MapTools();

    void    onStart();
    void    onFrame();
    void    draw() const;
    void    toggleDraw();

    int     width() const;
    int     height() const;

    bool    isValidTile(int tileX, int tileY) const;
    bool    isValidTile(const BWAPI::TilePosition & tile) const;
    bool    isValidPosition(const BWAPI::Position & pos) const;
    bool    isPowered(int tileX, int tileY) const;
    bool    isExplored(int tileX, int tileY) const;
    bool    isExplored(const BWAPI::Position & pos) const;
    bool    isExplored(const BWAPI::TilePosition & pos) const;
    bool    isVisible(int tileX, int tileY) const;
    bool    isWalkable(int tileX, int tileY) const;
    bool    isWalkable(const BWAPI::TilePosition& tile) const;
    bool    isBuildable(int tileX, int tileY) const;
    bool    isBuildable(const BWAPI::TilePosition& tile) const;
    bool    isDepotBuildableTile(int tileX, int tileY) const;
    void    drawTile(int tileX, int tileY, const BWAPI::Color & color) const;

    Base*                 setSelfNaturalExpansion(BWAPI::TilePosition startLocation); 
    void                  setEnemyStartLocation(BWAPI::TilePosition pos);
    ChokePoints*          computeMainChoke(Base* myMain, Base* myNatural);
    bool                  isEnemyBaseFound(bool found = false);
    BWAPI::TilePosition   getEnemyStartLocation();
    BWAPI::TilePosition   getSelfStartLocation();

    std::vector<Base*>&  getAllBases();
    std::vector<Base*>   getMyBases();
    std::vector<Base*>   getEnemyBases();

    Base*       getMainBase();
    Base*       getNaturalBase();
    Base*       getEnemyMain();
    Base*       getEnemyNatural();

    ChokePoints*    getMainChoke();
    ChokePoints*    getNaturalChoke();
    ChokePoints*    getEnemyMainChoke();
    ChokePoints*    getEnemyNaturalChoke();

    void                  testAssertFunction();
    void                  drawTestTiles();
};