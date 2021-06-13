#pragma once

#include "Common.h"

#include "../BWEM 1.4.1/src/bwem.h"

class Base
{
	int						p_baseID = 0;
	BWAPI::Player			p_owner;
	bool					p_isStartLocation = false;
	BWAPI::TilePosition		p_depotLocation;

	std::vector<BWAPI::Unit>	p_minerals;
	std::vector<BWAPI::Unit>	p_geysers;

	std::vector<BWAPI::Position>	p_mineralsPos;
	std::vector<BWAPI::Position>	p_geysersPos;
     
	const BWEM::Base*		bwemBase;	

public:

	Base(int baseID, BWAPI::TilePosition depotLocation, const BWEM::Base* bwemBase);

	const BWAPI::TilePosition&	getDepotLocation() const { return p_depotLocation; }
	//
	const BWEM::Area*			getArea() const { return bwemBase->GetArea(); }
	size_t						mineralPatchCount() const { return bwemBase->Minerals().size(); }
	
	
	const std::vector<BWAPI::Unit>&			getMinerals() const;
	const std::vector<BWAPI::Unit>&			getGeysers() const;
	const std::vector<BWAPI::Position>&		getMineralsPos() const;
	const std::vector<BWAPI::Position>&		getGeyserPos() const;

	BWAPI::Player	getOwner(BWAPI::Player owner = nullptr);

	const int	getID(const Base& base) const;
	int			minerals() const;
	int			gas() const;
	bool		isStartLocation(bool startLocation = false);




};
/*
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
*/

