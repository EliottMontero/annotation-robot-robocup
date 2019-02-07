#include <hl_communication/utils.h>
#include <hl_monitoring/camera.pb.h>
#include <hl_monitoring/replay_image_provider.h>

#include <tclap/CmdLine.h>

using namespace hl_communication;
using namespace hl_monitoring;

int main(int argc, char ** argv) {
  TCLAP::CmdLine cmd("Combine multiple type of files to create meta information for a video",
                     ' ', "0.9");

  TCLAP::ValueArg<std::string> video_arg("v", "video",
                                         "Path to a video, if specified, use the video as a "
                                         "source to create the time_stamps for each frame",
                                         false,"", "path", cmd);
  TCLAP::ValueArg<std::string> intrinsic_arg("i", "intrinsic",
                                             "Path to the file describing intrinsic parameters "
                                             "of the camera.", false, "","path", cmd);
  TCLAP::ValueArg<std::string> meta_arg("m", "meta-information",
                                        "Path to a file containing initial meta-information",
                                        false, "","path", cmd);
  TCLAP::ValueArg<std::string> pose_arg("p", "pose",
                                        "Path to the file describing the pose of the camera",
                                        false, "","path", cmd);
  TCLAP::ValueArg<std::string> output_arg("o","output",
                                          "The output path for the meta_information",
                                          true, "meta_information.bin", "path", cmd);
  TCLAP::ValueArg<double> dt_arg("t","step_time",
                                 "The interval between two frames when a video is provided",
                                 false, 0.03, "seconds", cmd);
  TCLAP::SwitchArg force_switch("f", "force",
                                "Allows to overwrite existing data in meta-information",
                                cmd, false);
  try {
    cmd.parse(argc, argv);
  } catch (const TCLAP::ArgException & e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    exit(EXIT_FAILURE);
  }

  VideoMetaInformation information;

  bool force = force_switch.getValue();

  if (meta_arg.getValue() != "") {
    readFromFile(meta_arg.getValue(), &information);
  }
  if (pose_arg.getValue() != "") {
    if (!force && information.has_default_pose()) {
      throw std::runtime_error(HL_DEBUG
                               + "video meta information already contains default pose."
                               " use -f to overwrite");
    }
    readFromFile(pose_arg.getValue(), information.mutable_default_pose());
  }
  if (intrinsic_arg.getValue() != "") {
    if (!force && information.has_camera_parameters()) {
      throw std::runtime_error(HL_DEBUG
                               + "video meta information already contains camera_parameters."
                               " use -f to overwrite");
    }
    readFromFile(intrinsic_arg.getValue(), information.mutable_camera_parameters());
  }
  if (video_arg.getValue() != "") {
    if (!force && information.frames_size() > 0) {
      throw std::runtime_error(HL_DEBUG
                               + "video meta information already contains frame entries."
                               " use -f to overwrite");
    }
    ReplayImageProvider video(video_arg.getValue());
    double dt = dt_arg.getValue();
    int nb_frames = video.getNbFrames();
    information.clear_frames();
    for (int i = 0; i < nb_frames; i++) {
      uint64 time_stamp = dt * i * 1000 * 1000;
      information.add_frames()->set_time_stamp(time_stamp);
    }
  }

  writeToFile(output_arg.getValue(), information);
}

