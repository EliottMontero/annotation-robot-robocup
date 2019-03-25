#include "traitement/robotInformation.h"

namespace traitement
{

  RobotInformation::RobotInformation(){
  }

  RobotInformation::~RobotInformation(){
  }
  void RobotInformation::updateRobotMessage( RobotMsg new_message_by_robot){
    message_by_robot = new_message_by_robot;
  }
  void RobotInformation::updateGCMessage( GCRobotMsg new_message_by_GC)
  {
    message_by_GC = new_message_by_GC;
  }

  /*Detail dans le .h si besoin*/
  Position RobotInformation::getTraceRobot(uint64_t time_stamp){
    /*  Position toSend = oldPos.front();
    oldPos.pop();
    oldPos.push(toSend);*/
    Position pos;
    
    return pos;
  }

  void RobotInformation::stockPos(Position pos,uint64_t time_stamp){
    /* setPosRobot(pos);
       oldPos.push(pos);*/
  }

  void RobotInformation::removePos(uint64_t time_stamp){
    // oldPos.pop();
  }

  RobotMsg RobotInformation::getMessageRobot()
  {
    return message_by_robot;
  }
  
   std::map<uint64_t, Position> RobotInformation::getRobotTrace()
  {
    return robot_trace;
  }
  
  void RobotInformation::setTeam(int team_id){
    team=team_id;
  }
  
  void RobotInformation::setnumRobot(int robot_id){
    robot=robot_id;
  }
  
}
