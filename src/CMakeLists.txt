project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model Simulation/main.cpp Common/Files.cpp Common/ShipPlan.cpp Common/Container.cpp Common/Floor.cpp Common/ShipRoute.cpp Common/Ship.cpp Common/Ship.h Common/Crane.cpp Common/Crane.h Common/Port.h Algo/NaiveAlgo.cpp Algo/NaiveAlgo.h Common/WeightBalanceCalculator.cpp Common/Port.cpp Common/AbstractAlgorithm.h Algo/NaiveAlgoWithTrick.cpp Algo/NaiveAlgoWithTrick.h Common/AlgorithmRegistration.h Simulation/AlgorithmRegistration.cpp Common/AlgorithmRegistrar.h)