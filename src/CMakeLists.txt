project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model Algo1/main.cpp Algo1/Files.cpp Algo1/ShipPlan.cpp Algo1/Container.cpp Algo1/Floor.cpp Algo1/ShipRoute.cpp Algo1/Ship.cpp Algo1/Ship.h Algo1/Crane.cpp Algo1/Crane.h Algo1/Port.h Algo1/NaiveAlgo.cpp Algo1/NaiveAlgo.h Algo1/WeightBalanceCalculator.cpp Algo1/Port.cpp Algo1/AbstractAlgorithm.h)