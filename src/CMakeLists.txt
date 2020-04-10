project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model main.cpp AlgoRunner.cpp Files.cpp ShipPlan.cpp Container.cpp Floor.cpp ShipRoute.cpp Ship.cpp Ship.h Crane.cpp Crane.h Port.h NaiveAlgo.cpp NaiveAlgo.h Balancer.cpp Port.cpp NaiveAndStupidAlgo.cpp NaiveAndStupidAlgo.h)