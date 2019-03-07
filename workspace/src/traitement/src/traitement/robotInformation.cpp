#include "traitement/robotInformation.h"

namespace traitement
{

  RobotInformation::RobotInformation(){
    team = 0;
    numRobotInformation = 0;
    penalty = 0;
    secs_till_unpenalised = 0;
    yellow_card_count = 0;
    red_card_count = 0;
  }

  RobotInformation::~RobotInformation(){
  }

  void RobotInformation::setTeam(int team_id){
    team=team_id;
  }
  void RobotInformation::setnumRobotInformation(int robot_id){
    numRobotInformation=robot_id;
  }
  
  void RobotInformation::setPosRobot(Position pos){
   robot_pos = pos;
  }

    
  void RobotInformation::setPosBall(Position pos){
   ball_pos = pos;
  }
  
  void RobotInformation::setPosTarget(Position pos){
   target_pos = pos;
  }
  
  void RobotInformation::setDirRobot(Direction dir){
    robot_dir = dir;
  }

  Position RobotInformation::getPosBall(){
    return ball_pos;
  }

  Position RobotInformation::getPosRobot(){
    return robot_pos;
  }

  Position RobotInformation::getPosTarget(){
    return target_pos;
  }
  
  Direction RobotInformation::getDirRobot(){
    return robot_dir;
  }
  
  int RobotInformation::getTeam() const{
    return team;
  }
  int RobotInformation::getNumRobotInformation() const{
    return numRobotInformation;
  }

  int RobotInformation::getPenalty() const{
    return penalty;
  }

  int RobotInformation::getSecsTillUnpenalised() const{
    return secs_till_unpenalised;
  }

  int RobotInformation::getYellowCardCount() const{
    return yellow_card_count;
  }

  int RobotInformation::getRedCardCount() const{
    return red_card_count;
  }
    
  int RobotInformation::sizeOfQueue(){
    return oldPos.size();
  }

  /*Detail dans le .h si besoin*/
  Position RobotInformation::getTraceRobot(){
    Position toSend = oldPos.front();
    oldPos.pop();
    oldPos.push(toSend);
    return toSend;
  }

  void RobotInformation::stockPos(Position pos){
    setPosRobot(pos);
    oldPos.push(pos);
  }

  void RobotInformation::removePos(){
    oldPos.pop();
  }

}
