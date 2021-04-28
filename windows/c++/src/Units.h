#pragma once

#include "Common.h"

struct UnitData
{

	int					unitID = 0;
	BWAPI::Player		player = nullptr;
	BWAPI::Unit			unit;
	BWAPI::Position		lastPosition = BWAPI::Positions::None;
	BWAPI::UnitType		type = BWAPI::UnitTypes::None;
	bool				completed = false;

	UnitData()
	{

	}

};

typedef std::map<BWAPI::Unit, UnitData> UnitDataMap;

class Units
{
	
	UnitDataMap			unitMap;
	//std::vector<int>	deadUnits;
	//std::vector<int>	liveUnits;

	int	lostMinerals = 0;
	int	lostGas = 0;

public:

	Units();

	void	addUnitUpdate(BWAPI::Unit unit);

};

