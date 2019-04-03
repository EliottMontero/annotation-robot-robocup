#include "annotateImage/direction.h"

#define ARROW_SIZE 50

namespace annotateImage {

  Direction::Direction(){
    mean = 0;
    std_dev;
    time_stamp = 0;
  }

  Direction::~Direction(){
  }

  void Direction::setMean(float m, uint64_t t_s){
    mean = m;
    time_stamp = t_s;
  }

}
