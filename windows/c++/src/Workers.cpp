#include "Workers.h"
#include "Global.h"
#include "MapTools.h"
#include "Tools.h"

Workers::Workers()
{

}

void Workers::initialWorkerToMinerals()
{
	auto base = Global::Map().getMainBase();

	auto minerals = base->getMinerals();

	// We only start out with 4 workers.
	minerals.resize(4);
	std::set<BWAPI::Unit> availableMinerals(minerals.begin(), minerals.end());
	
	for (int i(0); i < 4; i++)
	{
		int bestDist			= INT_MAX;
		BWAPI::Unit bestWorker	= nullptr;
		BWAPI::Unit bestPatch	= nullptr;
		for (auto& unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (!unit->getType().isWorker()) { continue; }
			if (!unit->isCompleted())		 { continue; }
			if (p_assignedPatch[unit])       { continue; }

			for (auto& patch : availableMinerals)
			{
				int dist = unit->getDistance(patch);
				if (dist < bestDist)
				{
					bestDist = dist;
					bestWorker = unit;
					bestPatch = patch;
				}
			}
		}

		if (bestWorker && bestPatch)
		{
			p_mineralPatches[bestPatch] += 1;
			p_assignedPatch[bestWorker] = bestPatch;
			availableMinerals.erase(bestPatch);
			bestWorker->rightClick(bestPatch);
		}
	}
}

void Workers::assignToMinerals(BWAPI::Unit unit)
{
	auto base = Global::Map().getMainBase();

	auto minerals = base->getMinerals();

	BWAPI::Unit bestPatch = nullptr;

	for (auto& patch : minerals)
	{
		int assignedWorkers = p_mineralPatches[patch];
		if (assignedWorkers >= 2) { continue; }

		if (assignedWorkers == 0)
		{
			bestPatch = patch;
			break;
		}
		bestPatch = patch;
	}

	if (bestPatch)
	{
		p_mineralPatches[bestPatch] += 1;
		p_assignedPatch[unit] = bestPatch;
	}
	else
	{
		unit->gather(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
	}
}



void Workers::removeFromResource(BWAPI::Unit unit)
{
	BWAPI::Unit patch = p_assignedPatch[unit];

	p_assignedPatch.erase(unit);

	auto& it = p_mineralPatches.find(patch);

	if (it != p_mineralPatches.end())
	{
		if (p_mineralPatches[patch] - 1 < 0)
		{
			p_mineralPatches[patch] = 0;
		}
		else
		{
			p_mineralPatches[patch] -= 1;
		}
	}

}

void Workers::removeResource(BWAPI::Unit unit)
{
	auto& it = p_assignedPatch.find(unit);
	if (it != p_assignedPatch.end())
	{
		p_assignedPatch.erase(it);
	}


	auto& mineralIt = p_mineralPatches.find(unit);
	if (mineralIt != p_mineralPatches.end())
	{
		p_mineralPatches.erase(mineralIt);
	}
}

void Workers::issueGatherOrder()
{
	auto base = Global::Map().getMainBase();
	if (base->minerals() == 0)
	{
		p_assignedPatch.clear();
		p_mineralPatches.clear();
		return;
	}
	for (auto& patch : p_mineralPatches)
	{
		for (auto& worker : BWAPI::Broodwar->self()->getUnits())
		{
			if (!worker->getType().isWorker()) { continue; }
			if (!worker->isCompleted())		   { continue; }

			if (worker->getDistance(Tools::GetDepot()) > 500)
			{
				removeFromResource(worker);
				continue;
			}

			if (worker->isIdle() && worker->isCarryingMinerals())
			{
				worker->returnCargo();
				continue;
			}

			if (worker->isIdle())
			{
				worker->rightClick(p_assignedPatch[worker]);
				continue;
			}

			if (worker->getOrder() == BWAPI::Orders::MiningMinerals &&
				worker->getOrderTarget() != p_assignedPatch[worker])
			{
				worker->rightClick(p_assignedPatch[worker]);
				continue;
			}
		}
	}
}
