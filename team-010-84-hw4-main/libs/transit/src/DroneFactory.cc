#include "DroneFactory.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("lightDrone") == 0 ||
      type.compare("mediumDrone") == 0 ||
      type.compare("heavyDrone") == 0) {
    std::cout << "Drone Created" << std::endl;
    return new Drone(entity);
  }
  return nullptr;
}
