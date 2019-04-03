#include <annotateImage/annotation.h>

#define SECONDS_TO_MS 1000
#define NEXT_FRAME 30000 //30fps in microseconds
#define INIT_TIME NEXT_FRAME*SECONDS_TO_MS //30s

using namespace annotateImage;

int main() {
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

  if (!manager.isLive()){
    now = manager.getStart();
    begin_time = now;
  }

  while(now-begin_time<INIT_TIME){
    manager.update();
    if (manager.isLive()){
      now = getTimeStamp();
    } else{
      now += NEXT_FRAME;
    }

    MessageManager::Status status = manager.getStatus(now);

    for(const auto & robot_entry : status.robot_messages){
      uint32_t team_id = robot_entry.first.team_id();

      if (teams.find(team_id)==teams.end()){
        Team t1;
        teams[team_id]=t1;
      }

      if (!teams[team_id].isRobot(robot_entry.first.robot_id())){
        teams[team_id].addRobot(robot_entry.first.robot_id());
        teams[team_id].setRobotTeam(robot_entry.first.robot_id(),
                                    team_id);
        teams[team_id].setRobotNum(robot_entry.first.robot_id());
      }

      auto robot_id = robot_entry.first.robot_id();
      auto msg = teams[team_id].getRobot(robot_id).getMessageRobot();
      if (robot_entry.second.time_stamp() !=  msg.time_stamp())
        teams[team_id].updateRobot(robot_id, robot_entry.second);

    }
  }
  std::cout << " In this game : " << std::endl;

  for(auto it_team = teams.begin();it_team != teams.end(); ++it_team){
    auto teamMap = teams[it_team->first].getRobotMap();
    for(auto it_robot = teamMap.begin() ; it_robot != teamMap.end()
                                        ; ++it_robot){
      std::cout << " Robot n° "
                << teams[it_team->first].getRobot(it_robot->first).robot
                << " , team n° "
                << teams[it_team->first].getRobot(it_robot->first).team
                << std::endl;
    }
  }

  return 0;
}
