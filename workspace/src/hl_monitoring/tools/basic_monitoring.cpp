/**
 * Acquire video and Game messages from a monitoring manager and displays them on screen.
 *
 * Depending on configuration of image providers, video streams and
 * meta_information are written
 */
#include <hl_communication/utils.h>
#include <hl_monitoring/field.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>

using namespace hl_communication;
using namespace hl_monitoring;

int main(int argc, char ** argv) {
  TCLAP::CmdLine cmd("Acquire and display one or multiple streams along with meta-information",
                     ' ', "0.9");

  TCLAP::ValueArg<std::string> config_arg("c", "config", "The path to the json configuration file",
                                          true, "config.json", "string");
  TCLAP::ValueArg<std::string> field_arg("f", "field", "The path to the json description of the file",
                                          true, "field.json", "string");
  TCLAP::SwitchArg verbose_arg("v", "verbose", "If enabled display all messages received",
                               cmd, false);
  cmd.add(config_arg);
  cmd.add(field_arg);

  try {
    cmd.parse(argc, argv);
    
  } catch (const TCLAP::ArgException & e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

  MonitoringManager manager;

  manager.loadConfig(config_arg.getValue());

  Field field;
  field.loadFile(field_arg.getValue());

  // While exit was not explicitly required, run
  uint64_t now = 0;
  uint64_t dt = 30 * 1000;//[microseconds]
  if (!manager.isLive()) {
    now = manager.getStart();
  }
  while(manager.isGood()) {
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += dt;
    }

    MessageManager::Status status = manager.getStatus(now);
    std::vector<cv::Scalar> team_colors = {cv::Scalar(255,0,255), cv::Scalar(255,255,0)};
    std::map<uint32_t,cv::Scalar> colors_by_team;
    for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
      const GCTeamMsg & team_msg = status.gc_message.teams(idx);
      if (team_msg.has_team_number() && team_msg.has_team_color()) {
        uint32_t team_number = team_msg.team_number();
        uint32_t team_color = team_msg.team_color();
        colors_by_team[team_number] = team_colors[team_color];
      }
    }

    if (verbose_arg.getValue()) {
      std::cout << "Time: " <<  now << std::endl;
      for (const auto & robot_entry : status.robot_messages) {
        std::cout << "-> Message from robot " << robot_entry.first.robot_id()
                  << " from team " << robot_entry.first.team_id() << std::endl;
      }
    }
    
    std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
    for (const auto & entry : images_by_source) {
      cv::Mat display_img = entry.second.getImg().clone();
      if (entry.second.isFullySpecified()) {
        const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
        field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
        // Basic drawing of robot estimated position
        for (const auto & robot_entry : status.robot_messages) {
          uint32_t team_id = robot_entry.first.team_id();
          if (colors_by_team.count(team_id) == 0) {
            std::cerr << "Unknown color for team " << team_id << std::endl;
          } else {
            const cv::Scalar & color = colors_by_team[team_id];
            if (robot_entry.second.has_perception()) {
              const Perception & perception = robot_entry.second.perception();
              for (int pos_idx = 0; pos_idx < perception.self_in_field_size(); pos_idx++) {
                const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
                const PositionDistribution & position = weighted_pose.pose().position();
                cv::Point3f pos_in_field(position.x(), position.y(), 0.0);
                cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
                int circle_size = 10;
                cv::circle(display_img, pos_in_img, circle_size, color, cv::FILLED);
              }
            }
          }
        }
      }
      cv::imshow(entry.first, display_img);
    }
    char key = cv::waitKey(10);
    if (key == 'q' || key == 'Q') break;
  }
}
