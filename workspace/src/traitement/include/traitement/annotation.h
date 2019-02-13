#pragma once

#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


namespace traitement{
	class Annotation{
	public:
		std::string config;
		std::string field_name;

	  Annotation();
	  ~Annotation();

	 void launchAnnotation();
};
}