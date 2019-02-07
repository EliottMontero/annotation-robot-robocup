#pragma once

#include "hl_monitoring/image_provider.h"

#include <opencv2/videoio.hpp>


namespace hl_monitoring
{

class ReplayImageProvider : public ImageProvider {
public:

  ReplayImageProvider();
  ReplayImageProvider(const std::string & video_path);
  ReplayImageProvider(const std::string & video_path,
                      const std::string & meta_information_path);

  int getNbFrames() const;

  void loadVideo(const std::string & video_path);
  void loadMetaInformation(const std::string & meta_information_path);

  void restartStream() override;

  CalibratedImage getCalibratedImage(uint64_t time_stamp) override;
  
  cv::Mat getNextImg() override;

  void update() override;

  bool isStreamFinished() override;

  void setIndex(int index);
  int getIndex(uint64_t time_stamp) const;

  uint64_t getStart() const override;

private:
  /**
   * The video read from the file
   */
  cv::VideoCapture video;

  /**
   * Information relevant
   */
  VideoMetaInformation meta_information;

  /**
   * Provide access to index using time_stamps
   */
  std::map<uint64_t, int> indices_by_time_stamp;

  /**
   * Index of the next image read in the video
   */
  int index;

  /**
   * The number of frames in the video
   */
  int nb_frames;
};

}
