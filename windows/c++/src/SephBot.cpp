#include "SephBot.h"
#include "Tools.h"
#include "MapTools.h"
#include "BuildingData.h"
#include "Global.h"
#include "ProductionManager.h"
#include "Workers.h"

#include "BWEM 1.4.1/src/bwem.h"
#include <iostream>

SephBot::SephBot()
{
    Global::GameStart();
}

// Called when the bot starts!
void SephBot::onStart()
{
    // Set our BWAPI options here    
	BWAPI::Broodwar->setLocalSpeed(10);
    BWAPI::Broodwar->setFrameSkip(0);
    
    // Enable the flag that tells BWAPI top let users enter input while bot plays
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

    // Call MapTools OnStart
    Global::Map().onStart();
    Global::Production().onStart();
    Global::Worker().initialWorkerToMinerals();

}

// Called whenever the game ends and tells you if you won or not
void SephBot::onEnd(bool isWinner)
{
    std::cout << "We " << (isWinner ? "won!" : "lost!") << "\n";
}

// Called on each frame of the game
void SephBot::onFrame()
{
    // Update our MapTools information
    Global::Map().onFrame();

    // Send our idle workers to mine minerals so they don't just stand there
    sendIdleWorkersToMinerals();

    // Send a worker to scout
    if (BWAPI::Broodwar->self()->supplyUsed() / 2 >= 14) { sendScout(); }

    // Follow the build order
    Global::Production().onFrame();

    // Train more workers so we can gather more income
    trainAdditionalWorkers();

    // Build more supply if we are going to run out soon
    buildAdditionalSupply();

    // Draw unit health bars, which brood war unfortunately does not do
    //Tools::DrawUnitHealthBars();

    // Send combat units to attack
    sendUnitsToAttack();

    // Draw some relevent information to the screen to help us debug the bot
    drawDebugInformation();

    /* TO MAKE SURE THAT ONFRAME ISNT RUN TOO MUCH - MIGHT NEED LATER
    if (BWAPI::Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
        return;
    */

}

// Send our idle workers to mine minerals so they don't just stand there
void SephBot::sendIdleWorkersToMinerals()
{
    // Let's send all of our starting workers to the closest mineral to them
    // First we need to loop over all of the units that we (BWAPI::Broodwar->self()) own
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();
    for (auto& unit : myUnits)
    {
        // Check the unit type, if it is an idle worker, then we want to send it somewhere
        if (unit->getType().isWorker() && unit->isIdle())
        {
            Global::Worker().assignToMinerals(unit);
            //unit->gather(unit->getClosestUnit(BWAPI::Filter::IsMineralField || BWAPI::Filter::IsRefinery));
            /*
            // Get the closest mineral to this worker unit
            BWAPI::Unit closestMineral = Tools::GetClosestUnitTo(unit, BWAPI::Broodwar->getMinerals());

            // If a valid mineral was found, right click it with the unit in order to start harvesting
            if (closestMineral) { unit->rightClick(closestMineral); }
            */
        }
    }
    //Global::Worker().issueGatherOrder();
}

// Train more workers so we can gather more income
void SephBot::trainAdditionalWorkers()
{
    const int mineralsAvailable = BWAPI::Broodwar->self()->minerals() - Global::Production().getReservedMinerals();
    
    const BWAPI::UnitType workerType = BWAPI::Broodwar->self()->getRace().getWorker();
    if (mineralsAvailable < workerType.mineralPrice()) { return; }

    const int workersWanted = 20;
    const int workersOwned = Tools::CountUnitsOfType(workerType, BWAPI::Broodwar->self()->getUnits());
    if (workersOwned < workersWanted)
    {
        // get the unit pointer to my depot
        const BWAPI::Unit myDepot = Tools::GetDepot();

        // if we have a valid depot unit and it's currently not training something, train a worker
        // there is no reason for a bot to ever use the unit queueing system, it just wastes resources
        if (myDepot && !myDepot->isTraining()) { myDepot->train(workerType); }
    }
}

// Build more supply if we are going to run out soon
void SephBot::buildAdditionalSupply()
{
    // Get the amount of supply supply we currently have unused
    const int unusedSupply = Tools::GetTotalSupply(true) - BWAPI::Broodwar->self()->supplyUsed();

    // If we have a sufficient amount of supply, we don't need to do anything
    if (unusedSupply >= 2) { return; }

    // Otherwise, we are going to build a supply provider
    const BWAPI::UnitType supplyProviderType = BWAPI::Broodwar->self()->getRace().getSupplyProvider();

    const bool startedBuilding = Tools::BuildBuilding(supplyProviderType);
    if (startedBuilding)
    {
        BWAPI::Broodwar->printf("Started Building %s", supplyProviderType.getName().c_str());
    }
}

// Draw some relevent information to the screen to help us debug the bot
void SephBot::drawDebugInformation()
{
    BWAPI::Broodwar->drawTextScreen(BWAPI::Position(10, 10), "Hello, World!\n");
    Tools::DrawUnitCommands();
    Tools::DrawUnitBoundingBoxes();
}

