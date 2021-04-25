#pragma once

#include "Common.h"
#include "BaseManager.h"

#include "../BWEM 1.4.1/src/bwem.h"

namespace WorkerJob
{
	enum { Unassigned = 0, Minerals = 1, Gas = 2, Reserved = 3 };
}


class Workers
{
	friend class Global;

	std::map<BWAPI::Unit, std::vector<BWAPI::Unit>> p_mineralPatches;

public:
	
	Workers();

	void		initialWorkerToMinerals();
	void		assignToMinerals(BWAPI::Unit unit);



};