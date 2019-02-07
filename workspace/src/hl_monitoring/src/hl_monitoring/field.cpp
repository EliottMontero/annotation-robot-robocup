#include "hl_monitoring/field.h"

#include <hl_communication/utils.h>
#include <hl_monitoring/utils.h>

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <fstream>

namespace hl_monitoring {

Field::Field() {
  ball_radius = 0.075;
  /// Expected field sizes
  line_width          = 0.05;
  center_radius       = 0.75;
  border_strip_width  = 0.70;//Minimum value
  penalty_mark_dist   = 2.10;
  penalty_mark_length = 0.10;
  goal_width          = 2.60;
  goal_depth          = 0.60;
  goal_area_length    = 1.00;
  goal_area_width     = 5.00;
  field_length        = 9.00;
  field_width         = 6.00;
}

Json::Value Field::toJson() const {
  Json::Value v;
  v["ball_radius"        ] = ball_radius         ;
  v["line_width"         ] = line_width          ;
  v["center_radius"      ] = center_radius       ;
  v["border_strip_width" ] = border_strip_width  ;
  v["penalty_mark_dist"  ] = penalty_mark_dist   ;
  v["penalty_mark_length"] = penalty_mark_length ;
  v["goal_width"         ] = goal_width          ;
  v["goal_depth"         ] = goal_depth          ;
  v["goal_area_length"   ] = goal_area_length    ;
  v["goal_area_width"    ] = goal_area_width     ;
  v["field_length"       ] = field_length        ;
  v["field_width"        ] = field_width         ;
  return v;
}

void Field::fromJson(const Json::Value & v) {
  readVal(v, "ball_radius"        , &ball_radius         );
  readVal(v, "line_width"         , &line_width          );
  readVal(v, "center_radius"      , &center_radius       );
  readVal(v, "border_strip_width" , &border_strip_width  );
  readVal(v, "penalty_mark_dist"  , &penalty_mark_dist   );
  readVal(v, "penalty_mark_length", &penalty_mark_length );
  readVal(v, "goal_width"         , &goal_width          );
  readVal(v, "goal_depth"         , &goal_depth          );
  readVal(v, "goal_area_length"   , &goal_area_length    );
  readVal(v, "goal_area_width"    , &goal_area_width     );
  readVal(v, "field_length"       , &field_length        );
  readVal(v, "field_width"        , &field_width         );
  updatePointsOfInterest();
  updateWhiteLines();
}

void Field::loadFile(const std::string & path) {
  std::ifstream in(path);
  if (!in.good()) {
    throw std::runtime_error(HL_DEBUG + " failed to open file '" + path + "'");
  }
  Json::Value root;
  in >> root;
  fromJson(root);
}

const std::map<std::string, cv::Point3f> & Field::getPointsOfInterest() const {
  return points_of_interest;
}

const std::vector<Field::Segment> & Field::getWhiteLines() const {
  return white_lines;
}


void Field::updatePointsOfInterest() {
  points_of_interest.clear();
  points_of_interest["center"] = cv::Point3f(0, 0, 0);
  double fc_x = field_length/2;
  double fc_y = field_width/2;
  points_of_interest["field_corner++"] = cv::Point3f( fc_x,  fc_y, 0);
  points_of_interest["field_corner+-"] = cv::Point3f( fc_x, -fc_y, 0);
  points_of_interest["field_corner-+"] = cv::Point3f(-fc_x,  fc_y, 0);
  points_of_interest["field_corner--"] = cv::Point3f(-fc_x, -fc_y, 0);
  double gac_x = field_length / 2 - goal_area_length;
  double gac_y = goal_area_width/2;
  points_of_interest["goal_area_corner++"] = cv::Point3f( gac_x,  gac_y, 0);
  points_of_interest["goal_area_corner+-"] = cv::Point3f( gac_x, -gac_y, 0);
  points_of_interest["goal_area_corner-+"] = cv::Point3f(-gac_x,  gac_y, 0);
  points_of_interest["goal_area_corner--"] = cv::Point3f(-gac_x, -gac_y, 0);
  double gat_x = field_length / 2;
  double gat_y = goal_area_width/2;
  points_of_interest["goal_area_t++"] = cv::Point3f( gat_x,  gat_y, 0);
  points_of_interest["goal_area_t+-"] = cv::Point3f( gat_x, -gat_y, 0);
  points_of_interest["goal_area_t-+"] = cv::Point3f(-gat_x,  gat_y, 0);
  points_of_interest["goal_area_t--"] = cv::Point3f(-gat_x, -gat_y, 0);
  double pm_x = field_length / 2 - penalty_mark_dist;
  points_of_interest["penalty_mark+"] = cv::Point3f( pm_x, 0, 0);
  points_of_interest["penalty_mark-"] = cv::Point3f(-pm_x, 0, 0);
  double mlt_y = field_width / 2;
  points_of_interest["middle_line_t+"] = cv::Point3f(0,  mlt_y, 0);
  points_of_interest["middle_line_t-"] = cv::Point3f(0, -mlt_y, 0);
}

void Field::updateWhiteLines() {
  white_lines.clear();
  white_lines.push_back({points_of_interest["field_corner++"],
        points_of_interest["field_corner+-"]});
  white_lines.push_back({points_of_interest["field_corner+-"],
        points_of_interest["field_corner--"]});
  white_lines.push_back({points_of_interest["field_corner--"],
        points_of_interest["field_corner-+"]});
  white_lines.push_back({points_of_interest["field_corner-+"],
        points_of_interest["field_corner++"]});
  white_lines.push_back({points_of_interest["middle_line_t+"],
        points_of_interest["middle_line_t-"]});
  white_lines.push_back({points_of_interest["goal_area_t++"],
        points_of_interest["goal_area_corner++"]});
  white_lines.push_back({points_of_interest["goal_area_corner++"],
        points_of_interest["goal_area_corner+-"]});
  white_lines.push_back({points_of_interest["goal_area_corner+-"],
        points_of_interest["goal_area_t+-"]});
  white_lines.push_back({points_of_interest["goal_area_t-+"],
        points_of_interest["goal_area_corner-+"]});
  white_lines.push_back({points_of_interest["goal_area_corner-+"],
        points_of_interest["goal_area_corner--"]});
  white_lines.push_back({points_of_interest["goal_area_corner--"],
        points_of_interest["goal_area_t--"]});
}

void Field::tagLines(const CameraMetaInformation & camera_information, cv::Mat * tag_img,
                     const cv::Scalar & line_color, double line_thickness) {
  if (!camera_information.has_camera_parameters() || !camera_information.has_pose()) {
    throw std::runtime_error(HL_DEBUG + " camera_information is not fully specified");
  }
  cv::Mat camera_matrix, distortion_coefficients, rvec, tvec;
  cv::Size size;
  intrinsicToCV(camera_information.camera_parameters(),
                &camera_matrix, &distortion_coefficients, &size);
  pose3DToCV(camera_information.pose(), &rvec, &tvec);
  if (size.width != tag_img->cols || size.height != tag_img->rows) {
    std::ostringstream oss;
    oss << HL_DEBUG << " size mismatch " << size  << " != " << tag_img->size;
    throw std::runtime_error(oss.str());
  }
  tagLines(camera_matrix, distortion_coefficients, rvec, tvec, tag_img, line_color, line_thickness);
}

void Field::tagLines(const cv::Mat & camera_matrix, const cv::Mat & distortion_coeffs,
                     const cv::Mat & rvec, const cv::Mat & tvec, cv::Mat * tag_img,
                     const cv::Scalar & line_color, double line_thickness) {
  for (const auto & segment : getWhiteLines()) {
    std::vector<cv::Point3f> object_points = {segment.first, segment.second};
    std::vector<cv::Point2f> img_points;
    cv::projectPoints(object_points, rvec, tvec, camera_matrix, distortion_coeffs, img_points);
    // When point is outside of image, screw up the drawing
    cv::Rect img_rect(cv::Point(), tag_img->size());
    if (img_rect.contains(img_points[0]) && img_rect.contains(img_points[1])) {
      cv::line(*tag_img, img_points[0], img_points[1], line_color, line_thickness);
    }
  }
}


}
