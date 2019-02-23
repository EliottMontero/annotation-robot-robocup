#pragma once

#include <traitement/robotInformation.h>
#include <traitement/position.h>

namespace traitement
{
 
  class Team {

  public:
    
    Team();
    ~Team();

    void AddRobot(int robot_id);
    bool IsRobot(int robot_id);
    RobotInformation GetRobot(int robot_id);
    void setRobotTeam(int robot_id, int team_id);
    void RobotUpdate(int robot_id, Position pos);
    void setRobotNum(int robot_id);
    //    Position getRobotPosition(int robot_id);
  private:
    std::map<int, RobotInformation>robots;
  };
}
