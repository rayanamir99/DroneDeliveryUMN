#include "SimulationModel.h"

#include "ChargingStationFactory.h"
#include "DroneFactory.h"
#include "HelicopterFactory.h"
#include "HumanFactory.h"
#include "PackageFactory.h"
#include "RobotFactory.h"

SimulationModel::SimulationModel(IController& controller)
    : controller(controller) {
  entityFactory.AddFactory(new DroneFactory());
  entityFactory.AddFactory(new PackageFactory());
  entityFactory.AddFactory(new RobotFactory());
  entityFactory.AddFactory(new HumanFactory());
  entityFactory.AddFactory(new HelicopterFactory());
  entityFactory.AddFactory(new ChargingStationFactory());
}

SimulationModel::~SimulationModel() {
  // Delete dynamically allocated variables
  for (auto& [id, entity] : entities) {
    delete entity;
  }
  delete graph;
}

IEntity* SimulationModel::createEntity(JsonObject& entity) {
  // TODO(username): problem after creating drone(s)
  std::string name = entity["name"];
  JsonArray position = entity["position"];
  // std::cout << name << ": " << position << std::endl;

  IEntity* myNewEntity = nullptr;
  if (myNewEntity = entityFactory.CreateEntity(entity)) {
    // Call AddEntity to add it to the view
    myNewEntity->linkModel(this);
    controller.addEntity(*myNewEntity);
    entities[myNewEntity->getId()] = myNewEntity;
  }
  // std::cout << "Created entity succesfully\n";
  return myNewEntity;
}

void SimulationModel::removeEntity(int id) { removed.insert(id); }

/// Schedules a Delivery for an object in the scene
void SimulationModel::scheduleTrip(JsonObject& details) {
  std::string name = details["name"];
  JsonArray start = details["start"];
  JsonArray end = details["end"];
  // std::cout << name << ": " << start << " --> " << end << std::endl;

  Robot* receiver = nullptr;

  for (auto& [id, entity] : entities) {
    if (name == entity->getName()) {
      if (Robot* r = dynamic_cast<Robot*>(entity)) {
        if (r->requestedDelivery) {
          receiver = r;
          break;
        }
      }
    }
  }

  Package* package = nullptr;

  for (auto& [id, entity] : entities) {
    if (name + "_package" == entity->getName()) {
      if (Package* p = dynamic_cast<Package*>(entity)) {
        if (p->requiresDelivery) {
          package = p;
          break;
        }
      }
    }
  }

  if (receiver && package) {
    package->initDelivery(receiver);
    package->setPackageWeight(details["weight"]);
    std::string strategyName = details["search"];
    package->setStrategyName(strategyName);
    scheduledDeliveries.push_back(package);
    controller.sendEventToView("DeliveryScheduled", details);
  }
}

const routing::IGraph* SimulationModel::getGraph() { return graph; }

ChargingStation* SimulationModel::getClosestRechargeStation(Vector3 position) {
  ChargingStation* bestStation = nullptr;
  double bestDist = -1;
  for (auto& [id, entity] : entities) {
    if (ChargingStation* c = dynamic_cast<ChargingStation*>(entity)) {
      // get distance from current station to package
      double curDist = position.dist(c->getPosition());
      if (bestDist == -1) {
        bestDist = curDist;
        bestStation = c;
      } else if (curDist < bestDist) {
        bestDist = curDist;
        bestStation = c;
      }
    }
  }
  return bestStation;
}

