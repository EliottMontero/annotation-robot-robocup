#pragma once

#include <hl_monitoring/camera.pb.h>
#include <google/protobuf/message.h>

#include <opencv2/core.hpp>

namespace traitement

{
class Direction {
  public:
    float mean;
    float std_dev;
    float uncertainty;

  Direction();
  ~Direction();

  void SetMean(float m);
  };

}
