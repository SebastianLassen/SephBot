#include "Global.h"

#include "MapTools.h"
#include "ProductionManager.h"

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
}

void Global::GameStart()
{
	Instance().init();
}

MapTools &				Global::Map()				{ return *get(Instance().p_mapTools); }
ProductionManager &		Global::Production()		{ return *get(Instance().p_productionManager); }
