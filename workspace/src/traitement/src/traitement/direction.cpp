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
  double mean;
  double std_dev;
  double uncertainty;
}

Direction::~Direction(){

}

void Direction::draw(cv::Mat * display_img, const ::google::protobuf::Message& from_weighted_pose,
    cv::Point3f pos_in_field, cv::Point2f pos_in_img,const ::google::protobuf::Message& from_camera_information,
    float positionX,float positionY,  cv::Scalar & color){
  /* direction du robot */
  CameraMetaInformation camera_information;
  camera_information.CopyFrom(from_camera_information);
  WeightedPose weighted_pose;
  weighted_pose.CopyFrom(from_weighted_pose);

  const AngleDistribution & dir = weighted_pose.pose().dir();
  float angle = this->mean;
  cv :: Point3f dir_in_field(positionX+cos(angle), positionY+sin(angle), 0.0);
  cv :: Point2f dir_in_img = fieldToImg(dir_in_field, camera_information);
  /* reduction taille des flèches à une longueur de 50 pour que la taille des flèches soit homogène*/
  float hypo = sqrt((dir_in_img.x - pos_in_img.x)*(dir_in_img.x - pos_in_img.x) +(dir_in_img.y- pos_in_img.y)*(dir_in_img.y- pos_in_img.y));
  cv :: Point2f fleche;
  fleche.x =  pos_in_img.x + (ARROW_SIZE*(dir_in_img.x - pos_in_img.x)/hypo);
  fleche.y= pos_in_img.y + (ARROW_SIZE*(dir_in_img.y- pos_in_img.y)/hypo);
  /*Affichage couleur pour les angles de degrès bizarre*/
  if (angle > 2*CV_PI)
    cv :: arrowedLine(*display_img, pos_in_img, fleche, cv::Scalar(0,0,0), 2, 0, 0.1);
  else
    cv :: arrowedLine(*display_img, pos_in_img, fleche, color, 2, 0, 0.1);
}



}
