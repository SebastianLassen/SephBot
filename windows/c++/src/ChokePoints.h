#pragma once

#include "Common.h"
#include "BWEM 1.4.1/src/bwem.h"

class ChokePoints
{


public:

	ChokePoints(const BWEM::ChokePoint* choke);

	const BWEM::ChokePoint*	choke;

	BWAPI::Position center;
	int width;

	BWAPI::Position end1;
	BWAPI::Position middle;
	BWAPI::Position end2;
	

};