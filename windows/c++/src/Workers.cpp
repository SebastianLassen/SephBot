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

	auto mineralPatches = base->getMinerals();

	// We only start out with 4 workers.
	mineralPatches.resize(4);
	std::set<BWAPI::Unit> availablePatches(mineralPatches.begin(), mineralPatches.end());
	
	for (int i(0); i < 4; i++)
	{
		int bestDist			= INT_MAX;
		BWAPI::Unit bestWorker	= nullptr;
		BWAPI::Unit bestPatch	= nullptr;
		for (auto& unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (!unit->getType().isWorker()) { continue; }
			if (!unit->isCompleted())		 { continue; }
			if (!unit->isIdle())			 { continue; }

			for (auto& patch : availablePatches)
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
			p_mineralPatches[bestPatch].push_back(bestWorker);
			availablePatches.erase(bestPatch);
			bestWorker->gather(bestPatch);
		}
	}
}

void Workers::assignToMinerals(BWAPI::Unit unit)
{
	auto base = Global::Map().getMainBase();

	auto mineralPatches = base->getMinerals();

	for (auto& patch : mineralPatches)
	{
		int assignedWorkers = p_mineralPatches[patch].size();
		if (assignedWorkers == 3) { continue; }

		p_mineralPatches[patch].push_back(unit);
		unit->gather(patch);
		break;
	}
}

void Workers::removeFromResource(BWAPI::Unit unit)
{
	for (auto& patch : p_mineralPatches)
	{
		auto& it = std::find(patch.second.begin(), patch.second.end(), unit);

		if (it != patch.second.end())
		{
			BWAPI::Broodwar << unit->getType().getName() << std::endl;
			patch.second.erase(it);
			return;
		}
	}
}
