#include "ProductionManager.h"
#include "Tools.h"
#include "Global.h"

ProductionManager::ProductionManager()
{

}

void ProductionManager::onStart()
{
	p_buildOrder.setBuildOrder();
	p_buildOrderMidGame.setMidGameBuildOrder();

	BWAPI::Unit depot = Tools::GetDepot();
	Building b(depot->getType(), depot->getTilePosition());
	b.self = depot;

	p_buildingPlacer.addBuildingPosition(b);
}

void ProductionManager::onFrame()
{
	if (p_reservedMinerals < 0)
	{
		p_reservedMinerals = 0;
	}

	if (p_reservedGas < 0)
	{
		p_reservedGas = 0;
	}

	removeFromBuildings();

	if (p_buildOrder.getSize() > 0)
	{
		if (p_buildOrder[0].supply <= BWAPI::Broodwar->self()->supplyUsed() / 2)
		{
			findItemToProduce(p_buildOrder);
		}
	}

	if (p_queue.getSize() > 0)
	{
		// Handle the item in front of the queue if there is any
		assignWorkerToItem();
		issueProduction();
		checkForStartedProduction();
	}

	
	if (p_buildOrder.isEmpty())
	{
		trainUnit(BWAPI::UnitTypes::Protoss_Zealot);

		if (p_buildOrderMidGame.getSize() > 0)
		{
			if (p_buildOrderMidGame[0].supply <= BWAPI::Broodwar->self()->supplyUsed() / 2)
			{
				findItemToProduce(p_buildOrderMidGame);
			}
		}
	}
}

void ProductionManager::findItemToProduce(BuildOrder & buildOrder)
{
	// Temporary variable to store the items type
	BWAPI::UnitType type = buildOrder[0].type;

	// Update the amount we need to build said item
	p_reservedMinerals += type.mineralPrice();
	p_reservedGas += type.gasPrice();

	// Add the item to the queue
	p_queue.addToQueue(type);

	if (type.isBuilding())
	{
		// Creating new object for the item to be created
		Building b(type, BWAPI::Broodwar->self()->getStartLocation());
		p_buildings.push_back(b);

		BWAPI::Broodwar << b.type.c_str() << std::endl;
	}

	// Remove the item from the build order vector
	buildOrder.removeBuildOrderItem();
}

void ProductionManager::assignWorkerToItem()
{
	for (Building& b : p_buildings)
	{
		if (b.status != BuildingStatus::Unassigned) { continue; }

		BWAPI::Unit builder = Tools::findBuilderUnit(b);

		if (builder)
		{

			b.builderUnit = builder;
			BWAPI::TilePosition desiredPos = b.position;
			int maxBuildRange = 16;
			bool buildingCreep = b.type.requiresCreep();

			BWAPI::TilePosition buildPos = BWAPI::Broodwar->getBuildLocation(b.type, desiredPos, maxBuildRange, buildingCreep);
			
			// TODO LATER ON
			// ---------------------------------
			//p_buildingPlacer.getBuildingLocation(b);

			if (!buildPos.isValid())
			{
				continue;
			}

			b.position = buildPos;

			b.status = BuildingStatus::Assigned;

		}
	}
}


void ProductionManager::issueProduction()
{
	for (auto& b : p_buildings)
	{
		if (b.status != BuildingStatus::Assigned) { continue; }

		if (!b.builderUnit->isConstructing())
		{
			b.builderUnit->build(b.type, b.position);
			b.buildCommandGiven = true;
		}
	}


	/*
	if (type.isBuilding())
	{
		// Attempt to build the type if it's a building
		const bool startedBuilding = Tools::BuildBuilding(type);
		if (startedBuilding)
		{
			BWAPI::Broodwar->printf("Started Building %s", type.getName().c_str());
		}
	}
	else
	{
		// Else if the type is that of a unit then train it
		trainUnit(type);
	}
	*/
}

void ProductionManager::checkForStartedProduction()
{
	for (auto& u : BWAPI::Broodwar->self()->getUnits())
	{
		if (!u->getType().isBuilding() || !u->isBeingConstructed()) { continue; }

		for (auto& b : p_buildings)
		{
			if (b.status != BuildingStatus::Assigned) { continue; }

			if (u->getTilePosition() == b.position)
			{
				p_reservedMinerals	-= b.type.mineralPrice();
				p_reservedGas		-= b.type.gasPrice();


				b.underConstruction = true;
				b.self = u;

				b.builderUnit = nullptr;

				b.status = BuildingStatus::UnderConstruction;

				//p_buildingPlacer.addBuildingPosition(b);

				p_queue.removeFromQueue();

				break;
			}
		}
	}
}

void ProductionManager::trainUnit(BWAPI::UnitType type)
{
	/*
	const int unitsWanted = SOME AMOUNT;
	const int unitsOwned = Tools::CountUnitsOfType(type, BWAPI::Broodwar->self()->getUnits());
	
	if (unitsOwned < unitsWanted) { }
	*/	

	// Get a unit that we own that is of the given type so it can train
	// If we can't find a valid trainer unit, then we have to cancel the training
	const BWAPI::Unit building = Tools::GetUnitOfType(type.whatBuilds().first, false);

	// if we have a valid building unit and it's currently not training something, train a unit
	// there is no reason for a bot to ever use the unit queueing system, it just wastes resources
	if (building && !building->isTraining()) { building->train(type); }
}

void ProductionManager::addToBuildings(BWAPI::Unit unit)
{

}

void ProductionManager::addToUnits(BWAPI::Unit unit)
{
	//p_units or something PROBABLY IN ANOTHER FILE
}

void ProductionManager::removeFromBuildings()
{
	std::vector<Building> toRemove;

	for (auto& b : p_buildings)
	{
		if (b.status != BuildingStatus::UnderConstruction)
		{
			continue;
		}

		if (b.self == nullptr || !b.self->getType().isBuilding() || b.self->getHitPoints() <= 0)
		{
			toRemove.push_back(b);
		}
	}

	removeBuildings(toRemove);
	// If the building successfully constructed, 
	//all actors should be either 'reset' or nullptr
}

void ProductionManager::removeBuildings(const std::vector<Building>& toRemove)
{
	for (auto& b : toRemove)
	{
		auto& it = std::find(p_buildings.begin(), p_buildings.end(), b);

		if (it != p_buildings.end())
		{
			p_buildings.erase(it);
		}
	}
}

void ProductionManager::removeFromUnits(BWAPI::Unit unit)
{
	/*
	auto& it = std::find(p_units.begin(), p_units.end(), unit);

	if (it != p_units.end())
	{
		p_units.erase(it);
	}
	*/
}

int ProductionManager::getReservedMinerals()
{
	return p_reservedMinerals;
}	

int ProductionManager::getReservedGas()
{
	return p_reservedGas;
}

