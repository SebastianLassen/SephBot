#include "Global.h"

#include "MapTools.h"
#include "ProductionManager.h"
#include "Workers.h"
#include "Squads.h"
#include "StateInformation.h"

Global::Global()
{
	init();
}

Global & Global::Instance()
{
	static Global instance;
	return instance;
}

void Global::init()
{
	reset(p_mapTools);
	reset(p_productionManager);
	reset(p_workers);
	reset(p_squads);
	reset(p_stateInfo);
}

void Global::GameStart()
{
	Instance().init();
}

MapTools &				Global::Map()				{ return *get(Instance().p_mapTools); }
ProductionManager &		Global::Production()		{ return *get(Instance().p_productionManager); }
Workers&				Global::Worker()			{ return *get(Instance().p_workers); }
Squads&					Global::Squads()			{ return *get(Instance().p_squads); }
StateInformation&		Global::StateInfo()			{ return *get(Instance().p_stateInfo); }