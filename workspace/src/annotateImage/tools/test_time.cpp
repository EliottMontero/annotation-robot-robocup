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
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>

#include <sstream>

#define SECONDS_TO_MS 1000
#define NEXT_FRAME 30000 //30fps in microseconds
#define NB_FRAME_READ 1000

using namespace hl_communication;
using namespace hl_monitoring;

using namespace annotateImage;

int main() {
  Json::Reader reader;
  Json::Value root;


  FILE* csv = NULL;

  csv = fopen("results.csv", "w");

  if (csv == NULL){
    fprintf(stderr, "\nErreur: Impossible d'écrire dans le fichier %s\n","result.csv");
    exit(EXIT_FAILURE);
  }

  fprintf(csv, "Time to update messages;Time getCalibratedImage;Time cloneCalibratedImage;Time Annotations Field&Score;Time addAnnotations;Total elapsed_time;Action for next image\n");

  std::ifstream match_settings("match_settings.json");
  if (!match_settings.good())
    throw std::runtime_error("failde to open file match_settings.json");
  match_settings >> root;

  checkMember(root["match_setting"], "config");
  checkMember(root["match_setting"], "speed_optimized");
  bool speed_optimized = root["match_setting"]["speed_optimized"].asBool();


  std::string conf = root["match_setting"]["config"].asString();
  std::cout << conf << std::endl;


  MonitoringManager manager;
  manager.loadConfig(conf);


  Annotation annotation("annotation_settings.json");
  std::map<int, Team>teams;
  uint64_t now = 0;
  uint64_t begin_time = 0;
  uint64_t end_video = 0;

  uint64_t time_stamp = 0; //used for calculs


  auto begin = std::chrono::system_clock::now(); //initialized at the begining of the programm, used to regulate the time
  auto loop = std::chrono::system_clock::now(); //initialized at the begining of the loop and used at the end
  auto start = std::chrono::system_clock::now();//used for calculs
  auto end = std::chrono::system_clock::now();//always have the latest time
  std::chrono::duration<double, std::micro> elapsed_useconds = end-start;

  if (!manager.isLive()) {
    std::set<std::string> image_provider =  manager.getImageProvidersNames();

    for (const auto & entry : image_provider)
      {

	if (now < manager.getImageProvider(entry).getStart())
	  now =  manager.getImageProvider(entry).getStart();
	if (end_video == 0 || end_video <  manager.getImageProvider(entry).getEnd())
	  end_video =  manager.getImageProvider(entry).getEnd();
      }

    begin_time = now;
  }


  while((now-begin_time) / NEXT_FRAME< NB_FRAME_READ) {
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += NEXT_FRAME;
    }
    start = std::chrono::system_clock::now();
    loop = std::chrono::system_clock::now();


    MessageManager::Status status = manager.getStatus(now);

     for (int idx = 0; idx < status.gc_message.teams_size(); idx++)
      {
	const GCTeamMsg& team_msg = status.gc_message.teams(idx);
	if (team_msg.has_team_number() && team_msg.has_score())
	  {
	    uint32_t team_number = team_msg.team_number();
	    if (teams.find(team_number)!=teams.end())
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
	if (!teams[team_id].isRobot(robot_entry.first.robot_id())){
	  teams[team_id].addRobot(robot_entry.first.robot_id());
	  teams[team_id].setRobotTeam(robot_entry.first.robot_id(),team_id);
	  teams[team_id].setRobotNum(robot_entry.first.robot_id());
	}

	if (robot_entry.second.time_stamp() !=  teams[team_id].getRobot(robot_entry.first.robot_id()).getMessageRobot().time_stamp())
	  teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);
      }


    end = std::chrono::system_clock::now();
    elapsed_useconds =end-start;
    time_stamp = elapsed_useconds.count();
    fprintf(csv,"%" PRIu64 ";", time_stamp);
    start = std::chrono::system_clock::now();

    std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);

    end = std::chrono::system_clock::now();
    elapsed_useconds =end-start;
    time_stamp = elapsed_useconds.count();
    fprintf(csv,"%" PRIu64 ";", time_stamp);
    start = std::chrono::system_clock::now();

    for (const auto & entry : images_by_source) {
      end = std::chrono::system_clock::now();
      elapsed_useconds= end-begin;

       if (elapsed_useconds.count()<=(now-begin_time) || !speed_optimized)
	{
	  cv::Mat display_img = entry.second.getImg().clone();

	  if (entry.first != images_by_source.begin()->first || images_by_source.size()==1)
	    {

	  end = std::chrono::system_clock::now();
	  elapsed_useconds =end-start;
	  time_stamp = elapsed_useconds.count();
	  fprintf(csv,"%" PRIu64 ";", time_stamp);
	  start = std::chrono::system_clock::now();
	    }


	  if ( entry.second.isFullySpecified()) {
	    const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	    if (annotation.annotation_choice["field"])
	      annotation.field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
	    if (annotation.annotation_choice["score"])
	      annotation.annoteScore(teams, display_img);

	     if (entry.first != images_by_source.begin()->first  || images_by_source.size()==1)
	    {

	    end = std::chrono::system_clock::now();
	    elapsed_useconds =end-start;
	    time_stamp = elapsed_useconds.count();
	    fprintf(csv,"%" PRIu64 ";", time_stamp);
	    start = std::chrono::system_clock::now();
	    }


	    for (const auto & robot_entry : status.robot_messages) {
	      display_img =annotation.addAnnotation(camera_information, teams[robot_entry.first.team_id()].getRobot(robot_entry.first.robot_id()) , display_img, now);

	    }
	     if (entry.first != images_by_source.begin()->first || images_by_source.size()==1)
	    {


	    end = std::chrono::system_clock::now();
	    elapsed_useconds =end-start;
	    time_stamp = elapsed_useconds.count();
	    fprintf(csv,"%" PRIu64 ";", time_stamp);
	    start = std::chrono::system_clock::now();

	    }

	  }



	  	  cv::namedWindow(entry.first, cv::WINDOW_AUTOSIZE);
		  cv::imshow(entry.first, display_img);

	   if (entry.first != images_by_source.begin()->first || images_by_source.size()==1)
	    {

	  end = std::chrono::system_clock::now();
	  elapsed_useconds = end-loop;
	  time_stamp = elapsed_useconds.count();
	  fprintf(csv,"%" PRIu64 ";", time_stamp);

	    }


	  if(!speed_optimized)
	    {
	      fprintf(csv, "non_optimized \n");
	      cv::waitKey(1);
	    }

	  else
	    {  if (entry.first != images_by_source.begin()->first || images_by_source.size()==1)
	    {

	      end = std::chrono::system_clock::now();
	      elapsed_useconds = end-begin;

	      if (elapsed_useconds.count() >=(now-begin_time)-SECONDS_TO_MS){
		cv::waitKey(1);
		fprintf(csv, "fast \n");
	      }
	      else{
		cv::waitKey(((now-begin_time)-elapsed_useconds.count())/SECONDS_TO_MS);
		fprintf(csv, "slow \n");
	      }
	    }
	    }



	}
       else
	 { if (entry.first != images_by_source.begin()->first || images_by_source.size()==1)
	    {

	   fprintf(csv, "0;0;0;0;skip image \n");
	    }

	 }


    }
  }
  return 0;
}
