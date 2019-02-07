#include "hl_monitoring/calibrated_image.h"

#include "hl_monitoring/utils.h"

#include <stdexcept>

namespace hl_monitoring
{

CalibratedImage::CalibratedImage() {
}

CalibratedImage::CalibratedImage(const cv::Mat & img_,
                                 const Pose3D & pose,
                                 const IntrinsicParameters & camera_parameters)
  : img(img_)
{
  camera_meta.mutable_pose()->CopyFrom(pose);
  camera_meta.mutable_camera_parameters()->CopyFrom(camera_parameters);
}

CalibratedImage::CalibratedImage(const cv::Mat & img_,
                                 const CameraMetaInformation & camera_meta_)
  : img(img_), camera_meta(camera_meta_)
{
}

const cv::Mat & CalibratedImage::getImg() const {
  return img;
}

const CameraMetaInformation & CalibratedImage::getCameraInformation() const {
  return camera_meta;
}


bool CalibratedImage::hasCameraParameters() const {
  return camera_meta.has_camera_parameters();
}

bool CalibratedImage::hasPose() const {
  return camera_meta.has_pose();
}

void CalibratedImage::exportCameraParameters(cv::Mat * camera_matrix,
                                             cv::Mat * distortion_coefficients,
                                             cv::Size * size) const {
  if (hasCameraParameters()) {
    intrinsicToCV(camera_meta.camera_parameters(), camera_matrix, distortion_coefficients, size);
  }
}
void CalibratedImage::exportPose(cv::Mat * rvec, cv::Mat * tvec) const {
  if (hasPose()) {
    pose3DToCV(camera_meta.pose(), rvec, tvec);
  }
}

bool CalibratedImage::isFullySpecified() const {
  return hasPose() && hasCameraParameters();
}


}
