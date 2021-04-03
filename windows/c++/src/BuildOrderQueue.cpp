#include "BuildOrderQueue.h"

BuildOrderQueue::BuildOrderQueue()
{

}

void BuildOrderQueue::addToQueue(BWAPI::UnitType type)
{
	if (p_queue.empty())
	{
		p_queue.push_front(type);
	}
	else
	{
		p_queue.push_back(type);
	}
}

void BuildOrderQueue::removeFromQueue()
{
	p_queue.pop_front();
}

size_t BuildOrderQueue::getSize()
{
	return p_queue.size();
}

bool BuildOrderQueue::isEmpty()
{
	return p_queue.empty();
}

const BWAPI::UnitType BuildOrderQueue::peekQueue() const
{
	return p_queue.front();
}

