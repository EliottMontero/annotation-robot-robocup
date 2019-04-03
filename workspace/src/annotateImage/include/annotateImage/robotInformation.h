#pragma once

#include <annotateImage/position.h>
#include <annotateImage/direction.h>

#include <hl_communication/wrapper.pb.h>

using namespace hl_communication;

namespace annotateImage {
  class RobotInformation {

  public :
    int team;
    int robot;
    std::map<uint64_t, Position> robot_trace;

    RobotInformation();
    ~RobotInformation();

    std::map<uint64_t, Position> getRobotTrace();
    Position getTraceRobot(uint64_t time_stamp);
    RobotMsg getMessageRobot();

    void setTeam(int team_id);
    void setnumRobot(int robot_id);

    void updateRobotMessage(RobotMsg new_message_by_robot);
    void updateRobotTrace(RobotMsg new_message_by_robot);

    /*Still not use but implement for extensions*/
    void updateGCMessage(GCRobotMsg new_message_by_GC);

    void stockPos(Position pos, uint64_t time_stamp);
    void removeOnePos(uint64_t time_stamp);
    void removeFiewPos(uint64_t time_stamp);

  private :
    RobotMsg message_by_robot;
    GCRobotMsg message_by_GC;

  };
}
