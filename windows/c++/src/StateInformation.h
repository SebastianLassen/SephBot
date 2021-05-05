#pragma once

#include "Common.h"
#include "Units.h"

class StateInformation
{
	friend class Global;

	std::map<BWAPI::Player, UnitData> p_unitData;

	void updateUnit(BWAPI::Unit unit);
	void updateUnitData();
	bool isUnitValid(BWAPI::Unit unit);

	StateInformation();

public:

	void update();

};