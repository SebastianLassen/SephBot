#pragma once

#include <BWAPI.h>
#include <vector>
#include <deque>
#include <algorithm>

#include "BuildOrder.h"
#include "BuildOrderQueue.h"
#include "BuildingData.h"

class ProductionManager
{
	BuildOrder			p_buildOrder;
	BuildOrderQueue		p_queue;

	int		p_reservedMinerals = 0;
	int		p_reservedGas = 0;

	std::vector<Building> p_buildings;

public:

	ProductionManager();

	void		onStart();
	void		onFrame();	
	void		findItemToProduce();
	void		assignWorkerToItem();
	void		issueProduction();
	void		checkForStartedProduction();
	void		trainUnit(BWAPI::UnitType type);
	void		addToBuildings(BWAPI::Unit unit);
	void		addToUnits(BWAPI::Unit unit);
	void		removeFromBuildings();
	void		removeBuildings(const std::vector<Building>& toRemove);
	void		removeFromUnits(BWAPI::Unit unit);

	int			getReservedMinerals();
	int			getReservedGas();

};