/// Updates the simulation
void SimulationModel::update(double dt) {
  // std::cout << "Updating\n";
  JsonArray batteryCharges;
  int droneIdx = 0;
  for (auto& [id, entity] : entities) {  // update all entities
    entity->update(dt);
    controller.updateEntity(*entity);
    //
    Drone* d = dynamic_cast<Drone*>(entity);
    if (d) {
      batteryCharges.push(
          JsonValue(100 * d->getBatteryCharge() /
                    static_cast<double>(d->getBatteryCapacity())));
      droneIdx++;
    }
  }
  //
  JsonObject batteryDetails;
  batteryDetails["batteries"] = batteryCharges;
  controller.sendEventToView("UpdateBatteries", batteryDetails);
  //
  for (int id : removed) {  // remove deleted entities from sim
    removeFromSim(id);
  }
  // loop through packages to schedule unplugDrone
  int deliverySize = scheduledDeliveries.size();
  while (deliverySize > 0) {
    // std::cout << "Entering deliveries\n";
    int bestBattery;
    int numAvailable = 0;
    //
    Package* package = scheduledDeliveries.front();
    ChargingStation* endStation =
        getClosestRechargeStation(package->getDestination());
    //
    double packageDist = package->getPosition().dist(package->getDestination());
    // calc distance between package location and destination
    packageDist += package->getPosition().dist(endStation->getPosition());
    // add distance from end position to recharge station
    //
    double bestSpeed = packageDist;
    // set the best speed to a large number so minimum will be smaller
    double bestWaitingSpeed = packageDist * 3;
    // set best waiting speed to large number as well
    double bestTotalDist = packageDist;
    //
    Drone* bestDrone = nullptr;
    //
    for (auto& [id, entity] : entities) {
      Drone* d = dynamic_cast<Drone*>(entity);
      // is the entity an available drone
      if (d && d->getAvailability()) {
        // does the drone have a high enough weight capacity
        if (package->getPackageWeight() < d->getWeight()) {
          // std::cout << "Drone with weight capacity found\n";
          //  calculate the distance between drone and package
          double droneDist = package->getPosition().dist(d->getPosition());
          // std::cout << "droneDist: " << packageDist + droneDist << "\n";
          //  does the battery have enough max capacity
          if (d->getBatteryCapacity() > packageDist + droneDist) {
            // std::cout << "Drone with battery capacity found\n";
            //  calc speed drone can do deliver in
            double travelSpeed = (packageDist + droneDist) / d->getSpeed();
            // does the battery have enough current charge
            if (d->getBatteryCharge() > packageDist + droneDist) {
              // std::cout << "Drone with battery charge found\n";
              //
              if (travelSpeed < bestSpeed) {
                // set best non-waiting speed to current drone's speed
                bestSpeed = travelSpeed;
                // a non-waiting speed is available, ignore all waiting speeds
                bestWaitingSpeed = -1;
                bestDrone = d;
                bestTotalDist = packageDist + droneDist;
              }
              // not enough current charge, enough battery capacity,
              // and no non-waiting speed found
            } else if (bestWaitingSpeed != -1) {
              // std::cout << "Drone with not enough battery charge found\n";
              //
              if (travelSpeed < bestWaitingSpeed) {
                // set the waiting speed to the current drone's speed
                bestWaitingSpeed = travelSpeed;
                bestDrone = d;
                bestTotalDist = packageDist + droneDist;
              }
              //
            }
          }
        }
        numAvailable++;
      }
    }
    //
    // std::cout << "Finished drone finding\n";
    deliverySize--;
    scheduledDeliveries.pop_front();
    if (numAvailable == 0 || !bestDrone) {
      // std::cout << "No suitable drone\n";
      scheduledDeliveries.push_back(package);
      if (numAvailable == 0) break;
      continue;
    }
    //
    // std::cout << "best total: " << bestTotalDist << std::endl;
    // no drones available, stop looking for them
    bestDrone->setNextDelivery(package, bestTotalDist, endStation);
  }
  //
  removed.clear();
}

void SimulationModel::stop(void) { controller.stop(); }

void SimulationModel::removeFromSim(int id) {
  IEntity* entity = entities[id];
  if (entity) {
    for (auto i = scheduledDeliveries.begin(); i != scheduledDeliveries.end();
         ++i) {
      if (*i == entity) {
        scheduledDeliveries.erase(i);
        break;
      }
    }
    controller.removeEntity(*entity);
    entities.erase(id);
    delete entity;
  }
}
