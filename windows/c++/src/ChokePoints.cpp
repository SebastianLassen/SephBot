#include "ChokePoints.h"

ChokePoints::ChokePoints(const BWEM::ChokePoint* choke)
	: choke(choke)
	, center(BWAPI::Position(choke->Center()) + BWAPI::Position(4, 4))
	, width(-1)
	//, isNarrowChoke(false)
	//, length(-1)
	, end1(BWAPI::Position(choke->Pos(choke->end1)))
	, middle(BWAPI::Position(choke->Pos(choke->middle)))
	, end2(BWAPI::Position(choke->Pos(choke->end2)))
{


	width = (int) BWAPI::Position(choke->Pos(choke->end1)).getDistance(BWAPI::Position(choke->Pos(choke->end2))) + 15;

}

