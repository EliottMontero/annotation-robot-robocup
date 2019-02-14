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

		//Key pour retrouver plus facilement les Map avec info des Team
 		uint32_t Team1;
 		uint32_t Team2;

 		std::map<uint32_t, uint32_t> score_by_team;

		std::map<uint32_t,cv::Scalar> colors_by_team;
		std::vector<cv::Scalar> team_colors;


	  Annotation();
	  ~Annotation();
		std::string getScore();
	 void displayAnnotation();
	 void launchAnnotation(int argc, char ** argv, bool affichage);
};
}
