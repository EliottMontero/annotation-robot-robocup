#pragma once

#include <hl_monitoring/camera.pb.h>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace hl_monitoring {

/**
 * Contains all the information about the size of the field, also allows to
 * retrieve points of interest
 *
 *
 * Referential is as follows:
 * - Zero: Center of field at ground level
 * - X-axis goes from center of the field to the center of the post at the right side of the team area
 * - Y-axis goes from center toward side line opposite to team area
 * - Z-axis points toward the roof
 *
 *                         y
 *  |----------------------------------------------|
 *  |                      |                       |
 *  |                      |                       |
 *  |                      |                       |
 *  |                      |                       |
 *  |                      0--->x                  |
 *  |                      |                       |
 *  |                      |                       |
 *  |                      |                       |
 *  |                      |                       |
 *  |----------------------------------------------|
 *
 *         |--------------------------------|
 *         |                                |
 *         |           Team area            |
 *         |                                |
 *         |--------------------------------|
 *
 * - All field informations are in meters
 */
class Field {
public:

  typedef std::pair<cv::Point3f, cv::Point3f> Segment;

  Field();

  Json::Value toJson() const;
  void fromJson(const Json::Value & v);
  void loadFile(const std::string & path);

  const std::map<std::string, cv::Point3f> & getPointsOfInterest() const;
  const std::vector<Segment> & getWhiteLines() const;

  void tagLines(const CameraMetaInformation & camera_information, cv::Mat * tag_img,
                const cv::Scalar & line_color, double line_thickness);
  void tagLines(const cv::Mat & camera_matrix, const cv::Mat & distortion_coeffs,
                const cv::Mat & rvec, const cv::Mat & tvec, cv::Mat * tag_img,
                const cv::Scalar & line_color, double line_thickness);

  /*
   * Radius of the ball [m]
   */
  double ball_radius;

  /*
   *  The width of white lines [m]
   */
  double line_width;

  /*
   *  Radius of the central circle (lines included) [m]
   */
  double center_radius;

  /*
   *  Distance from fieldBorder to arenaBorder (line excluded) [m]
   */
  double border_strip_width;

  /*
   *  Distance from center of penaltyMark to closest goal (goal line included) [m]
   */
  double penalty_mark_dist;

  /*
   *  Length of the penalty mark [m]
   */
  double penalty_mark_length;

  /*
   *  Distance between two posts (posts excluded) [m]
   */
  double goal_width;

  /*
   *  From goal line to the back of the goal (goal line included) [m]
   */
  double goal_depth;

  /*
   *  From goal line to goal area line (lines included) [m]
   */
  double goal_area_length;

  /*
   *  From one side of goal area to the other (lines included) [m]
   */
  double goal_area_width;

  /*
   *  From one goal line to the other (lines included) [m]
   */
  double field_length;

  /*
   *  From one side of the field to the other (lines included) [m]
   */
  double field_width;
  

private:
  
  /**
   * Synchronize the points of interests with current size of the field
   */
  void updatePointsOfInterest();
  
  /**
   * Synchronize the white lines based on the points of interests
   */
  void updateWhiteLines();

  /**
   * Stores points of interest visually identifiable of the field along with a given name
   * e.g.: corners penalty marks etc...
   */
  std::map<std::string, cv::Point3f> points_of_interest;

  /**
   * List all the white segments in the field
   */
  std::vector<Segment> white_lines;
};

}
