#include "traitement/team.h"


namespace traitement
{

  Team::Team(){
    std::map<int, RobotInformation>robots;
  }

  Team::~Team(){
}
  
  void Team::AddRobot(int robot_id){
    if (robots.find(robot_id)==robots.end()){
      RobotInformation rb;
      robots[robot_id]=rb;
    }
  }
  bool Team::IsRobot(int robot_id){
     if (robots.find(robot_id)==robots.end())
       return false;
     else
       return true;
  }

  RobotInformation Team::GetRobot(int robot_id){
    return robots[robot_id];
  }

  void Team::setRobotTeam(int robot_id, int team_id){
    robots[robot_id].setTeam(team_id);
  }
  
  void Team::setRobotNum(int robot_id){
    robots[robot_id].setnumRobotInformation(robot_id);
  }
  void Team::setRobotPos(int robot_id, Position pos){
    robots[robot_id].stockPos(pos);
  }

  void Team::setRobotPosBall(int robot_id, Position pos){
    robots[robot_id].setPosBall(pos);
  }

  void Team::setRobotPosTarget(int robot_id, Position pos){
    robots[robot_id].setPosTarget(pos);
  }
  
  void Team::setRobotDirRobot(int robot_id, Direction dir){
    robots[robot_id].setDirRobot(dir);
  }
}
