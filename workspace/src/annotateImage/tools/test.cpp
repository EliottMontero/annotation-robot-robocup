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


using namespace hl_communication;
using namespace hl_monitoring;

using namespace annotateImage;

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
  uint64_t end_video = 0;

  if (!manager.isLive()) {
    std::set<std::string> image_provider =  manager.getImageProvidersNames();
    
    for (const auto & entry : image_provider)
      {
	
	if (now < manager.getImageProvider(entry).getStart())
	  now =  manager.getImageProvider(entry).getStart();
	if (end_video == 0 || end_video <  manager.getImageProvider(entry).getEnd())
	  end_video =  manager.getImageProvider(entry).getEnd();
      }
    
  }
uint64_t stop = now+6000000;

  while(now+NEXT_FRAME< end_video) {
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += NEXT_FRAME;
      }
    MessageManager::Status status = manager.getStatus(now);
    
    // std::cout << "GCMSG : " << status.gc_message.time_stamp() <<std::endl;
      for (int idx = 0; idx < status.gc_message.teams_size(); idx++)
      {

	const GCTeamMsg& team_msg = status.gc_message.teams(idx);
	if (team_msg.has_team_number() && team_msg.has_score())
	  {
	    uint32_t team_number = team_msg.team_number();
	    if (teams.find(team_number)==teams.end()){
	      Team t1;
	      teams[team_number]=t1;	      
	    }
	    teams[team_number].setScore(team_msg.score());
	  }
      }
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

      }

    
    std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
    for (const auto & entry : images_by_source) {
      cv::Mat display_img = entry.second.getImg().clone();
     if (entry.second.isFullySpecified())
       {
	    const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	    if (annotation.annotation_choice["field"])
	      annotation.field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
	    if (annotation.annotation_choice["score"])
	      annotation.annoteScore(teams, display_img);
	    
	    for (const auto & robot_entry : status.robot_messages) 
	      {
		display_img =annotation.AddAnnotation(camera_information, teams[robot_entry.first.team_id()].getRobot(robot_entry.first.robot_id()) , display_img, now);
	      }
	    
	  }
   
     cv::namedWindow(entry.first, cv::WINDOW_AUTOSIZE);
     cv::imshow(entry.first, display_img);
     cv::waitKey(5);
  
   }

 
  }
  return 0;
}

