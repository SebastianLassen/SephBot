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

	//bool isNarrowChoke;
	//int length;
	BWAPI::Position end1;
	BWAPI::Position middle;
	BWAPI::Position end2;
	//std::vector<int> tileSide; // Assigns each map tile within a certain area of the choke a "side". -2 = side 1, 1 = side 2, 0 = inside choke
	//std::set<BWAPI::TilePosition> chokeTiles; // Tiles inside and close to the ends of the choke	



	

};