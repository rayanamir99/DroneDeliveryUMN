# How to run in docker

## 1. Make sure docker is running

Open Docker Desktop.

## 2. Build the docker image

Run `docker build -t 3rix40/drone_sim_hw4 .`

You can also pull the current image to speed up
the process using `docker pull`.

## 3. Run the Code

Use `docker run --rm -it -p 8081:8081 3rix40/drone_sim_hw4`

## 4. Push your image

At the end, you can push your image to speed things up for others.
Run `docker push 3rix40/drone_sim_hw4 .`

# How To run in Ubuntu

Run `make -j`

Then run `./build/bin/transit_service 8081 apps/transit_service/web/`

# The write up

Team-010-84

Rayan Amir, amir0045
Eric Patton, patto164
Artemis Pearson, pear0836

## What is the project about?
This project depicts a package delivery simulation which spans the entire UMN campus. Users are in charge of choosing a package pickup and dropoff location as well as the weight of the package and the desired path strategy they want the drone to follow. Drones will be selected for deliveries based on their attributes and battery capacities and will return to recharge stations after their designated package deliveries since their batteries deplete over time.

## How to run the simulation?
To run the simulation, you’d first need to run the make -j command to compile the project within the terminal. After this, running the ./build/bin/transit_service 8081 apps/transit_service/web/ command will allow you to host the project locally in port 8081.

## What does the simulation do specifically?
Once the project is hosted locally, the user will be prompted to schedule deliveries spanning the UMN-Twin Cities campus. The user will have to choose a pickup and dropoff location as well as a name for the delivery. The user will also be prompted to choose the path strategy that the drone is expected to follow when delivering the package. When traveling to the package, the drone will always follow the beeline strategy path. Along with this, the user will also designate a weight to the package which indicates which type of drone should be in charge of the delivery process. Packages can only be picked up by drones that meet the minimum weight requirement. Speed is the number 1 priority in the package delivery process, so lighter drones are prioritized for packages that they are able to pick up. After the drone beelines to the package and utilizes the indicated path strategy to deliver the package, it will do a celebration which depends on the path strategy utilized and then beeline to the nearest recharge station as the battery of the drone depletes over time. Once arrived at the recharge station, the drone will join the recharge queue as only three drones can be charging at the same time. The drones at the recharge station are ready for their next package pickup. 

## New Feature 1: Drone Attributes

### What does it do?
Our first new feature is the drone attributes feature. When scheduling a package delivery, the user is prompted to choose a weight for their package which determines which drone picks it up. There are three different types of drones, light, medium, and heavy. Light drones are in charge of delivering packages of at most 10 Lb’s, have a speed of 40, and a battery capacity of 2000 units. Medium drones are in charge of delivering packages of at most 20 Lb’s, have a speed of 30, and a battery capacity of 3000 units. Heavy drones are able to deliver packages of all weights, have a speed of 20, and a battery capacity of 10000. If multiple drones fit the requirements of a package pickup, pick the drone with the fastest delivery time. If no drone is available to pick up a package of associated weight, we wait to schedule the package and move onto the next one. 

### Why is it significantly interesting?
In the real world, different forms of transportation are utilized to deliver packages with different characteristics and we wanted to replicate this process as closely as we could. A singular entity in charge of delivering all packages simply is not accurate to how the real world works and we wanted to make our simulation as realistic as possible. When paired with the battery extension, the attribute extension becomes even more interesting because regardless of the type of drone, drones must have the battery capacity to pick up a package.

### How does it add to the existing work?
In the given code drones are assigned to packages based on when they became available regardless of their proximity to the package. Our code makes this concept more realistic because our drones are more specialized and speed is the number one priority in a delivery. Especially since package delivery systems are becoming more and more optimized by the day in the real world, we felt that this was an important concept to mirror in our simulation

### Which design pattern did you choose to implement and why?
We used the STATE design pattern to incorporate the different drone types and attributes. What this means is that whenever a package delivery is initiated, drones of different types are chosen based on the weight of the package and battery capacity of the drone. The behavior of the drone changes as the internal state changes since the weight, battery capacity, and speed of the drone all change which affects the drone's behavior. We felt that this design pattern allowed us to utilize the base code of the Drone class most effectively while also allowing us to change the behavior of the drone based on the internal state. 

### Instruction to use new feature
Whenever the user schedules a package delivery, the drone attribute extension is utilized because only certain drones can pick up certain packages. Although the feature is not directly user interactable, when observing the simulation, users can clearly see it at work as speed is a much higher priority than before.


## New feature 2: Battery Extension

### What does it do?
Our second new feature is the battery extension. Each drone is given a different battery capacity depending on the type of drone and this plays a large role in what package it delivers. This is because drones can only deliver a package if they have enough charge to pick up and drop off the package, while also having charge leftover to travel to a recharge station after the delivery. The charge of the drone depletes as the drone traverses throughout the map and drones must go to recharge stations to recharge their batteries. Recharge stations only have space for three drones at a time and the rest of the drones are added to a queue waiting to be charged. Before each trip drones will check if they have the battery capacity to complete the trip and will only be able to recharge before and after trips.

### Why is it significantly interesting?
There is nothing in the real world that is able to run indefinitely without a source of power and we wanted to replicate that in our simulation. Along with this, we also made the battery capacities of the drones directly proportional to the weight capacities and inversely proportional to the speed to model the fact that larger objects take more power to run but are also slower. Along with that, the addition of the recharge station means that drones not only have to worry about completing the trip, but also must ensure that they have enough charge to reach the nearest recharge station. 

### How does it add to the existing work?
In the given code drones are able to deliver packages for an infinite amount of time without needing to fuel up which is unrealistic. By incorporating the battery extension and recharge station we make the simulation model more realistic. Additionally, speed being the first priority creates an interesting situation because in many situations the fastest drone is likely not the drone chosen because of the smaller battery capacity. 

### Which design pattern did you choose to implement and why?
We used the STATE design pattern to incorporate the battery itself and the composite factory to incorporate the recharge stations. What this means is that whenever the battery member variable of the drone changes the behavior of the drone as drones must have the battery charge to pick up packages. The recharge station is implemented using the composite factory which allows us to charge multiple drones at once without having to worry about the drones themselves.

### Instruction to use new feature
Whenever the user schedules a package delivery, the battery extension is utilized because drones must have the battery capacity to pick up and deliver the package, while ensuring they have enough charge to travel to a charging station. Although the feature is not directly user interactable, when observing the simulation, users can clearly see it at work as drones are chosen based on battery capacity and return to charging stations after they’ve completed their delivery.


## Sprint Retrospective

Looking back on the sprint, we feel that we could have done a better job of starting early to make sure that bug fixes were addressed early on in the development process. We also feel that we could have gone more in depth in our Jira as this would have better set us up for success during the actual development process because we would have had a more effective roadmap. I feel that as a team we were able to work together and distribute work evenly among our 3 active members which is something that I am proud of looking back on the project. 

## [Docker link](https://hub.docker.com/repository/docker/3rix40/drone_sim_hw4/general)

https://hub.docker.com/repository/docker/3rix40/drone_sim_hw4/general

## [Video presentation Link](https://youtu.be/mGB8Ec27aRk)

https://youtu.be/mGB8Ec27aRk

## UML Diagram
![Click to view](./hw4_uml.pdf)