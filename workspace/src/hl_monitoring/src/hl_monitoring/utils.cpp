#include <hl_monitoring/utils.h>

#include <hl_communication/utils.h>

#include <opencv2/calib3d.hpp>

namespace hl_monitoring
{

void intrinsicToCV(const IntrinsicParameters & camera_parameters,
                   cv::Mat * camera_matrix,
                   cv::Mat * distortion_coefficients,
                   cv::Size * img_size) {
  *camera_matrix = cv::Mat(3,3,CV_64F);
  camera_matrix->at<double>(0,0) = camera_parameters.focal_x();
  camera_matrix->at<double>(1,1) = camera_parameters.focal_y();
  camera_matrix->at<double>(0,2) = camera_parameters.center_x();
  camera_matrix->at<double>(1,2) = camera_parameters.center_y();
  camera_matrix->at<double>(2,2) = 1.0;
  img_size->width = camera_parameters.img_width();
  img_size->height = camera_parameters.img_height();
  *distortion_coefficients = cv::Mat(1,camera_parameters.distortion_size(), CV_64F);
  for (int i = 0; i < camera_parameters.distortion_size(); i++) {
    distortion_coefficients->at<double>(0,i) = camera_parameters.distortion(i);
  }
  
}

void cvToIntrinsic(const cv::Mat & camera_matrix,
                   const cv::Mat & distortion_coefficients,
                   const cv::Size & img_size,
                   IntrinsicParameters * camera_parameters) {
  camera_parameters->Clear();
  camera_parameters->set_focal_x(camera_matrix.at<double>(0,0));
  camera_parameters->set_focal_y(camera_matrix.at<double>(1,1));
  camera_parameters->set_center_x(camera_matrix.at<double>(0,2));
  camera_parameters->set_center_y(camera_matrix.at<double>(1,2));
  camera_parameters->set_img_width(img_size.width);
  camera_parameters->set_img_height(img_size.height);
  for (int i = 0; i < distortion_coefficients.cols; i++) {
    camera_parameters->add_distortion(distortion_coefficients.at<double>(0,i));
  }
}

void pose3DToCV(const Pose3D & pose,
                cv::Mat * rvec,
                cv::Mat * tvec) {
  if (pose.rotation_size() != 3) {
    throw std::runtime_error("Only Rodrigues rotation vector is supported currently");
  }
  if (pose.translation_size() != 3) {
    throw std::runtime_error("Size of translation in Pose3D is not valid (only 3 is accepted)");
  }
  *rvec = cv::Mat(3,1,CV_64F);
  *tvec = cv::Mat(3,1,CV_64F);
  for (int i=0; i<3; i++) {
    rvec->at<double>(i,0) = pose.rotation(i);
    tvec->at<double>(i,0) = pose.translation(i);
  }
}

void cvToPose3D(const cv::Mat & rvec,
                const cv::Mat & tvec,
                Pose3D * pose) {
  pose->Clear();
  for (int i=0; i<3; i++) {
    pose->add_rotation(rvec.at<double>(i,0));
    pose->add_translation(tvec.at<double>(i,0));
  }
}

cv::Point2f fieldToImg(const cv::Point3f & pos_in_field,
                       const CameraMetaInformation & camera_information) {
  if (!camera_information.has_camera_parameters() || !camera_information.has_pose()) {
    throw std::runtime_error(HL_DEBUG + " camera_information is not fully specified");
  }
  cv::Mat camera_matrix, distortion_coeffs, rvec, tvec;
  cv::Size size;
  intrinsicToCV(camera_information.camera_parameters(),
                &camera_matrix, &distortion_coeffs, &size);
  pose3DToCV(camera_information.pose(), &rvec, &tvec);
  std::vector<cv::Point3f> object_points = {pos_in_field};
  std::vector<cv::Point2f> img_points;
  cv::projectPoints(object_points, rvec, tvec, camera_matrix, distortion_coeffs, img_points);
  return img_points[0];
}

void checkMember(const Json::Value & v, const std::string & key)
{
  if (!v.isObject() || !v.isMember(key)){
    throw std::runtime_error(HL_DEBUG + "Could not find member '" + key + "'");
  }
}

template <>
void readVal<bool>(const Json::Value & v, const std::string & key, bool * dst)
{
  checkMember(v,key);
  if (!v[key].isBool()) {
    throw std::runtime_error(HL_DEBUG + "Expecting a bool for key '" + key + "'");
  }
  *dst = v[key].asBool();
}

template <>
void readVal<int>(const Json::Value & v, const std::string & key, int * dst)
{
  checkMember(v,key);
  if (!v[key].isInt()) {
    throw std::runtime_error(HL_DEBUG + "Expecting an int for key '" + key + "'");
  }
  *dst = v[key].asInt();
}

template <>
void readVal<double>(const Json::Value & v, const std::string & key, double * dst)
{
  checkMember(v,key);
  if (!v[key].isDouble()) {
    throw std::runtime_error(HL_DEBUG + "Expecting a double for key '" + key + "'");
  }
  *dst = v[key].asDouble();
}

template <>
void readVal<std::string>(const Json::Value & v, const std::string & key, std::string * dst)
{
  checkMember(v,key);
  if (!v[key].isString()) {
    throw std::runtime_error(HL_DEBUG + "Expecting a string for key '" + key + "'");
  }
  *dst = v[key].asString();
}


}
