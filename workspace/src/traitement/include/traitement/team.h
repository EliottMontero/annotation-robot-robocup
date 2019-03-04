#pragma once

#include <traitement/robotInformation.h>
#include <traitement/position.h>
#include <traitement/direction.h>

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
    void setRobotPosBall(int robot_id, Position pos);
    //    void setRobotPosRobot(int robot_id, Position pos);
    void setRobotDirRobot(int robot_id, Direction dir);
  private:
    std::map<int, RobotInformation>robots;
  };
}
