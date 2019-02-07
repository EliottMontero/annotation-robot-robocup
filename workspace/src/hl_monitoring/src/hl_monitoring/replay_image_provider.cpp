#include "hl_monitoring/replay_image_provider.h"

#include <hl_communication/utils.h>

#include <fstream>
#include <iostream>

namespace hl_monitoring
{

ReplayImageProvider::ReplayImageProvider()
  : index(0), nb_frames(0)
{
}

ReplayImageProvider::ReplayImageProvider(const std::string & video_path)
{
  loadVideo(video_path);
}

ReplayImageProvider::ReplayImageProvider(const std::string & video_path,
                                         const std::string & meta_information_path) {
  loadVideo(video_path);
  loadMetaInformation(meta_information_path);
}

int ReplayImageProvider::getNbFrames() const {
  return nb_frames;
}

void ReplayImageProvider::loadVideo(const std::string & video_path) {
  if (!video.open(video_path)) {
    throw std::runtime_error("Failed to open video '" + video_path + "'");
  }
  index = 0;
  nb_frames = video.get(cv::CAP_PROP_FRAME_COUNT);
}

void ReplayImageProvider::loadMetaInformation(const std::string & meta_information_path) {
  std::ifstream in(meta_information_path, std::ios::binary);
  if (!in.good()) {
    throw std::runtime_error("Failed to open file '" + meta_information_path + "'");
  }
  if (!meta_information.ParseFromIstream(&in)) {
    throw std::runtime_error("Failed to read file '" + meta_information_path + "'");
  }
  index = 0;
  nb_frames = meta_information.frames_size();
  std::cout << "After loading meta informations: " << nb_frames << " frames" << std::endl;
  for (int idx = 0; idx < nb_frames; idx++) {
    uint64_t time_stamp =  meta_information.frames(idx).time_stamp();
    if (indices_by_time_stamp.count(time_stamp) > 0) {
      throw std::runtime_error(HL_DEBUG + "Duplicated time_stamp "
                               + std::to_string(time_stamp));
    }
    indices_by_time_stamp[time_stamp] = idx;
  }
}

void ReplayImageProvider::restartStream() {
  setIndex(0);
}

CalibratedImage ReplayImageProvider::getCalibratedImage(uint64_t time_stamp) {
  setIndex(getIndex(time_stamp));

  cv::Mat img;
  video.read(img);
  if (img.empty()) {
    throw std::runtime_error(HL_DEBUG + "Blank frame has been read");
  }
  
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

cv::Mat ReplayImageProvider::getNextImg() {
  if (isStreamFinished()) {
    throw std::logic_error("Asking for a new frame while stream is finished");
  }

  cv::Mat img;
  video.read(img);
  index++;
  if (img.empty()) {
    throw std::runtime_error(HL_DEBUG + "Blank frame at frame: "
                             + std::to_string(index) + "/" + std::to_string(nb_frames));
  }
  return img;
}

void ReplayImageProvider::update() {
  // Nothing required
}

bool ReplayImageProvider::isStreamFinished() {
  return index >= nb_frames;
}

void ReplayImageProvider::setIndex(int new_index) {
  index = new_index;
  if (!video.set(cv::CAP_PROP_POS_FRAMES, index)) {
    throw std::runtime_error(HL_DEBUG + "Failed to set index to "
                             + std::to_string(index) + " in video");
  }
}
int ReplayImageProvider::getIndex(uint64_t time_stamp) const {
  if (indices_by_time_stamp.size() == 0) {
    throw std::runtime_error(HL_DEBUG + "indices_by_time_stamp is empty");
  }
  auto it = indices_by_time_stamp.upper_bound(time_stamp);
  if (it == indices_by_time_stamp.end() || it->first > time_stamp) {
    it--;
  }
  return it->second;
}

uint64_t ReplayImageProvider::getStart() const {
  if (indices_by_time_stamp.size() == 0) {
    throw std::runtime_error(HL_DEBUG + " indices_by_time_stamp is empty");
  }
  return indices_by_time_stamp.begin()->first;
}

}
