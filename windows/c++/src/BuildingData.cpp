#include "BuildingData.h"

BuildingData::BuildingData()
{

}

const std::vector<Building>	& BuildingData::getBuildings() const
{
	return p_buildings;
}

void BuildingData::addBuilding(const Building& b)
{
	p_buildings.push_back(b);
}

void BuildingData::removeBuilding(const Building& b)
{
	auto& it = std::find(p_buildings.begin(), p_buildings.end(), b);

	if (it != p_buildings.end())
	{
		p_buildings.erase(it);
	}
}

void BuildingData::removeBuildings(const std::vector<Building>& buildings)
{
	for (auto& b : buildings)
	{
		removeBuilding(b);
	}
}
