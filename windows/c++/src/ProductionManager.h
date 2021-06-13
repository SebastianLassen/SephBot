#pragma once

#include "Common.h"

#include "BuildOrder.h"
#include "BuildOrderQueue.h"
#include "BuildingData.h"
#include "BuildingPlacer.h"

class ProductionManager
{
	friend class Global;

	BuildOrder			p_buildOrder;
	BuildOrder			p_buildOrderMidGame;
	BuildOrderQueue		p_queue;
	BuildingPlacer		p_buildingPlacer;

	int		p_reservedMinerals = 0;
	int		p_reservedGas = 0;

	bool	naturalNexus = false;

	std::vector<Building> p_buildings;

public:

	ProductionManager();

	void		onStart();
	void		onFrame();	
	void		findItemToProduce(BuildOrder & buildOrder);
	void		assignWorkerToItem();
	void		buildNaturalNexus();
	void		issueProduction();
	void		checkForStartedProduction();
	void		trainUnit(BWAPI::UnitType type);
	void		addToBuildings(BWAPI::Unit unit);
	void		addToUnits(BWAPI::Unit unit);
	void		removeFromBuildings();
	void		removeBuildings(const std::vector<Building>& toRemove);
	void		removeFromUnits(BWAPI::Unit unit);
	bool		checkNaturalNexus();

	int			getReservedMinerals();
	int			getReservedGas();

};