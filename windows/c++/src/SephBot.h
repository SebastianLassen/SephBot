#pragma once

#include "Common.h"

#include <windows.h>


class SephBot
{

	BWAPI::Unit p_scout = nullptr;
	std::map<int, BWAPI::TilePosition> startingLocationMap;
	
	//int mineralCounting = 0; test variable for mineral optimization test

public:

    SephBot();

    // helper functions to get you started with bot programming and learn the API
    void sendIdleWorkersToMinerals();
    void trainAdditionalWorkers();
    void buildAdditionalSupply();
    void drawDebugInformation();

	// extended
	void sendScout();
	void sendUnitsToAttack();

    // functions that are triggered by various BWAPI events from main.cpp
	void onStart();
	void onFrame();
	void onEnd(bool isWinner);
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitMorph(BWAPI::Unit unit);
	void onSendText(std::string text);
	void onUnitCreate(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onUnitShow(BWAPI::Unit unit);
	void onUnitHide(BWAPI::Unit unit);
	void onUnitRenegade(BWAPI::Unit unit);

};