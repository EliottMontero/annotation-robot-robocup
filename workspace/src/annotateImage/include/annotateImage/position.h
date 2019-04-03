#pragma once

#include <google/protobuf/message.h>

namespace annotateImage {
  class Position {

  public :
    float x;
    float y;
    float uncertainty;
    uint64_t time_stamp;

    Position();
    ~Position();

    void setPosition(float xp, float yp,  uint64_t time_stamp);
    void setTimeStamp(uint64_t t_s);

  };
}
