#pragma once

#include <annotateImage/robotInformation.h>

namespace annotateImage {
  class Team {

  public :
    Team();
    ~Team();

    void addRobot(int robot_id);
    bool isRobot(int robot_id);

    void setScore(uint32_t score);
    uint32_t getScore();

    RobotInformation getRobot(int robot_id);
    std::map<int, RobotInformation> getRobotMap();
    void updateRobot(int robot_id, RobotMsg rb_msg);
    void setGCMessage(int robot_id, GCRobotMsg gc_msg);
    void setRobotTeam(int robot_id, int team_id);
    void setRobotNum(int robot_id);

  private : 
    uint32_t team_score;
    std::map<int, RobotInformation>robots;

  };
}
