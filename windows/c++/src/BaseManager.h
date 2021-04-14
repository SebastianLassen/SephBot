#pragma once

#include <BWAPI.h>
#include <vector>
#include <map>

class BaseManager
{
	int						p_baseID = 0;
	BWAPI::TilePosition		p_startLocation;
	bool					p_isStartLocation = false;
	BWAPI::TilePosition		p_depotLocation;

	std::vector<BWAPI::Unit>	p_minerals;
	std::vector<BWAPI::Unit>	p_geysers;

	std::vector<BWAPI::Position>	p_mineralsPos;
	std::vector<BWAPI::Position>	p_geysersPos;

	std::map<BWAPI::Player, bool>	p_isPlayerOccupying;
	std::map<BWAPI::Player, bool>	p_isPlayerStartLocation;

public:

	BaseManager(int baseID, const std::vector<BWAPI::Unit> & resources);


	
};

