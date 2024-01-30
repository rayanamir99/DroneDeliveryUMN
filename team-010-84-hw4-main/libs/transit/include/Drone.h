#ifndef DRONE_H_
#define DRONE_H_

#include <vector>

#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"

class Package;
class ChargingStation;

// Represents a drone in a physical system.
// Drones move using euler integration based on a specified
// velocity and direction.
/**
 * @class Drone
 * @brief Represents a drone in a physical system. Drones move using euler
 * integration based on a specified velocity and direction.
 */
class Drone : public IEntity {
 public:
  /**
   * @brief Drones are created with a name
   * @param obj JSON object containing the drone's information
   */
  Drone(JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~Drone();

  /**
   * @brief Sets the next package to be delivered
   *
   * @param package1 next package to be delivered
   * @param chargeRequired charge needed to get from current
   * location to package to nearest recharge station
   * @param endStation nearest recharge station to destination
   */
  void setNextDelivery(Package* package1,
                       double chargeRequired,
                       ChargingStation* endStation);

  /**
   * @brief Updates the drone's position
   * @param dt Delta time
   */
  void update(double dt);

  /**
   * @brief Removing the copy constructor operator
   * so that drones cannot be copied.
   *
   * @param drone object that cannot be copied
   */
  Drone(const Drone& drone) = delete;

  /**
   * @brief Removing the assignment operator
   * so that drones cannot be copied.
   *
   * @param drone object that cannot be copied
   */
  Drone& operator=(const Drone& drone) = delete;

  /**
   * @brief Recharge the battery by the given amount
   * @param amount the amount to charge the battery by
   */
  void recharge(const double amount);

  /**
   * @brief Indicates whether the drone is available or not
   * @return boolean value indicating the availability of the drone
   */
  bool getAvailability();


  /**
   * @brief Indicates the drone weight capacity
   * @return integer value representing the weight of the drone
   */
  int getWeight();

  /**
   * @brief Indicates the drone current charge
   * @return integer value representing the current battery charge of the drone
   */
  double getBatteryCharge();

  /**
   * @brief Indicates the drone battery capacity
   * @return integer value representing the maximum battery capacity of the drone
   */
  int getBatteryCapacity();

 private:
  bool available = false;
  bool pickedUp = false;
  Package* package = nullptr;
  IStrategy* toPackage = nullptr;
  IStrategy* toFinalDestination = nullptr;
  IStrategy* rechargeStation = nullptr;
  ChargingStation* nextChargingStation = nullptr;
  double batteryCharge = 0;
  int weightCapacity = 0;
  int batteryCapacity = 0;
  double chargeRequired = 0.0;  // charger required to start moving to package
  bool starting = true;
};

#endif
