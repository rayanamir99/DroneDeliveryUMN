#include "ChargingStation.h"

ChargingStation::ChargingStation(JsonObject& obj) : IEntity(obj) {
  slotsAvailible = obj["slots"];
  chargeRate = obj["charge_speed"];
  droneCurCharge.assign(slotsAvailible, nullptr);
}

ChargingStation::~ChargingStation() {}

void ChargingStation::update(double dt) {
  rechargeDrone(dt);
  while (slotsAvailible > 0) {
    if (droneQueue.size() == 0) return;
    for (int i = 0; i < droneCurCharge.size(); i++) {
      if (droneCurCharge.at(i) == nullptr) {
        droneCurCharge.at(i) = droneQueue.front();
        break;
      }
    }
    droneQueue.pop();
    slotsAvailible -= 1;
  }
  return;
}

void ChargingStation::rechargeDrone(double dt) {
  for (int i = 0; i < droneCurCharge.size(); i++) {
    Drone* drone = droneCurCharge.at(i);
    if (!drone) continue;
    if (drone->getBatteryCapacity() >
        static_cast<int>(drone->getBatteryCharge())) {
      drone->recharge(static_cast<double>(chargeRate) * dt);
    } else {
      unplugDrone(drone);
    }
  }
}

void ChargingStation::queueUp(Drone* drone) { droneQueue.push(drone); }

void ChargingStation::unplugDrone(Drone* drone) {
  for (int i = 0; i < droneCurCharge.size(); i++) {
    if (droneCurCharge.at(i) == drone) {
      if (droneQueue.size() == 0) {
        droneCurCharge.at(i) = nullptr;
        slotsAvailible += 1;
        return;
      }
      droneCurCharge.at(i) = droneQueue.front();
      droneQueue.pop();
      return;
    }
  }
}
