// BZMinLogger.cpp
//

// Standard C++ headers
#include <iostream>
#include <ostream>

// 
#include "bzfsAPI.h"

std::string bzTeamToString(bz_eTeamType team)
{
    if (team == eRogueTeam) {
        return "Rogue Team";
    } else if (team == eRedTeam) {
        return "Red Team";
    } else if (team == eGreenTeam) {
        return "Green Team";
    } else if (team == eBlueTeam) {
        return "Blue Team";
    } else if (team == ePurpleTeam) {
        return "Purple Team";
    } else if (team == eRabbitTeam) {
        return "Rabbit Team";
    } else if (team == eHunterTeam) {
        return "Hunter Team";
    } else if (team == eObservers) {
        return "Observer";
    } else {
        return "Non-Team";
    }
}

class BZMinLogger : public bz_Plugin
{
public:
    virtual const char* Name(){return "BZMinLogger";}
    virtual void Init ( const char* /*config*/ );
    virtual void Event(bz_EventData *eventData );
    virtual void Cleanup ( void );
    bool admin = true; // Logs as "admin".

};

BZ_PLUGIN(BZMinLogger)

void BZMinLogger::Init (const char* config) {
    Register(bz_ePlayerJoinEvent);
    Register(bz_ePlayerPartEvent);
    Register(bz_ePlayerPausedEvent);
    Register(bz_ePlayerSpawnEvent);
}

void BZMinLogger::Cleanup (void) {
    Flush();
}

void BZMinLogger::Event(bz_EventData *eventData ){
    switch (eventData->eventType) {

     case bz_ePlayerJoinEvent: {
        bz_PlayerJoinPartEventData_V1* joinData = (bz_PlayerJoinPartEventData_V1*)eventData;
        std::string playerCallsign = joinData->record->callsign;
        std::string playerIP = joinData->record->ipAddress;
        bz_eTeamType team = joinData->record->team;
        if (admin == false) {
            std::cout << "*** \'" << playerCallsign << "\' joined the game as " << bzTeamToString(team) << "." << std::endl;
        } else {
            std::cout << "*** \'" << playerCallsign << "\' joined the game as " << bzTeamToString(team) << " from " << playerIP << "." << std::endl;
        }
        
     }break;
    
     case bz_ePlayerPartEvent: {
        bz_PlayerJoinPartEventData_V1* partData = (bz_PlayerJoinPartEventData_V1*)eventData;
        std::string playerCallsign = partData->record->callsign;
        std::cout << "*** '" << playerCallsign << "' left the game." << std::endl;
        
     }break;
    
     case bz_ePlayerPausedEvent: {
        bz_PlayerPausedEventData_V1 *pauseData = (bz_PlayerPausedEventData_V1*)eventData;
        std::string playerCallsign = bz_getPlayerCallsign(pauseData->playerID);
        if (pauseData->pause == true) { 
            std::cout << "*** '" << playerCallsign << "': paused." << std::endl;
        } else if (pauseData->pause == false) {
            std::cout << "*** '" << playerCallsign << "': resumed." << std::endl;
        } else {
            // WTH happened here?
        }
        
     }break;
     
     case bz_ePlayerSpawnEvent: {
        bz_PlayerSpawnEventData_V1* spawnData = (bz_PlayerSpawnEventData_V1*)eventData;
        std::string playerCallsign = bz_getPlayerCallsign(spawnData->playerID);
        std::cout << "*** '" << playerCallsign << "' has respawned." << std::endl;
        
     }break;

     default: {
     }break;
     
    }
}
