#pragma once

#include <hl_communication/utils.h>
#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>

#include <traitement/robotInformation.h>
#include <traitement/position.h>
#include <traitement/direction.h>
#include <hl_communication/wrapper.pb.h>

using namespace hl_communication;
using namespace hl_monitoring;

namespace traitement
{

  class Team {

  public:

    Team();
    ~Team();

    void AddRobot(int robot_id);
    bool IsRobot(int robot_id);
    RobotInformation GetRobot(int robot_id);
    std::map<int, RobotInformation> getRobotMap();
    void updateRobot(int robot_id, RobotMsg rb_msg);
    void setGCMessage(int robot_id, GCRobotMsg gc_msg);
    void setRobotTeam(int robot_id, int team_id);
    void setRobotNum(int robot_id);
  private:
    std::map<int, RobotInformation>robots;
  };
}
