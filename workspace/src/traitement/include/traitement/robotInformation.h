#pragma once

#include <hl_communication/utils.h>
#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>

#include <traitement/position.h>
#include <traitement/direction.h>
#include <hl_communication/wrapper.pb.h>

#include <queue>       

using namespace hl_communication;
using namespace hl_monitoring;

namespace traitement
{
  /* Dans cette classe, nous stockons les informations des robots que l'on obtient dans les messages*/
  class RobotInformation {

  public:
    int team;
    int robot;
    std::map<uint64_t, Position> robot_trace;


    
    RobotInformation();
    ~RobotInformation();
    void updateRobotMessage( RobotMsg new_message_by_robot);
    void updateGCMessage( GCRobotMsg new_message_by_GC);
    void updateRobotTrace(RobotMsg new_message_by_robot);
    
    /*retourne la position en haut de la queue, la place en bas de la queue
      lorsqxu'on lance cette fonction on la lance sizeOfQueue fois,
      le fait de lire le haut et de le mettre ensuite au bout de la queue
      nous permet de lire chaque position une fois puis de revenir comme avant la lecture*/
    Position getTraceRobot(uint64_t time_stamp);
    /*add a postion in the map*/
    void stockPos(Position pos, uint64_t time_stamp);
    RobotMsg getMessageRobot();
    std::map<uint64_t, Position> getRobotTrace();
    void setTeam(int team_id);
    void setnumRobot(int robot_id);
    
  private:
    RobotMsg message_by_robot;
    GCRobotMsg message_by_GC;
    /*To stock the old positions of the robot*/
  

  };

}
