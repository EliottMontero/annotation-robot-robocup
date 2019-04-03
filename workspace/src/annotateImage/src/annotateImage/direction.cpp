#include "annotateImage/direction.h"

#include <hl_communication/utils.h>
#include <hl_monitoring/field.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/monitoring_manager.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace hl_communication;
using namespace hl_monitoring;

namespace annotateImage {

#define ARROW_SIZE 50

//ToDo pour amélioration !!!!
//Direction a besoin de Position donc peut-être ajouter un attribut Position
// à Direction afin de largement diminuer le nombre d'attribut
// OU
//Recalculer Position directement mais c'est un peu idiot...
// OU
// Passer un attribut Position en argument de Draw aussi...
// OU
// autre chose

  Direction::Direction(){
    mean = 0;
    std_dev;
    time_stamp = 0;
  }

  Direction::~Direction(){
    
  }

  void Direction::SetMean(float m, uint64_t t_s){
  mean = m;
  time_stamp = t_s;
}




}
