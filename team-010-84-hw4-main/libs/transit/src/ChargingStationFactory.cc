#include "ChargingStationFactory.h"

IEntity* ChargingStationFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("chargingStation") == 0) {
    std::cout << "Charging station Created" << std::endl;
    return new ChargingStation(entity);
  }
  return nullptr;
}
