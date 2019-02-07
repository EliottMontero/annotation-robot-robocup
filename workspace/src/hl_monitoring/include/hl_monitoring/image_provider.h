#pragma once

#include "hl_monitoring/calibrated_image.h"

namespace hl_monitoring
{

class ImageProvider {
public:

  virtual ~ImageProvider(){}

  /**
   * Return to the first frame of the stream, throw error on live stream
   */
  virtual void restartStream() = 0;
  
  /**
   * Return an image along with associated intrinsic and extrinsic parameters
   */
  virtual CalibratedImage getCalibratedImage(uint64_t time_stamp) = 0;

  /**
   * For livestream, receive images from the stream
   */
  virtual void update() = 0;

  /**
   * Get the next available image
   */
  virtual cv::Mat getNextImg() = 0;

  /**
   * Return true if the last image of the stream has been reached.
   * For live streams, means that the camera has been disconnected 
   */
  virtual bool isStreamFinished() = 0;

  /**
   * Return the first time_stamp of the images received
   */
  virtual uint64_t getStart() const = 0;
};

}
