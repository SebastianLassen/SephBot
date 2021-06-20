#include "Units.h"

Units::Units()
	: lostMinerals(0)
	, lostGas(0)
{
}

void Units::addUnitUpdate(BWAPI::Unit unit)
{
	if (!unit) { return; }

	auto& it = unitMap.find(unit);
	if (it == unitMap.end())
	{
		unitMap[unit] = UnitData();
	}

	unitMap[unit].unitID		= unit->getID();
	unitMap[unit].player		= unit->getPlayer();
	unitMap[unit].unit			= unit;
	unitMap[unit].lastPosition  = unit->getPosition();
	unitMap[unit].type			= unit->getType();
	unitMap[unit].completed		= unit->isCompleted();

}

