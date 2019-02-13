#pragma once

#include <hl_monitoring/camera.pb.h>
#include <google/protobuf/message.h>

#include <opencv2/core.hpp>

namespace traitement

{
class Direction {
  public:
    double mean;
    double std_dev;
    double uncertainty;

  Direction();
  ~Direction();
  void draw(cv::Mat * display_img, const ::google::protobuf::Message& weighted_pose,
      cv::Point3f pos_in_field, cv::Point2f pos_in_img,const ::google::protobuf::Message& camera_information,
      float positionX,float positionY, cv::Scalar & color);

  };
}
