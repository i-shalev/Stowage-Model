project(Stowage-Model)

set(CMAKE_CXX_STANDARD 14)

add_executable(Stowage-Model main.cpp Files.cpp ShipPlan.cpp Container.cpp Floor.cpp ShipRoute.cpp Ship.cpp Ship.h Crane.cpp Crane.h Port.h Move.cpp Move.h)