#include "StateInformation.h"
#include "Global.h"

StateInformation::StateInformation()
{
	update();
}

void StateInformation::updateUnit(BWAPI::Unit unit)
{

}

void StateInformation::updateUnitData()
{

}

bool StateInformation::isUnitValid(BWAPI::Unit unit)
{
	return false;
}

void StateInformation::update()
{
	updateUnitData();
}