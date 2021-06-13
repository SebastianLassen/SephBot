#include "Squads.h"

Squads::Squads()
{

}

void Squads::addToSquad(BWAPI::Unit unit)
{
	std::string unitName = unit->getType().getName();

	auto& it = p_squads.find(unitName);
	if (it == p_squads.end())
	{
		p_squads[unitName] = SquadData();
	}

	p_squads[unitName].units.insert(unit);
}

void Squads::removeFromSquad(BWAPI::Unit unit)
{
	std::string unitName = unit->getType().getName();
	p_squads[unitName].units.erase(unit);
}

int	Squads::getSquadSize(std::string unitName)
{
	return p_squads[unitName].units.size();
}
const SquadData& Squads::getSquad(std::string unitName)
{
	return p_squads[unitName];
}

const std::map<std::string, SquadData>& Squads::getAllSquads()
{
	return p_squads;
}