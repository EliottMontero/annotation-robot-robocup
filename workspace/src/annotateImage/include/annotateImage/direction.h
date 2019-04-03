#pragma once

#include <google/protobuf/message.h>

namespace annotateImage {
  class Direction {

  public :
    float mean;
    float std_dev;
    float uncertainty;
    uint64_t time_stamp;

    Direction();
    ~Direction();

    void setMean(float m, uint64_t t_s);

  };
}
