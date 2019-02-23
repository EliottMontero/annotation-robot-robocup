#pragma once

#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <traitement/robotInformation.h>
#include <traitement/direction.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <google/protobuf/message.h>

using namespace hl_monitoring;
using namespace hl_communication;


namespace traitement{
  class Annotation{
  public:
		
    std::map<std::string, bool>annotation_choice;
    int sizecircle;
    int sizecircletrace;
    int ballsize;
    int sizearrow;
    int robottrace;
    int robotball;
    std::map<int ,cv::Scalar> color;


    Annotation(std::string);
    ~Annotation();


    cv::Mat annotePosition(Position pos,  CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display);
    cv::Mat annoteDirection(Position pos, Direction dir, CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display);
    cv::Mat  annoteTrace(CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display);
    cv::Mat  annoteBall(Position pos, Direction dir, CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display);
    cv::Mat AddAnnotation(Position pos, Direction dir,CameraMetaInformation camera_information,  RobotInformation rb,cv::Mat display);

  };
}
