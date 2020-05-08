project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model main.cpp common/Files.cpp common/ShipPlan.cpp common/Container.cpp common/Floor.cpp common/ShipRoute.cpp common/Ship.cpp common/Ship.h common/Crane.cpp common/Crane.h common/Port.h Algo/NaiveAlgo.cpp Algo/NaiveAlgo.h common/WeightBalanceCalculator.cpp common/Port.cpp common/AbstractAlgorithm.h Algo/NaiveAlgoWithTrick.cpp Algo/NaiveAlgoWithTrick.h)