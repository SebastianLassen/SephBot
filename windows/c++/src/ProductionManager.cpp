#include "ProductionManager.h"
#include "Tools.h"

ProductionManager::ProductionManager()
{

}

void ProductionManager::onStart()
{
	p_buildOrder.setBuildOrder();
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

	if (p_buildOrder.getBuildOrderItem().supply <= BWAPI::Broodwar->self()->supplyUsed() / 2)
	{
		if (p_buildOrder.getSize() > 0)
		{
			findItemToProduce();
		}
	}

	if (p_queue.getSize() > 0)
	{
		// Handle the item in front of the queue if there is any
		assignWorkerToItem();
		issueProduction();
		checkForStartedProduction();
	}
}

void ProductionManager::findItemToProduce()
{
	// Temporary variable to store the items type
	BWAPI::UnitType type = p_buildOrder.getBuildOrderItem().type;

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
	}

	// Remove the item from the build order vector
	p_buildOrder.removeBuildOrderItem();
}

void ProductionManager::assignWorkerToItem()
{
	for (Building& b : p_buildings)
	{
		if (b.assigned) { continue; }

		BWAPI::Unit builder = Tools::findBuilderUnit(b);

		if (builder)
		{
			BWAPI::TilePosition desiredPos = b.position;
			int maxBuildRange = 16;
			bool buildingCreep = b.type.requiresCreep();

			BWAPI::TilePosition buildPos = BWAPI::Broodwar->getBuildLocation(b.type, desiredPos, maxBuildRange, buildingCreep);

			if (!buildPos.isValid())
			{
				continue;
			}

			b.position = buildPos;
			b.assigned = true;

		}
	}
}

void ProductionManager::issueProduction()
{
	for (auto& b : p_buildings)
	{
		if (!b.assigned || b.constructing) { continue; }

		b.builderUnit->build(b.type, b.position);

		//NULLPTR???!?!?!?

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
			if (!b.assigned) { continue; }

			if (u->getTilePosition() == b.position)
			{
				p_reservedMinerals	-= b.type.mineralPrice();
				p_reservedGas		-= b.type.gasPrice();

				b.self = u;
				b.constructing = true;
				b.builderUnit = nullptr;

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
	BWAPI::Unit building = Tools::GetUnitOfType(type, false);
	if (!building) { return; }

	// if we have a valid building unit and it's currently not training something, train a unit
	// there is no reason for a bot to ever use the unit queueing system, it just wastes resources
	if (!building->isTraining()) 
	{ 
		const bool startedProduction = building->train(type); 
		if (startedProduction)
		{
			BWAPI::Broodwar->printf("Started Production %s", type.getName().c_str());
		}
	}


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
		if (!b.constructing)
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

