#pragma once

#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <google/protobuf/message.h>

namespace traitement{
	class Annotation{
	public:
		std::string config;
		std::string field_name;
		cv::Mat display_img ;


		//Etat du Jeu
		uint64_t now;
		uint64_t time_start;


		//Gestion des TeamMsg
 		std::map<uint32_t, uint32_t> score_by_team;
		std::map<uint32_t,cv::Scalar> colors_by_team;
		std::vector<cv::Scalar> team_colors;
		uint32_t TeamNumber[2];


	  Annotation();
	  ~Annotation();
		std::string getScore();
		std::string getTime();
	 	void displayAnnotation();
	 	void launchAnnotation(int argc, char ** argv, bool affichage);
};
}
