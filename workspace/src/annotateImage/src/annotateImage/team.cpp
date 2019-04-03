#include "annotateImage/team.h"


namespace annotateImage
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

  void Team::setScore(uint32_t score)
  {
    if (team_score!=score)
      {
	team_score = score;
      }
  }

  uint32_t Team::getScore()
  {
    return team_score;
  }
  

  RobotInformation Team::getRobot(int robot_id){
    return robots[robot_id];
  }

  std::map<int,RobotInformation> Team::getRobotMap(){
    std::map<int,RobotInformation> copy_robots;
    for(auto robot : robots){
      copy_robots[robot.first] = robot.second;
    }
    return copy_robots;
  }

  void Team::updateRobot(int robot_id, RobotMsg rb_msg)
  {
    
    robots[robot_id].updateRobotMessage(rb_msg);
    robots[robot_id].updateRobotTrace(rb_msg);
    
  }

  void Team::setGCMessage(int robot_id, GCRobotMsg gc_msg)
  {
    
    robots[robot_id].updateGCMessage(gc_msg);
  }

  void Team::setRobotTeam(int robot_id, int team_id){
    robots[robot_id].setTeam(team_id);
  }

  void Team::setRobotNum(int robot_id){
    robots[robot_id].setnumRobot(robot_id);
  }

}
