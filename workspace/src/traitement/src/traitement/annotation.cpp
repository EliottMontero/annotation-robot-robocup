#include "traitement/annotation.h"


#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/position.h>


#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string.h>

using namespace hl_communication;
using namespace hl_monitoring;


namespace traitement{
	Annotation::Annotation(){
		std::string config = "replay.json";
		std::string field_name= "eirlab.json";
	}

	Annotation::~Annotation(){
	}

	/*void Annotation::displayAnnotation(){
		cv::imshow("annoted_video", display_img);
	} 
*/
	void Annotation :: launchAnnotation(std::string config_arg, std::string field_arg, bool affichage){
	MonitoringManager manager;

	manager.loadConfig(config_arg);

	Field field;
	field.loadFile(field_arg);
	uint64_t now = 0;
  	uint64_t dt = 30 * 1000;//[microseconds]
	 if (!manager.isLive()) {
	    now = manager.getStart();
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
    for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
      const GCTeamMsg & team_msg = status.gc_message.teams(idx);
      if (team_msg.has_team_number() && team_msg.has_team_color()) {
        uint32_t team_number = team_msg.team_number();
        uint32_t team_color = team_msg.team_color();
        uint32_t team_score = team_msg.score();
        colors_by_team[team_number] = team_colors[team_color];
      }
    }

    std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
    for (const auto & entry : images_by_source) {
      cv ::Mat display_img = entry.second.getImg().clone();
      if (entry.second.isFullySpecified()) {
        const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
        field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
        // Basic drawing of robot estimated position
        for (const auto & robot_entry : status.robot_messages) {
          uint32_t team_id = robot_entry.first.team_id();
          if (colors_by_team.count(team_id) == 0) {
          } else {
            const cv::Scalar & color = colors_by_team[team_id];
            if (robot_entry.second.has_perception()) {
              const Perception & perception = robot_entry.second.perception();
              for (int pos_idx = 0; pos_idx < perception.self_in_field_size(); pos_idx++) {
                /* position du robot */
                const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
                const PositionDistribution & position = weighted_pose.pose().position();

                Position pos;

                pos.setPosition(position.x(), position.y());
                cv :: Point3f pos_in_field(pos.x, pos.y, 0.0);
                cv :: Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
                 int circle_size = 10;
        		 cv::circle(display_img,pos_in_img, circle_size, color, cv::FILLED);
              }
            }
          }
        }
      }
	cv::imshow(entry.first, display_img);
    }
}		

   
	  }

}
	/* on crée un monitoring, on load des messages, on appelle les classes pour afficher des annotations, ex = position, fleche etc */
