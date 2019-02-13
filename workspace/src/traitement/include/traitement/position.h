#pragma once

#include <hl_monitoring/camera.pb.h>
#include <google/protobuf/message.h>

#include <opencv2/core.hpp>


namespace traitement
{
class Position {
  public:
    float x;
    float y;
    float uncertainty;

    Position();
    ~Position();
    void setPosition(float xp, float yp );
    void annotePosition(cv::Mat* display_img, const ::google::protobuf::Message& from,cv::Scalar & color);
  };
}