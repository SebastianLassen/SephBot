#pragma once

#include "Common.h"

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

	const std::vector<BWAPI::Unit>&			getMinerals() const;
	const std::vector<BWAPI::Unit>&			getGeysers() const;
	const std::vector<BWAPI::Position>&		getMineralsPos() const;
	const std::vector<BWAPI::Position>&		getGeyserPos() const;
	const BWAPI::TilePosition&				getDepotLocation() const;

	bool			isStartLocation() const;
	bool			isPlayerOccupying(BWAPI::Player player) const;
	bool			isPlayerStartLocation(BWAPI::Player player) const;


	
};

