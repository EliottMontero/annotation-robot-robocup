#include "../../include/traitement/position.h"

#include <hl_communication/utils.h>
#include <hl_monitoring/field.h>
#include <hl_communication/message_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/monitoring_manager.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace hl_communication;
using namespace hl_monitoring;


namespace traitement
/*Afficher la position du robot depuis les logs*/
{
	Position::Position(){
		float x = 0;
		float y = 0;
		float uncertainty = 0;
	}
	Position::~Position(){
	}

	void Position::getPosition(float xp, float yp ){
		x = xp;
		y = yp;
	}

	void Position::annotePosition(cv::Mat* display_img, const CameraMetaInformation & camera_information, cv::Scalar & color){
		 cv::Point3f pos_in_field(x, y, 0.0);
         cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
         int circle_size = 10;
         cv::circle(&display_img, pos_in_img, circle_size, color, cv::FILLED);
	}

}/*on fait des position.get à partir des logs pui position.draw pour add à l'image*/