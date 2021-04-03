#pragma once

#include <BWAPI.h>
#include <vector>

class Building
{

public:

	BWAPI::TilePosition position		= BWAPI::TilePositions::None;
	BWAPI::UnitType		type			= BWAPI::UnitTypes::None;
	BWAPI::Unit			builderUnit		= nullptr;
	BWAPI::Unit			self			= nullptr;
	bool				assigned		= false;
	bool				constructing	= false;

	Building()
	{
	}
	
	Building(BWAPI::UnitType t, BWAPI::TilePosition pos)
		: type(t)
		, position(pos)
	{
	}

	// equals operator
	bool operator==(const Building& b)
	{
		// buildings are equal if their worker unit or building unit are equal
		return (b.self == self) || (b.builderUnit == builderUnit);
	}

};

class BuildingData
{

	std::vector<Building> p_buildings;

public:

	BuildingData();

	const std::vector<Building>	&	getBuildings() const;
	void							addBuilding(const Building & b);
	void							removeBuilding(const Building & b);
	void							removeBuildings(const std::vector<Building> & buildings);

};