#pragma once

#include "Common.h"
#include "BWEM 1.4.1/src/bwem.h"

class ChokePoints
{


public:

	ChokePoints(const BWEM::ChokePoint* choke);

	BWEM::ChokePoint*	choke;

	

};