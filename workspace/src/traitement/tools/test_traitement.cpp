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
  checkMember(root["match_setting"], "speed_optimized");
  bool speed_optimized = root["match_setting"]["speed_optimized"].asBool();
  
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

  //Use to optimize the speed of the video
  uint64_t begin_time = now;  
  auto start = std::chrono::system_clock::now(); 
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double, std::micro> elapsed_useconds = end-start;
  
  if (!manager.isLive()) {
    now = manager.getStart();
    begin_time = now;
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


    std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
    for (const auto & entry : images_by_source) {
      if ((now-begin_time-elapsed_useconds.count())>-3000)
	{
	  cv::Mat display_img = entry.second.getImg().clone();
	  if (entry.second.isFullySpecified()) {
	    const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	    //field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
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
      

	  if(!speed_optimized)
	    cv::waitKey(1);
	  else
	    {
	      end = std::chrono::system_clock::now();
	      elapsed_useconds = end-start;
	      if (elapsed_useconds.count() >=(now-begin_time)-1000){
		cv::waitKey(1);
		//	std::cout << "fast" << std::endl;

	      }
	      else{
		cv::waitKey(((now-begin_time)-elapsed_useconds.count())/1000);
		//		std::cout << "slow" << std::endl;

	      }
	    }
	 
	  
	}
       else
	    {
	      //	      std::cout << "skip image" << std::endl;

	      elapsed_useconds = end-start;
	    }
    }  
  }
  return 0;
}