void SephBot::sendScout()
{
    if (Global::Map().isEnemyBaseFound())
    {
        return;
    }

    if (!p_scout)
    {
        p_scout = Tools::GetUnitOfType(BWAPI::Broodwar->self()->getRace().getWorker(), true);
    }

    BWAPI::TilePosition tempPos = Global::Map().getEnemyStartLocation();

    if (tempPos != BWAPI::TilePositions::None)
    {
        if (BWAPI::Broodwar->isVisible(tempPos) || BWAPI::Broodwar->isExplored(tempPos))
        {
            for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
            {
                if (unit->getType().isResourceDepot() && unit->getTilePosition() == tempPos)
                {
                    Global::Map().isEnemyBaseFound(true);
                    Global::Map().setEnemyStartLocation(tempPos);
                    BWAPI::Broodwar << "Found enemy base" << std::endl;
                    p_scout->move(BWAPI::Position(Global::Map().getSelfStartLocation()));
                    return;
                }
            }
        }
    }

    for (auto startLocation : BWAPI::Broodwar->getStartLocations())
    {
        if (!BWAPI::Broodwar->isExplored(startLocation) && p_scout)
        {
            Global::Map().setEnemyStartLocation(startLocation);
            p_scout->move(BWAPI::Position(startLocation));
            BWAPI::Broodwar->drawCircleMap(BWAPI::Position(startLocation), 32, BWAPI::Colors::Purple);
            break;
        }
    }


    /*
    for (BWAPI::TilePosition tp : startLocations)
    {
        if (BWAPI::Broodwar->isExplored(tp)) 
        { 
            for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
            {
                if (unit->getType().isResourceDepot() && unit->getTilePosition() == tp)
                {
                    p_mapTools.setEnemyStartLocation(tp);
                    BWAPI::Position pos(p_mapTools.getSelfStartLocation());
                    p_scout->move(pos);
                    break;
                }
            }
        }
        else
        {
            if (p_scout)
            {
                BWAPI::Position pos(tp);

                p_scout->move(pos);
                break;
            }
        }
    }
    */
}

void SephBot::sendUnitsToAttack()
{
    if (Tools::CountUnitsOfType(BWAPI::UnitTypes::Protoss_Zealot, BWAPI::Broodwar->self()->getUnits()) < 3)
    {
        return;
    }

    if (!Global::Map().isEnemyBaseFound())
    {
        return;
    }

    BWAPI::Broodwar << "In here" << std::endl;

    BWAPI::Position pos(Global::Map().getEnemyStartLocation());
    const BWAPI::Unitset& myUnits = BWAPI::Broodwar->self()->getUnits();

    for (auto& unit : myUnits)
    {
        if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->isIdle())
        {
            unit->attack(pos);
        }
    }
}

// Called whenever a unit is destroyed, with a pointer to the unit
void SephBot::onUnitDestroy(BWAPI::Unit unit)
{
    // MIGHT MOVE THIS LATER TO CLEAR UP SEPHBOT.CPP
    if (unit->getPlayer() == BWAPI::Broodwar->self())
    {
        if (unit->getType().isWorker())
        {
            Global::Worker().removeFromResource(unit);
        }
    }
    
}

// Called whenever a unit is morphed, with a pointer to the unit
// Zerg units morph when they turn into other units
void SephBot::onUnitMorph(BWAPI::Unit unit)
{
	
}

// Called whenever a text is sent to the game by a user
void SephBot::onSendText(std::string text)
{ 
    if (text == "/map")
    {
        Global::Map().toggleDraw();
    }
}

// Called whenever a unit is created, with a pointer to the destroyed unit
// Units are created in buildings like barracks before they are visible, 
// so this will trigger when you issue the build command for most units
void SephBot::onUnitCreate(BWAPI::Unit unit)
{ 

}

// Called whenever a unit finished construction, with a pointer to the unit
void SephBot::onUnitComplete(BWAPI::Unit unit)
{

}

// Called whenever a unit appears, with a pointer to the destroyed unit
// This is usually triggered when units appear from fog of war and become visible
void SephBot::onUnitShow(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit gets hidden, with a pointer to the destroyed unit
// This is usually triggered when units enter the fog of war and are no longer visible
void SephBot::onUnitHide(BWAPI::Unit unit)
{ 
	
}

// Called whenever a unit switches player control
// This usually happens when a dark archon takes control of a unit
void SephBot::onUnitRenegade(BWAPI::Unit unit)
{ 
	
}



/*              EXAMPLE OF A COUNTDOWN TIMER USING REGISTER EVENT
void ExampleAIModule::onStart()
{
// Register a callback that only occurs once when the countdown timer reaches 0
if ( BWAPI::Broodwar->getGameType() == BWAPI::GameTypes::Capture_The_Flag ||
BWAPI::Broodwar->getGameType() == BWAPI::GameTypes::Team_Capture_The_Flag )
 {
BWAPI::Broodwar->registerEvent([](BWAPI::Game*){ BWAPI::Broodwar->sendText("Try to find my flag!"); }, // action
 [](BWAPI::Game*){ returnBWAPI::Broodwar->countdownTimer() == 0; }, // condition
 1); // times to run (once)
 }
}



*/