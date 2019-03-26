#include <traitement/annotation.h>


#include <hl_communication/utils.h>
#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/position.h>
#include <traitement/direction.h>
#include <traitement/team.h>
#include <hl_communication/perception.pb.h>


#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>
#include <string>
#include <fstream>

#include <sstream>


using namespace hl_communication;
using namespace hl_monitoring;

using namespace traitement;

int main() {
  Json::Reader reader;
  Json::Value root;
  

  std::ifstream match_settings("match_settings.json");
  if (!match_settings.good())
    throw std::runtime_error("failed to open file match_settings.json");
  match_settings >> root;

  checkMember(root["match_setting"], "config");
  checkMember(root["match_setting"], "field");
  std::string conf = root["match_setting"]["config"].asString();
  std::cout << conf << std::endl;


  MonitoringManager manager;
  manager.loadConfig(conf);

  std::string f = root["match_setting"]["field"].asString();
  Field field;
  field.loadFile(f);

  std::cout << f << std::endl;

  Annotation annotation("annotation_settings.json");
    std::map<int, Team>teams;
// While exit was not explicitly required, run
  uint64_t now = 0;
  uint64_t dt = 30 * 1000;//[microseconds]
  if (!manager.isLive()) {
    now = manager.getStart();
    //time_start = manager.getStart();
  }
  while(manager.isGood()) {
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += dt;
      }
    MessageManager::Status status = manager.getStatus(now);
    std::vector<cv::Scalar> team_colors = {cv::Scalar(255,0,255), cv::Scalar(255,255,0)};
    std::map<uint32_t,cv::Scalar> colors_by_team;

    //std::cout << "Temps de Jeu : " << manager.getTime() << std::endl;
    /*   for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
      std::cout << " team size : " << idx  << std::endl;  
      const GCTeamMsg & team_msg = status.gc_message.teams(idx);
      for (int idxr = 0; idxr < team_msg.robots_size(); idxr++) {
	const GCRobotMsg & robots_msg =team_msg.robots(idxr);

      }
    }
    
    */


      std::map<std::string, CalibratedImage> images_by_source =
	manager.getCalibratedImages(now);
      for (const auto & entry : images_by_source) {
	cv::Mat display_img = entry.second.getImg().clone();
	if (entry.second.isFullySpecified()) {
	  const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	  //field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
	  // Basic drawing of robot estimated position
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
	      teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);
	   	   
	      display_img =annotation.AddAnnotation(camera_information, teams[team_id].GetRobot(robot_entry.first.robot_id()) , display_img, now);
	    }
	}
   
	cv::namedWindow(entry.first, cv::WINDOW_AUTOSIZE);
	cv::imshow(entry.first, display_img);
	cv::waitKey(1);
  
      }

 
  
  }
  return 0;
}
