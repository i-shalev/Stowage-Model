project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model main.cpp tests.cpp ShipPlan.cpp Container.cpp Floor.cpp ShipRoute.cpp Ship.cpp Ship.h)