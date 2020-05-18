project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model simulator/main.cpp common/Files.cpp common/ShipPlan.cpp common/Container.cpp common/Floor.cpp common/ShipRoute.cpp common/Ship.cpp common/Ship.h common/Crane.cpp common/Crane.h common/Port.h algorithm/_319088373_a.cpp algorithm/_319088373_a.h common/WeightBalanceCalculator.cpp common/Port.cpp common/AbstractAlgorithm.h algorithm/_319088373_b.cpp algorithm/_319088373_b.h common/AlgorithmRegistration.h simulator/AlgorithmRegistration.cpp simulator/AlgorithmRegistrar.cpp)