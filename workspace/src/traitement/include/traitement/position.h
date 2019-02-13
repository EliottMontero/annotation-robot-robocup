#pragma once


#include <hl_monitoring/camera.pb.h>

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
    void getPosition(float xp, float yp );
    void annotePosition(cv::Mat* display_img, const CameraMetaInformation & camera_information, cv::Scalar & color);
  };
}