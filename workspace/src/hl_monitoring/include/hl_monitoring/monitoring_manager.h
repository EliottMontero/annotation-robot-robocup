#pragma once

#include <hl_monitoring/image_provider.h>
#include <hl_communication/message_manager.h>

#include <json/json.h>
#include <memory>

namespace hl_monitoring
{

/**
 * Manage the monitoring of a game or a replay for the RoboCup humanoid league
 *
 * Provide access to the following elements:
 * - Status message sent by the robot
 * - Game Controller status
 * - Named video streams with parameters of the cameras (intrinsic+extrinsic)
 */
class MonitoringManager {
public:
  MonitoringManager();
  ~MonitoringManager();

  void loadConfig(const std::string & path);

  std::unique_ptr<ImageProvider> buildImageProvider(const Json::Value & v);

  void loadImageProviders(const Json::Value & v);
  void loadMessageManager(const Json::Value & v);

  void setMessageManager(std::unique_ptr<hl_communication::MessageManager> message_manager);
  void addImageProvider(const std::string & name,
                        std::unique_ptr<ImageProvider> image_provider);

  void update();

  std::map<std::string, CalibratedImage> getCalibratedImages(uint64_t time_stamp);

  hl_communication::MessageManager::Status getStatus(uint64_t time_stamp);

  /**
   * Return the first time_stamp found in messages and video streams
   */
  uint64_t getStart() const;
  
  bool isGood() const;

  bool isLive() const;

private:
  /**
   * Access to message from both, robots and GameController
   */
  std::unique_ptr<hl_communication::MessageManager> message_manager;

  /**
   * Access to all the channels allowing to retrieve images
   */
  std::map<std::string,std::unique_ptr<ImageProvider>> image_providers;

  /**
   * Path to the output file where all the received messages will be stored upon deletion
   * - If empty, messages are not saved
   */
  std::string msg_collection_path;

  /**
   * Is the monitoring session live or not?
   */
  bool live;

};

}
