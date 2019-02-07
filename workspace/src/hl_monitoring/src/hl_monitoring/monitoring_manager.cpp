#include "hl_monitoring/monitoring_manager.h"

#include <hl_communication/utils.h>
#include <hl_monitoring/opencv_image_provider.h>
#include <hl_monitoring/replay_image_provider.h>
#include <hl_monitoring/utils.h>

#include <fstream>

using namespace hl_communication;

namespace hl_monitoring
{

MonitoringManager::MonitoringManager() {
}

MonitoringManager::~MonitoringManager() {
  if (msg_collection_path != "") {
    message_manager->saveMessages(msg_collection_path);
  }
  for (auto & entry : image_providers) {
    delete(entry.second.release());
  }
}


void MonitoringManager::loadConfig(const std::string & path) {
  // Reading Json file
  std::ifstream in(path);
  if (!in.good()) {
    throw std::runtime_error(HL_DEBUG + " failed to open file '" + path + "'");
  }
  Json::Value root;
  in >> root;
  // Parsing json content
  checkMember(root, "image_providers");
  checkMember(root, "message_manager");
  loadImageProviders(root["image_providers"]);
  loadMessageManager(root["message_manager"]);
  readVal(root, "live", &live);
  tryReadVal(root, "msg_collection_path", &msg_collection_path);
}

std::unique_ptr<ImageProvider> MonitoringManager::buildImageProvider(const Json::Value & v) {
  checkMember(v, "class_name");
  checkMember(v, "input_path");
  std::unique_ptr<ImageProvider> result;
  std::string class_name, input_path;
  readVal(v,"class_name", &class_name);
  readVal(v,"input_path", &input_path);
  if (class_name == "OpenCVImageProvider") {
    std::string output_prefix;
    if (v.isMember("output_prefix")) {
      result.reset(new OpenCVImageProvider(input_path, v["output_prefix"].asString()));
    } else { 
      result.reset(new OpenCVImageProvider(input_path));
    }
  } else if (class_name == "ReplayImageProvider") {
    std::string meta_information_path;
    if (v.isMember("meta_information_path")) {
      result.reset(new ReplayImageProvider(input_path, v["meta_information_path"].asString()));
    } else {
      result.reset(new ReplayImageProvider(input_path));
    }
  }
  return result;
}

void MonitoringManager::loadImageProviders(const Json::Value & v) {
  if (!v.isObject()) {
    throw std::runtime_error(HL_DEBUG + " invalid type for v, expecting an object");
  }
  for (Json::ValueConstIterator it = v.begin(); it != v.end(); it++) {
    const std::string & key = it.name();
    addImageProvider(key, buildImageProvider(v[key]));
  }
}

void MonitoringManager::loadMessageManager(const Json::Value & v) {
  if (!v.isObject()) {
    throw std::runtime_error(HL_DEBUG + " invalid type for v, expecting an object");
  }
  std::string file_path;
  std::vector<int> ports;
  tryReadVal(v, "file_path", &file_path);
  if (v.isMember("ports")) {
    if (!v["ports"].isArray()) {
      throw std::runtime_error(HL_DEBUG + " expecting an array of int for ports");
    }
    for (Json::ArrayIndex idx = 0; idx < v["ports"].size(); idx++) {
      if (!v["ports"][idx].isInt()) {
        throw std::runtime_error(HL_DEBUG + " value at index " + std::to_string(idx)
                                 + " of ports is not an int");
      }
      ports.push_back(v["ports"][idx].asInt());
    }
  }
  bool ports_set = ports.size() != 0;
  bool file_path_set = file_path != "";
  if (!ports_set && !file_path_set) {
    throw std::runtime_error(HL_DEBUG + " neither 'ports' nor 'file_path' provided");
  } else if (ports_set && file_path_set) {
    throw std::runtime_error(HL_DEBUG + " both 'ports' and 'file_path' provided");
  } if (ports_set) {
    message_manager.reset(new MessageManager(ports));
  } else {
    message_manager.reset(new MessageManager(file_path));
  }
}

void MonitoringManager::setMessageManager(std::unique_ptr<MessageManager> new_message_manager) {
  message_manager = std::move(new_message_manager);
}

void MonitoringManager::addImageProvider(const std::string & name,
                                         std::unique_ptr<ImageProvider> image_provider) {
  if (image_providers.count(name) > 0) {
    throw std::logic_error("Failed to add Image Provider: '" + name
                           + "' already in collection");
  }
  image_providers[name] = std::move(image_provider);
}

void MonitoringManager::update() {
  for (const auto & entry : image_providers) {
    entry.second->update();
  }
  message_manager->update();
}

std::map<std::string, CalibratedImage>
MonitoringManager::getCalibratedImages(uint64_t time_stamp) {
  std::map<std::string, CalibratedImage> images;
  for (const auto & entry : image_providers) {
    images[entry.first] = entry.second->getCalibratedImage(time_stamp);
  }
  return images;
}

hl_communication::MessageManager::Status MonitoringManager::getStatus(uint64_t time_stamp) {
  return message_manager->getStatus(time_stamp);
  
}

uint64_t MonitoringManager::getStart() const {
  uint64_t min_ts = std::numeric_limits<uint64_t>::max();
  min_ts = std::min(min_ts, message_manager->getStart());
  for (const auto & entry : image_providers) {
    min_ts = std::min(min_ts, entry.second->getStart());
  }
  return min_ts;
}

bool MonitoringManager::isGood() const {
  for (const auto & entry : image_providers) {
    if (entry.second->isStreamFinished())
      return false;
  }
  return true;
}

bool MonitoringManager::isLive() const {
  return live;
}


}
