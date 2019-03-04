#include <traitement/annotation.h>


#include <hl_communication/utils.h>

#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/position.h>
#include <traitement/direction.h>
#include <traitement/team.h>



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

int main(int argc, char ** argv) {
  Json::Reader reader;
  Json::Value root;
  

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
    /*  for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
      std::cout << " team size : " << idx  << std::endl;  
      const GCTeamMsg & team_msg = status.gc_message.teams(idx);
      if (team_msg.has_team_number() && team_msg.has_team_color()) {
        uint32_t team_color = team_msg.team_color();
        uint32_t team_number = team_msg.team_number();
        colors_by_team[team_number] = team_colors[team_color];
      }
    }
    */


   std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
   for (const auto & entry : images_by_source) {
     cv::Mat display_img = entry.second.getImg().clone();
     if (entry.second.isFullySpecified()) {
       const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
       field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
       // Basic drawing of robot estimated position
       for (const auto & robot_entry : status.robot_messages) {
	 uint32_t team_id = robot_entry.first.team_id();
         if (robot_entry.second.has_perception()) {
	   const Perception & perception = robot_entry.second.perception();			
	   for (int pos_idx = 0; pos_idx < perception.self_in_field_size(); pos_idx++) {
	     
	     /* init des robots présent sur le jeu*/
	     if (teams.find(team_id)==teams.end()){
	       Team t1;
	       teams[team_id]=t1;
	     }
	     if (!teams[team_id].IsRobot(robot_entry.first.robot_id())){
	       teams[team_id].AddRobot(robot_entry.first.robot_id());
	       teams[team_id].setRobotTeam(robot_entry.first.robot_id(),team_id);
	       teams[team_id].setRobotNum(robot_entry.first.robot_id());

	     }


	     /* position du robot */
	     const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
	     const PositionDistribution & position = weighted_pose.pose().position();
	     Position pos;
	     pos.setPosition(position.x(),position.y());
	     teams[team_id].RobotUpdate(robot_entry.first.robot_id(),pos);

	     const AngleDistribution & dir = weighted_pose.pose().dir();
	     Direction direction;
	     direction.SetMean (dir.mean());
	     teams[team_id].setRobotDirRobot(robot_entry.first.robot_id(),direction);

	     const PositionDistribution & ball = perception.ball_in_self();
	     Position pos_ball;
	     pos_ball.setPosition(ball.x(), ball.y());
	     teams[team_id].setRobotPosBall(robot_entry.first.robot_id(), pos_ball);
		
	     display_img =annotation.AddAnnotation(camera_information, teams[team_id].GetRobot(robot_entry.first.robot_id()) , display_img);
	   }
            
	 }
       }
     }
   
     cv::namedWindow(entry.first, cv::WINDOW_AUTOSIZE);
     cv::imshow(entry.first, display_img);
     cv::waitKey(10);
  
   }

 
  }
  return 0;
}
