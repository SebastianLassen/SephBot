#pragma once

#include "Common.h"
#include "BaseManager.h"

#include "../BWEM 1.4.1/src/bwem.h"

namespace WorkerJob
{
	enum { Unassigned = 0, Minerals = 1, Gas = 2, Scouting = 4, Reserved = 5 };
}


class Workers
{
	friend class Global;

	std::map<BWAPI::Unit, int>			p_mineralPatches;
	std::map<BWAPI::Unit, BWAPI::Unit>	p_assignedPatch;

public:
	
	Workers();

	void		initialWorkerToMinerals();
	void		assignToMinerals(BWAPI::Unit unit);
	void		removeFromResource(BWAPI::Unit unit);
	void		removeResource(BWAPI::Unit unit);
	void		issueGatherOrder();





};