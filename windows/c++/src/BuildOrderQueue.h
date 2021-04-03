#pragma once

#include <BWAPI.h>
#include <vector>
#include <deque>

class BuildOrderQueue
{

	std::deque<BWAPI::UnitType> p_queue;

public:

	BuildOrderQueue();

	void					addToQueue(BWAPI::UnitType type);
	void					removeFromQueue();
	size_t					getSize();
	bool					isEmpty();
	const BWAPI::UnitType	peekQueue() const;

};