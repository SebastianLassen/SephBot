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

	/*
	newItem.supply = 8;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 10;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 12;
	newItem.type = BWAPI::UnitTypes::Protoss_Assimilator;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 13;
	newItem.type = BWAPI::UnitTypes::Protoss_Zealot;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 16;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 18;
	newItem.type = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 20;
	newItem.type = BWAPI::UnitTypes::Protoss_Dragoon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 21;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 25;
	newItem.type = BWAPI::UnitTypes::Protoss_Dragoon;
	p_buildOrderItems.push_back(newItem);
	*/
}

void BuildOrder::setMidGameBuildOrder()
{
	BuildOrderItem newItem;

	newItem.supply = 15;
	newItem.type = BWAPI::UnitTypes::Protoss_Assimilator;
	p_buildOrderItems.push_back(newItem);
	
	newItem.supply = 22;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 24;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 26;
	newItem.type = BWAPI::UnitTypes::Protoss_Gateway;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 30;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 36;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
	p_buildOrderItems.push_back(newItem);

	newItem.supply = 41;
	newItem.type = BWAPI::UnitTypes::Protoss_Pylon;
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