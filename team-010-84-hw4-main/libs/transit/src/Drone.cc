#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>

#include "AstarStrategy.h"
#include "BeelineStrategy.h"
#include "BfsStrategy.h"
#include "ChargingStation.h"
#include "DfsStrategy.h"
#include "DijkstraStrategy.h"
#include "JumpDecorator.h"
#include "Package.h"
#include "SimulationModel.h"
#include "SpinDecorator.h"

Drone::Drone(JsonObject& obj) : IEntity(obj) {
  available = true;
  batteryCapacity = obj["battery_cap"];
  weightCapacity = obj["weight_cap"];
  batteryCharge = batteryCapacity;
  // std::cout << "Finished constructor\n";
}

Drone::~Drone() {
  if (toPackage) delete toPackage;
  if (toFinalDestination) delete toFinalDestination;
  if (package) delete package;
  if (rechargeStation) delete rechargeStation;
}

void Drone::setNextDelivery(Package* package1, double chargeRequired,
                            ChargingStation* endStation) {
  package = package1;                // store package
  nextChargingStation = endStation;  // store charging station
  rechargeStation =
      new BeelineStrategy(package->getDestination(), endStation->getPosition());
  //
  if (package) {  // package exists
    available = false;
    pickedUp = false;
    //
    this->chargeRequired = chargeRequired;
    //
    Vector3 packagePosition = package->getPosition();
    Vector3 finalDestination = package->getDestination();

    toPackage = new BeelineStrategy(position, packagePosition);

    std::string strat = package->getStrategyName();
    if (strat == "astar") {
      toFinalDestination = new JumpDecorator(new AstarStrategy(
          packagePosition, finalDestination, model->getGraph()));
    } else if (strat == "dfs") {
      toFinalDestination = new SpinDecorator(new JumpDecorator(new DfsStrategy(
          packagePosition, finalDestination, model->getGraph())));
    } else if (strat == "bfs") {
      toFinalDestination = new SpinDecorator(new SpinDecorator(new BfsStrategy(
          packagePosition, finalDestination, model->getGraph())));
    } else if (strat == "dijkstra") {
      toFinalDestination =
          new JumpDecorator(new SpinDecorator(new DijkstraStrategy(
              packagePosition, finalDestination, model->getGraph())));
    } else {
      toFinalDestination =
          new BeelineStrategy(packagePosition, finalDestination);
    }
  }
}

void Drone::update(double dt) {
  // std::cout << "Entering update loop\n";
  if (starting && model) {
    nextChargingStation = model->getClosestRechargeStation(getPosition());
    rechargeStation =
        new BeelineStrategy(position, nextChargingStation->getPosition());

    starting = false;
    // std::cout << "This thing done succesfully\n";
  }
  // currently moving to package
  if (toPackage && batteryCharge > chargeRequired) {
    toPackage->move(this, dt);
    batteryCharge -= (speed * dt);
    // charge has already been guaranteed, and can be disregarded moving forward
    if (chargeRequired != 0) {
      nextChargingStation->unplugDrone(this);
      chargeRequired = 0.0;
    }
    if (toPackage->isCompleted()) {
      delete toPackage;
      toPackage = nullptr;
      pickedUp = true;
    }
  } else if (toFinalDestination) {  // currently moving to final destination
    toFinalDestination->move(this, dt);
    batteryCharge -= (speed * dt);
    if (package && pickedUp) {
      package->setPosition(position);
      package->setDirection(direction);
    }
    if (toFinalDestination->isCompleted()) {  // reached final destination
      delete toFinalDestination;
      toFinalDestination = nullptr;
      package->handOff();
      package = nullptr;
      pickedUp = false;
    }
  } else if (rechargeStation) {
    rechargeStation->move(this, dt);
    batteryCharge -= (speed * dt);
    if (rechargeStation->isCompleted()) {
      // std::cout << "path completed\n";
      available = true;
      delete rechargeStation;
      rechargeStation = nullptr;
      nextChargingStation->queueUp(this);
      // std::cout << "completed\n";
    }
  }
  // std::cout << "Battery charge: " << batteryCharge << "\n";
}

void Drone::recharge(const double amount) { batteryCharge += amount; }

bool Drone::getAvailability() { return available; }

int Drone::getWeight() { return weightCapacity; }

int Drone::getBatteryCapacity() { return batteryCapacity; }

double Drone::getBatteryCharge() { return batteryCharge; }
