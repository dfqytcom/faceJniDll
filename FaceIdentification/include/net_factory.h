#ifndef NET_FACTORY_H_
#define NET_FACTORY_H_

#include "net.h"

#include <map>
#include <string>
#include <iostream>
#include <memory>

class NetRegistry {
 public:
  typedef std::shared_ptr<Net> (*Creator)();
  typedef std::map<std::string, Creator> CreatorRegistry;

  static CreatorRegistry& Registry() {
    static CreatorRegistry* g_registry_ = new CreatorRegistry();
    return *g_registry_;
  }

  static void AddCreator(const std::string& type, Creator creator) {
    CreatorRegistry& registry = Registry();
    if (registry.count(type) != 0) {
      LOG(INFO) << "Net type " << type << " already registered.";
    }
    registry[type] = creator;
  }

  static std::shared_ptr<Net> CreateNet(const std::string type) {
    CreatorRegistry& registry = Registry();
    if (registry.count(type) != 1) {
      LOG(ERROR) << "Net type " << type << " haven't registered.";
    }
    return registry[type]();
  }
 private:
  NetRegistry() {}

};

class NetRegisterer {
 public:
  NetRegisterer(const std::string& type,
                std::shared_ptr<Net> (*creator)()) {
    LOG(INFO) << "Registering net type: " << type;
    NetRegistry::AddCreator(type, creator);
  }
};

#define REGISTER_NET_CREATOR(type, creator)                                    \
  static NetRegisterer g_creator_##type(#type, creator)

#define REGISTER_NET_CLASS(type)                                               \
  std::shared_ptr<Net> Creator_##type##Net()                                   \
  {                                                                            \
    return std::shared_ptr<Net>(new type##Net());                              \
  }                                                                            \
  REGISTER_NET_CREATOR(type, Creator_##type##Net);                             \
  static type##Net type
#endif //NET_FACTORY_H_
