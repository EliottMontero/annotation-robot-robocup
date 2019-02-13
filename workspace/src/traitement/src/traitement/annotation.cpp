#include "../../include/traitement/annotation.h"


#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>


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
	}

	Annotation::~Annotation(){
	}

	void Annotation :: lauchAnnotation(){
	MonitoringManager manager;

	manager.loadConfig(config);

	Field field;
	field.loadFile(field_name);
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
	              }
	          }
	      }
	  }
}
	/* on crée un monitoring, on load des messages, on appelle les classes pour afficher des annotations, ex = position, fleche etc */
