#pragma once

#include <hl_monitoring/camera.pb.h>
#include <google/protobuf/message.h>

#include <opencv2/core.hpp>


namespace traitement
{
	/*Créer des positions pour la facilité de stockage */

class Position {
  public:
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
