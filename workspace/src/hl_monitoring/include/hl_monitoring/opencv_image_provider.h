#pragma once

#include "hl_monitoring/image_provider.h"

#include <opencv2/videoio.hpp>


namespace hl_monitoring
{

/**
 * Use OpenCV standard API to open a video stream
 * - Images read can be directly encoded in a video
 * - Timestamps are based on acquisition time
 */
class OpenCVImageProvider : public ImageProvider {
public:
  /**
   * If output_prefix is not empty, write video during execution and saves
   * MetaInformation when object is closed
   */
  OpenCVImageProvider(const std::string & video_path,
                      const std::string & output_prefix = "");
  virtual ~OpenCVImageProvider();

  double getFPS() const;

  void openInputStream(const std::string & video_path);
  void openOutputStream(const std::string & output_path);
  
  void restartStream() override;

  CalibratedImage getCalibratedImage(uint64_t time_stamp) override;

  void update() override;
  
  cv::Mat getNextImg() override;

  bool isStreamFinished() override;

  void saveVideoMetaInformation();

  uint64_t getStart() const override;

private:
  /**
   * The video read from the file
   */
  cv::VideoCapture input;
  
  /**
   * The video read from the file
   */
  cv::VideoWriter output;

  /**
   * Information relevant
   */
  VideoMetaInformation meta_information;

  /**
   * Last img read
   */
  cv::Mat img;

  /**
   * Size of the images provided by the camera
   */
  cv::Size img_size;

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

  /**
   * The prefix used for writing video file and meta_information file. If empty,
   * then no files are written
   */
  std::string output_prefix;
};

}
