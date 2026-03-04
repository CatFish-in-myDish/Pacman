cmake_minimum_required(VERSION 3.10)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

set(SOURCES
    src/AggressiveGreedyStrategy.cpp
    src/AStarStrategy.cpp
    src/BacktrackStrategy.cpp
    src/DirectionalGreedyStrategy.cpp
    src/DistanceGreedyStrategy.cpp
    src/Entity.cpp
    src/GameController.cpp
    src/GameWidget.cpp
    src/GhostTerritory.cpp
    src/Graph.cpp
    src/HeuristicGreedyStrategy.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/Monster.cpp
    src/Node.cpp
    src/Pacman.cpp
    src/PinchGreedyStrategy.cpp
    src/ScatterStrategy.cpp
    src/RandomStrategy.cpp
)

set(HEADERS
    include/AggressiveGreedyStrategy.h
    include/AStarStrategy.h
    include/BacktrackStrategy.h
    include/DirectionalGreedyStrategy.h
    include/DistanceGreedyStrategy.h
    include/Entity.h
    include/GameController.h
    include/GameWidget.h
    include/GhostTerritory.h
    include/Graph.h
    include/GreedyStrategy.h
    include/HeuristicGreedyStrategy.h
    include/Location.h
    include/MainWindow.h
    include/Monster.h
    include/Node.h
    include/Pacman.h
    include/PinchGreedyStrategy.h
    include/ScatterStrategy.h
    include/RandomStrategy.h
)

add_executable(Pacman ${SOURCES} ${HEADERS})

target_link_libraries(Pacman Qt5::Widgets)

target_include_directories(Pacman PRIVATE include)
