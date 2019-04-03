#pragma once

#include <hl_monitoring/camera.pb.h>
#include <google/protobuf/message.h>

#include <opencv2/core.hpp>

namespace annotateImage

{
class Direction {
  public:
    float mean;
    float std_dev;
    float uncertainty;
    uint64_t time_stamp;

  Direction();
  ~Direction();

  void SetMean(float m, uint64_t t_s);
  };

 
}
