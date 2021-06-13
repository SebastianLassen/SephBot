#pragma once

#include "Common.h"

struct SquadData
{

	BWAPI::Unitset	units;
	std::string		role = "Default";

	SquadData()
	{

	}
};

class Squads
{
	friend class Global;

	std::map<std::string, SquadData> p_squads;
		
public:
	
	Squads();

	void				addToSquad(BWAPI::Unit unit);
	void				removeFromSquad(BWAPI::Unit unit);
	int					getSquadSize(std::string unitName);
	const SquadData&	getSquad(std::string unitName);

	const std::map<std::string, SquadData>& getAllSquads();

	//void setSquadRole(std::string role = "Attacking");

};