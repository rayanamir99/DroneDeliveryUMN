#ifndef CHARGING_STATION_H_
#define CHARGING_STATION_H_


#include <vector>
#include <queue>


#include "Drone.h"

/**
 * @class ChargingStation 
 * @brief In charge of recharging the battery
 * of the drones
 **/
class ChargingStation : public IEntity {
 public:
    /**
     * @brief Charging station constructor
     *
     * @param obj Utilized in IEntitiy constructor
     **/
    ChargingStation(JsonObject& obj);


    /**
     * @brief Charging Station Destructor
     */
    ~ChargingStation();


    /**
     * @brief If there is space at the charging station, decrements drone queue
     * and adds drone to charging station
     *
     * @param dt delta time
     **/
    void update(double dt);


    /**
     * @brief Removing the copy constructor operator
     * so that charging stations cannot be copied.
     *
     * @param chargingStation object that cannot be copied
     */
    ChargingStation(const ChargingStation& chargingStation) = delete;


    /**
     * @brief Removing the assignment operator
     * so that charging stations cannot be copied.
     *
     * @param chargingStation object that cannot be copied
     */
    ChargingStation& operator=(const ChargingStation& chargingStation) = delete;


    /**
     * @brief Once drone arrives at charging station, queueUp is used
     * to add drone to queue of drones to be charged
     *
     * @param drone Drone to be charged
     **/
    void queueUp(Drone* drone);


    /**
     * @brief Function that recharges drone
     **/
    void rechargeDrone(double dt);


    /**
     * @brief Unplugs drone once charging is completed
     *
     * @param drone Drone to be unplugged
     **/
    void unplugDrone(Drone* drone);


 private:
    int slotsAvailible;
    int chargeRate;
    std::queue<Drone*> droneQueue;
    std::vector<Drone*> droneCurCharge;
};


#endif
