#pragma once

#include "hl_monitoring/camera.pb.h"

#include <opencv2/core.hpp>

namespace hl_monitoring
{

/**
 * Represents an image along with its intrinsic and extrinsic parameters
 */
class CalibratedImage {
public:
  CalibratedImage();
  CalibratedImage(const cv::Mat & img,
                  const Pose3D & pose,
                  const IntrinsicParameters & camera_parameters);
  CalibratedImage(const cv::Mat & img,
                  const CameraMetaInformation & camera_meta);

  const cv::Mat & getImg() const;

  const CameraMetaInformation & getCameraInformation() const;

  bool hasCameraParameters() const;
  bool hasPose() const;

  void exportCameraParameters(cv::Mat * camera_matrix,
                              cv::Mat * distortion_coefficients,
                              cv::Size * size) const;
  void exportPose(cv::Mat * rvec, cv::Mat * tvec) const;

  /**
   * Return true if both pose and camera_parameters are specified
   */
  bool isFullySpecified() const;

private:
  cv::Mat img;

  CameraMetaInformation camera_meta;
};

}
