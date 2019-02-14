#include "traitement/direction.h"

#include <hl_communication/utils.h>
#include <hl_monitoring/field.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/monitoring_manager.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace hl_communication;
using namespace hl_monitoring;

namespace traitement {

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
  float mean;
  float std_dev;
  float uncertainty;
}

Direction::~Direction(){

}

void Direction::SetMean(float m){
  mean = m;
}




}
