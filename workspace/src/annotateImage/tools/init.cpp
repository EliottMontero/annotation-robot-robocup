#include <annotateImage/annotation.h>


#include <hl_communication/utils.h>
#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <annotateImage/position.h>
#include <annotateImage/direction.h>
#include <annotateImage/team.h>
#include <hl_communication/perception.pb.h>


#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>
#include <string>
#include <fstream>

#include <sstream>

#define SECONDS_TO_MS 1000
#define NEXT_FRAME 30000 //30fps in microseconds  
#define INIT_TIME NEXT_FRAME*SECONDS_TO_MS //30s

using namespace hl_communication;
using namespace hl_monitoring;

using namespace annotateImage;

int main() {
  Json::Reader reader;
  Json::Value root;
  

  std::ifstream match_settings("match_settings.json");
  if (!match_settings.good())
    throw std::runtime_error("failed to open file match_settings.json");
  match_settings >> root;

  checkMember(root["match_setting"], "config");
  
  std::string conf = root["match_setting"]["config"].asString();
  std::cout << conf << std::endl;


  MonitoringManager manager;
  manager.loadConfig(conf);

  std::map<int, Team>teams;
  uint64_t now = 0;
  uint64_t begin_time = now;  

  if (!manager.isLive()) {
    now = manager.getStart();
    begin_time = now;
  }
  while(now-begin_time<INIT_TIME) {
    
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += NEXT_FRAME;
    }

    MessageManager::Status status = manager.getStatus(now);
    
    for (const auto & robot_entry : status.robot_messages) 
      {
	uint32_t team_id = robot_entry.first.team_id();
	if (teams.find(team_id)==teams.end()){
	  Team t1;
	  teams[team_id]=t1;
	}
	if (!teams[team_id].IsRobot(robot_entry.first.robot_id())){
	  teams[team_id].AddRobot(robot_entry.first.robot_id());
	  teams[team_id].setRobotTeam(robot_entry.first.robot_id(),team_id);
	  teams[team_id].setRobotNum(robot_entry.first.robot_id());
	}

	if (robot_entry.second.time_stamp() !=  teams[team_id].getRobot(robot_entry.first.robot_id()).getMessageRobot().time_stamp())
	  teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);
      }
    
  }
  std::cout << " In this game : " << std::endl;
  
  for (std::map<int, Team>::iterator it_team=teams.begin();it_team != teams.end(); ++it_team)
    {
      for (std::map<int, RobotInformation>::iterator it_robot=teams[it_team->first].getRobotMap().begin();it_robot != teams[it_team->first].getRobotMap().end(); ++it_robot)
	{
	  std::cout << " Robot n° " << teams[it_team->first].getRobot(it_robot->first).robot << " , team n° " << teams[it_team->first].getRobot(it_robot->first).team << std::endl;
	}

    }
  
	  
  return 0;
}
