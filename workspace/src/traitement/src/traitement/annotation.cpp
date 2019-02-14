#include "traitement/annotation.h"


#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/position.h>
#include <traitement/direction.h>


#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>
#include <string.h>

using namespace hl_communication;
using namespace hl_monitoring;


namespace traitement{
	Annotation::Annotation(){
		std::string config = "replay.json";
		std::string field_name= "eirlab.json";
		cv::Mat display_img ;
	}

	Annotation::~Annotation(){
	}

	void Annotation::displayAnnotation(){
		cv::imshow("annoted_video", display_img);
	}

	void Annotation :: launchAnnotation(int argc, char ** argv, bool affichage){
		TCLAP::CmdLine cmd("Acquire and display one or multiple streams along with meta-information",
	                     ' ', "0.9");

	  TCLAP::ValueArg<std::string> config_arg("c", "config", "The path to the json configuration file",
	                                          true, "config.json", "string");
	  TCLAP::ValueArg<std::string> field_arg("f", "field", "The path to the json description of the file",
	                                          true, "field.json", "string");

	 TCLAP::MultiArg<int> annot_arg("a", "annotation", "annotation to print",
	                                          true, "vector of annot", cmd);
	  TCLAP::SwitchArg verbose_arg("v", "verbose", "If enabled display all messages received",
	                               cmd, false);
	  cmd.add(config_arg);
	  cmd.add(field_arg);

	  try {
	    cmd.parse(argc, argv);

	  } catch (const TCLAP::ArgException & e) {
	    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	  }
	  std::vector<int> annot = annot_arg.getValue();

	  MonitoringManager manager;

	  manager.loadConfig(config_arg.getValue());

	  Field field;
	  field.loadFile(field_arg.getValue());

	  // While exit was not explicitly required, run
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
	        std::cout << " team " << team_msg.team_number()
	                  << " score : " << team_score << std::endl;
	        /*for (int idxr = 0; idxr < team_msg.robots_size(); idxr++) {
	           const GCRobotMsg & robots_msg =team_msg.robots(idxr);
	           if (robots_msg.has_penalty()) {
	            uint32_t robot_penalty = robots_msg.penalty();
	            uint32_t robot_secs_till_unpenalised = robots_msg.secs_till_unpenalised();
	             std::cout << "-> Message from robot " << idx << " of team  " << team_number
	                  << " -> penalty " << robot_penalty  << " secs_till_unpenalised : " << robot_secs_till_unpenalised   << std::endl;
	           }
	         }*/
	      }
	    }

	    if (verbose_arg.getValue()) {
	      std::cout << "Time: " <<  now << std::endl;
	      for (const auto & robot_entry : status.robot_messages) {
	        std::cout << "-> Message from robot " << robot_entry.first.robot_id()
	                  << " from team " << robot_entry.first.team_id() << std::endl;
	      }
	    }

	    std::map<std::string, CalibratedImage> images_by_source =
	      manager.getCalibratedImages(now);
	    for (const auto & entry : images_by_source) {
	     	display_img = entry.second.getImg().clone();
	      if (entry.second.isFullySpecified()) {
	        const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	        field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
	        // Basic drawing of robot estimated position
	        for (const auto & robot_entry : status.robot_messages) {
	          uint32_t team_id = robot_entry.first.team_id();
	          if (colors_by_team.count(team_id) == 0) {
	            std::cerr << "Unknown color for team " << team_id << std::endl;
	          } else {
	            const cv::Scalar & color = colors_by_team[team_id];
	            if (robot_entry.second.has_perception()) {
	              const Perception & perception = robot_entry.second.perception();
	              for (int pos_idx = 0; pos_idx < perception.self_in_field_size(); pos_idx++) {
	                /* position du robot */
	                const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
	                const PositionDistribution & position = weighted_pose.pose().position();
	               /* cv::Point3f pos_in_field(position.x(), position.y(), 0.0);
	                cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
	                int circle_size = 10;
	                cv::circle(display_img, pos_in_img, circle_size, color, cv::FILLED);*/
	                 Position pos;

              		  pos.setPosition(position.x(), position.y());
               		 cv :: Point3f pos_in_field(pos.x, pos.y, 0.0);
	               	 cv :: Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
	               	 if (annot[0] == 1){
	                 int circle_size = 10;
                         cv::circle(display_img,pos_in_img, circle_size, color,cv::FILLED);
                     }

                     if (annot[1] == 1){

	                /* direction du robot */
	                const AngleDistribution & dir = weighted_pose.pose().dir();
	                Direction direct;
	                direct.SetMean(dir.mean());
	                cv :: Point3f pos_in_fielddir(pos.x+cos(direct.mean),pos.x+sin(direct.mean), 0.0);
	                cv :: Point2f pos_in_imgdir = fieldToImg(pos_in_fielddir, camera_information);
	                /* reduction taille des flèches à une longueur de 50 pour que la taille des flèches soit homogène*/
 					float hypo = sqrt((pos_in_imgdir.x - pos_in_img.x)*(pos_in_imgdir.x - pos_in_img.x) +(pos_in_imgdir.y- pos_in_img.y)*(pos_in_imgdir.y- pos_in_img.y));
               		 cv :: Point2f fleche;
                	fleche.x =  pos_in_img.x + (50*(pos_in_imgdir.x - pos_in_img.x)/hypo);
                	fleche.y= pos_in_img.y + (50*(pos_in_imgdir.y- pos_in_img.y)/hypo);	                /*Affichage couleur pour les angles de degrès bizarre*/
	                if (direct.mean > 2*CV_PI)
	                  cv :: arrowedLine(display_img, pos_in_img, fleche, cv::Scalar(0,0,0), 2, 0, 0.1);
	                else
	                  cv :: arrowedLine(display_img, pos_in_img, fleche, color, 2, 0, 0.1);



	                /* Affichage du message pour vérifications */
	                std::cout << "-> Robot num " << robot_entry.first.robot_id()
	                << " from team " << robot_entry.first.team_id()
	                << "and pos x : " << position.x() << " y : " <<position.y() << " dir  == > " << dir.mean() <<std::endl;}
	                /*std::cout << "-> Robot pos x : " << pos_in_img.x << " y : " << pos_in_img.y
	                 << " pos fleche : " << pos_in_imgdir.x << " y : " << pos_in_imgdir.y
	                 << " pos fleche new : " << fleche.x << " y : " << fleche.y << " hypo ==> " << hypo <<std::endl;*/
	              }
	            }
	          }
	        }
	      }
	      displayAnnotation();	
	    }
	    char key = cv::waitKey(10);
	    if (key == 'q' || key == 'Q') break;
	  }
	}
}
	/* on crée un monitoring, on load des messages, on appelle les classes pour afficher des annotations, ex = position, fleche etc */
