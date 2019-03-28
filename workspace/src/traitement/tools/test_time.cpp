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
#include <iostream>
#include <chrono>
#include <ctime>    
#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>

#include <sstream>


using namespace hl_communication;
using namespace hl_monitoring;

using namespace traitement;

int main(int argc, char ** argv) {
  Json::Reader reader;
  Json::Value root;


  FILE* csv = NULL;

  csv = fopen("results.csv", "w");

  if (csv == NULL){
    fprintf(stderr, "\nErreur: Impossible d'écrire dans le fichier %s\n","out.csv");
    exit(EXIT_FAILURE);
  }
  fprintf(csv, "Time getCalibratedImage; Time cloneCalibratedImage; Time addFieldLines;Time addAnnotations; total elapsed_time; Time for next image\n");
  
  std::ifstream match_settings("match_settings.json");
  if (!match_settings.good())
    throw std::runtime_error("failde to open file match_settings.json");
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
  uint64_t begin_time = now;

  uint64_t time_stamp = 0; //used for calculs
  uint64_t old_time_stamp = 0; //stock the time_stamp of the previous message

  
  auto begin = std::chrono::system_clock::now(); //initialized at the begining of the programm, used to regulate the time
  auto loop = std::chrono::system_clock::now(); //initialized at the begining of the loop and used at the end
  auto start = std::chrono::system_clock::now();//used for calculs
  auto end = std::chrono::system_clock::now();//always have the latest time
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
    start = std::chrono::system_clock::now();
    loop = std::chrono::system_clock::now();
  
    
    MessageManager::Status status = manager.getStatus(now);

    std::vector<cv::Scalar> team_colors = {cv::Scalar(255,0,255), cv::Scalar(255,255,0)};
    std::map<uint32_t,cv::Scalar> colors_by_team;

    std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
    
    end = std::chrono::system_clock::now();
    elapsed_useconds =end-start;
    time_stamp = elapsed_useconds.count();
    fprintf(csv,"%" PRIu64 ";", time_stamp);
    start = std::chrono::system_clock::now();
    
    for (const auto & entry : images_by_source) {
      elapsed_useconds =end-begin;
      if ((now-begin_time-elapsed_useconds.count())>=-3000)
	{
	  cv::Mat display_img = entry.second.getImg().clone();

	
	  end = std::chrono::system_clock::now();
	  elapsed_useconds =end-start;
	  time_stamp = elapsed_useconds.count();
	  fprintf(csv,"%" PRIu64 ";", time_stamp);
	  start = std::chrono::system_clock::now();
   
	  if ( entry.second.isFullySpecified()) {
	    const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	    field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);

	    end = std::chrono::system_clock::now();
	    elapsed_useconds =end-start;
	    time_stamp = elapsed_useconds.count();
	    fprintf(csv,"%" PRIu64 ";", time_stamp);
	    start = std::chrono::system_clock::now();
	  
	    for (const auto & robot_entry : status.robot_messages) {
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
	      if (robot_entry.second.time_stamp() != old_time_stamp)
		teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);

	      display_img =annotation.AddAnnotation(camera_information, teams[team_id].GetRobot(robot_entry.first.robot_id()) , display_img, now);

	    }

	    end = std::chrono::system_clock::now();
	    elapsed_useconds =end-start;
	    time_stamp = elapsed_useconds.count();
	    fprintf(csv,"%" PRIu64 ";", time_stamp);
	    start = std::chrono::system_clock::now();
	  
	  }
	
	

	  cv::namedWindow(entry.first, cv::WINDOW_AUTOSIZE);
	  cv::imshow(entry.first, display_img);


	  end = std::chrono::system_clock::now();
	  elapsed_useconds = end-loop;
	  time_stamp = elapsed_useconds.count();
	  old_time_stamp = status.gc_message.time_stamp();
	  fprintf(csv,"%" PRIu64 ";", time_stamp);
	  elapsed_useconds = end-begin;
	  
	  if (elapsed_useconds.count() >=(now-begin_time)-1000){
	    cv::waitKey(1);
	    fprintf(csv, "fast\n");  
	  }
	  else{
	    cv::waitKey(((now-begin_time)-elapsed_useconds.count())/1000);
	    fprintf(csv, "slow\n");
	  
	  }
	
	}
       
	
      else{
	elapsed_useconds = end-loop;
	time_stamp = elapsed_useconds.count();
	fprintf(csv, "0;0;0;%" PRIu64 ";skip image\n", time_stamp);
	  
      }
    }  
  }
  return 0;
}
