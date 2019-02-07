#include "hl_monitoring/opencv_image_provider.h"

#include <hl_communication/utils.h>

#include <chrono>
#include <fstream>

using namespace std::chrono;
using namespace hl_communication;

namespace hl_monitoring
{

OpenCVImageProvider::OpenCVImageProvider(const std::string & video_path,
                                         const std::string & output_prefix_)
  : output_prefix(output_prefix_)
{
  openInputStream(video_path);
  if (output_prefix != "") {
    openOutputStream(output_prefix + ".avi");
  }
}
OpenCVImageProvider::~OpenCVImageProvider() {
  saveVideoMetaInformation();
}


double OpenCVImageProvider::getFPS() const {
  return input.get(cv::CAP_PROP_FPS);
}

void OpenCVImageProvider::openInputStream(const std::string & video_path) {
  if (!input.open(video_path)) {
    throw std::runtime_error(HL_DEBUG + " failed to open device '" + video_path + "'");
  }
  int read_width = input.get(cv::CAP_PROP_FRAME_WIDTH);
  int read_height = input.get(cv::CAP_PROP_FRAME_HEIGHT);
  img_size = cv::Size(read_width, read_height);
  if (meta_information.has_camera_parameters()) {
    // Intrinsic parameters have been provided, check consistency
    int expected_width = meta_information.camera_parameters().img_width();
    int expected_height = meta_information.camera_parameters().img_height();
    if (expected_width != read_width || expected_height != read_height) {
      std::ostringstream oss;
      oss << HL_DEBUG << " mismatch of sizes: expecting "
          << expected_width << "*" << expected_height
          << ", stream size " << read_width << "*" << read_height;
      throw std::runtime_error(oss.str());
    }
  }
}

void OpenCVImageProvider::openOutputStream(const std::string & output_path) {
  if (!input.isOpened()) {
    throw std::logic_error(HL_DEBUG + " input stream is not open yet");
  }
  double fps = getFPS();
  bool use_color = true;
  output.open(output_path, cv::VideoWriter::fourcc('X','V','I','D'), fps, img_size, use_color);
  if (!output.isOpened()) {
    throw std::runtime_error(HL_DEBUG + "Failed to open video");
  }
}

void OpenCVImageProvider::restartStream() {
  throw std::logic_error("It makes no sense to restart the stream in a 'OpenCVImageProvider'");
}

CalibratedImage OpenCVImageProvider::getCalibratedImage(uint64_t time_stamp) {
  if (nb_frames == 0) {
    throw std::runtime_error(HL_DEBUG + " no frames found in the stream");
  }
  if (time_stamp < indices_by_time_stamp.rbegin()->first) {
    throw std::runtime_error(HL_DEBUG + " asking for frames in the past is not supported");
  }

  int index = indices_by_time_stamp.size() - 1;

  CameraMetaInformation camera_meta;
  if (meta_information.has_camera_parameters()) {
    camera_meta.mutable_camera_parameters()->CopyFrom(meta_information.camera_parameters());
  }
  
  const FrameEntry & frame = meta_information.frames(index);
  if (frame.has_pose()) {
    camera_meta.mutable_pose()->CopyFrom(frame.pose());
  } else if (meta_information.has_default_pose()) {
    camera_meta.mutable_pose()->CopyFrom(meta_information.default_pose());
  }

  return CalibratedImage(img, camera_meta);
}

void OpenCVImageProvider::update() {
  //TODO: note: ideally, images should be polled in another thread and only
  //synchronization should happen here
  getNextImg();
}

cv::Mat OpenCVImageProvider::getNextImg() {
  input.read(img);

  uint64_t time_stamp = getTimeStamp();
  if (img.empty()) {
    throw std::runtime_error(HL_DEBUG + "Blank frame at frame: "
                             + std::to_string(index) + "/" + std::to_string(nb_frames));
  }
  // register image
  indices_by_time_stamp[time_stamp] = index;
  FrameEntry * entry  = meta_information.add_frames();
  entry->set_time_stamp(time_stamp);
  index++;
  nb_frames++;
  // Write image to output video if opened
  if (output.isOpened()) {
    output.write(img);
  }
  return img;
}

bool OpenCVImageProvider::isStreamFinished() {
  return false;
}

void OpenCVImageProvider::saveVideoMetaInformation() {
  // Do not save if no output_prefix has been provided
  if (output_prefix == "")
    return;
  
  std::string path = output_prefix + ".bin"; 
  std::ofstream out(path, std::ios::binary);
  if (!out.good()) {
    throw std::runtime_error(HL_DEBUG + "Failed to open file '" + path + "'");
  }
  if (!meta_information.SerializeToOstream(&out)) {
    throw std::runtime_error(HL_DEBUG + "Failed to write to file '" + path + "'");
  }
}

uint64_t OpenCVImageProvider::getStart() const {
  if (indices_by_time_stamp.size() == 0) {
    throw std::runtime_error(HL_DEBUG + " indices_by_time_stamp is empty");
  }
  return indices_by_time_stamp.begin()->first;
}

}
