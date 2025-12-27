cmake_minimum_required(VERSION 3.10)
project(PacmanGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

set(SOURCES
    src/AggressiveGreedyStrategy.cpp
    src/DirectionalGreedyStrategy.cpp
    src/DistanceGreedyStrategy.cpp
    src/Entity.cpp
    src/GameController.cpp
    src/GameWidget.cpp
    src/Graph.cpp
    src/HeuristicGreedyStrategy.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/Monster.cpp
    src/Node.cpp
    src/Pacman.cpp
)

set(HEADERS
    include/AggressiveGreedyStrategy.h
    include/DirectionalGreedyStrategy.h
    include/DistanceGreedyStrategy.h
    include/Entity.h
    include/GameController.h
    include/GameWidget.h
    include/Graph.h
    include/GreedyStrategy.h
    include/HeuristicGreedyStrategy.h
    include/Location.h
    include/MainWindow.h
    include/Monster.h
    include/Node.h
    include/Pacman.h
)

add_executable(PacmanGame ${SOURCES} ${HEADERS})

target_link_libraries(PacmanGame Qt5::Widgets)

target_include_directories(PacmanGame PRIVATE include)