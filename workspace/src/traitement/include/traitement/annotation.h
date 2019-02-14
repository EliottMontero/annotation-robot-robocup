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
		 //cv::Mat display_img ;
	  Annotation();
	  ~Annotation();
	 //void displayAnnotation();
	 void launchAnnotation(int argc, char ** argv);
};
}
