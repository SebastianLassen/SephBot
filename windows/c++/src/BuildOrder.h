#pragma once

#include <BWAPI.h>
#include <vector>

struct BuildOrderItem
{
	int supply				= 0;
	BWAPI::UnitType type	= BWAPI::UnitTypes::Unknown;

	BuildOrderItem()
	{
	}

	BuildOrderItem(int s, BWAPI::UnitType t)
		: supply(s)
		, type(t)
	{
	}
};

class BuildOrder
{
	std::vector<BuildOrderItem> p_buildOrderItems;

public: 

	BuildOrder();

	const BuildOrderItem&	operator[] (const size_t& index) const;
	void					setBuildOrder();
	void					removeBuildOrderItem();
	int						getSize();
	bool					isEmpty();

};