#ifndef PACKAGE_H
#define PACKAGE_H

#include <vector>

#include "IEntity.h"
#include "math/vector3.h"
#include "util/json.h"

class Robot;

class Package : public IEntity {
 public:
  /**
   * @brief Constructor
   * @param obj JSON object containing the package's information
   */
  Package(JsonObject& obj);

  /**
   * @brief Gets the Package's destination
   * @return The Package's destination
   */
  Vector3 getDestination() const;

  /**
   * @brief Returns the name of the strategy for this package
   * 
   * @returns String name of strategy
  */
  std::string getStrategyName() const;

  /**
   * @brief Set the Strategy Name
   *
   * @param strategyName_ Strategy name
   */
  void setStrategyName(std::string strategyName_);


  /**
   * @brief Returns the weight of this package
   * 
   * @returns int weight of package
  */
  int getPackageWeight() const;

  /**
   * @brief Set the package weight
   *
   * @param strategyName_ Package weight
   */
  void setPackageWeight(int packageWeight);

  /**
   * @brief Updates the Package
   * 
   * @param dt difference in time since last update
  */
  void update(double dt);

  /**
   * @brief Sets the attributes for delivery
   * 
   * @param owner Robot for the package to be delivered to
  */
  void initDelivery(Robot* owner);

  /**
   * @brief Gives the robot/owner this package
  */
  void handOff();

  /**
   * @brief get the closest recharge station to the package
   * 
   */
  IEntity* getClosestRechargeStation();

  bool requiresDelivery = true;

 private:
  Vector3 destination;
  std::string strategyName;
  Robot* owner = nullptr;
  int weight = 5;
};

#endif  // PACKAGE_H
