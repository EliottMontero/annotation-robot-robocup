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

#define SECONDS_TO_MS 1000
#define NEXT_FRAME 30000 //30fps in microseconds  


using namespace hl_communication;
using namespace hl_monitoring;

using namespace traitement;

int main(int argc, char ** argv) {
  Json::Reader reader;
  Json::Value root;
  

  std::ifstream match_settings("match_settings.json");
  if (!match_settings.good())
    throw std::runtime_error("failde to open file match_settings.json");
  match_settings >> root;

  checkMember(root["match_setting"], "config");
  std::string conf = root["match_setting"]["config"].asString();
  std::cout << conf << std::endl;


  MonitoringManager manager;
  manager.loadConfig(conf);

  Annotation annotation("annotation_settings.json");
  std::map<int, Team>teams;
  uint64_t now = 0;
  if (!manager.isLive()) {
    now = manager.getStart();
  }
uint64_t stop = now+6000000;

  while(manager.isGood()) {
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += NEXT_FRAME;
      }
    MessageManager::Status status = manager.getStatus(now);

   std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
   for (const auto & entry : images_by_source) {
     cv::Mat display_img = entry.second.getImg().clone();
     if (entry.second.isFullySpecified()) {
       const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
       for (const auto & robot_entry : status.robot_messages) {
	 uint64_t message_time = robot_entry.second.time_stamp();
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
	 if (robot_entry.first.robot_id() ==1){//pos for robot 1
	   if (now<stop-4000000 || now >stop+4000000 ){ //no position during 8s
	     teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);
	   }
	 }
	 if (robot_entry.first.robot_id() ==2){
	   if (now<stop+6000000 || now >stop+15000000 ){
	     
	     teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);
	   }
	 }
	 if (robot_entry.first.robot_id() ==4){

	   if (now<stop|| now >stop+8000000 ){ 
	     teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);
	     
	   }
	   
	 }
	
	 display_img =annotation.AddAnnotation(camera_information, teams[team_id].GetRobot(robot_entry.first.robot_id()) , display_img, now);
       }
     
       
     }
   
     cv::namedWindow(entry.first, cv::WINDOW_AUTOSIZE);
     cv::imshow(entry.first, display_img);
     cv::waitKey(5);
  
   }

 
  }
  return 0;
}

