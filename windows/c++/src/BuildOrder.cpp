#include "BuildOrder.h"

BuildOrder::BuildOrder()
{

}

void BuildOrder::setBuildOrder()
{
	BuildOrderItem newItem;

	newItem.supply = 8;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 10;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 12;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 15;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);
}

void BuildOrder::setMidGameBuildOrder()
{
	BuildOrderItem newItem;
	
	newItem.supply = 22;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 35;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 50;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 55;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

}

void BuildOrder::removeBuildOrderItem()
{
	p_buildOrderItems.erase(p_buildOrderItems.begin());
}

int BuildOrder::getSize()
{
	return p_buildOrderItems.size();
}

bool BuildOrder::isEmpty()
{
	return p_buildOrderItems.empty();
}

const BuildOrderItem& BuildOrder::operator[] (const size_t & index) const
{
	return p_buildOrderItems[index];
}