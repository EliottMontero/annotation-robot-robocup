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

  void RobotInformation::updateRobotTrace(RobotMsg message_by_robot)
  {
    if (message_by_robot.has_perception()) {
	  const Perception & perception = message_by_robot.perception();
	  const WeightedPose & weighted_pose = perception.self_in_field(0);	  
	  if (weighted_pose.pose().has_position())
	    {
	      const PositionDistribution & position = weighted_pose.pose().position();
	      Position pos;
	      pos.setPosition(position.x(),position.y(), message_by_robot.time_stamp());
	      stockPos(pos, message_by_robot.time_stamp());
	      
	    }
    }
    
  }
  

  /*Detail dans le .h si besoin*/
  Position RobotInformation::getTraceRobot(uint64_t time_stamp){
    
    return robot_trace[time_stamp];
  }

  void RobotInformation::stockPos(Position pos,uint64_t time_stamp){
    robot_trace[time_stamp]=pos;
  }
  void RobotInformation::removeOnePos(uint64_t time_stamp)
  {
    robot_trace.erase(time_stamp);
  }
  
  void RobotInformation::removeFiewPos(uint64_t time_stamp)
  {
    auto it=robot_trace.upper_bound(time_stamp);
    robot_trace.erase(it, robot_trace.end());
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
