#include "Package.h"

#include "Robot.h"

Package::Package(JsonObject &obj) : IEntity(obj) {
}

Vector3 Package::getDestination() const {
  return destination;
}

std::string Package::getStrategyName() const {
  return strategyName;
}

void Package::setStrategyName(std::string strategyName_) {
  strategyName = strategyName_;
}

int Package::getPackageWeight() const {
  return weight;
}

void Package::setPackageWeight(int packageWeight) {
  weight = packageWeight;
}

void Package::update(double dt) {}

void Package::initDelivery(Robot* owner) {
  this->owner = owner;
  owner->requestedDelivery = false;
  requiresDelivery = false;
  destination = owner->getPosition();
}

void Package::handOff() {
  if (owner) {
    owner->receive(this);
  }
}
