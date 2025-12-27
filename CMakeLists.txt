cmake_minimum_required(VERSION 3.10)
project(PacmanGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

set(SOURCES
    src/main.cpp
    src/cpp/Location.cpp
    src/cpp/Node.cpp
    src/cpp/Graph.cpp
    src/cpp/Entity.cpp
    src/cpp/Pacman.cpp
    src/cpp/Monster.cpp
    src/cpp/DistanceGreedyStrategy.cpp
    src/cpp/HeuristicGreedyStrategy.cpp
    src/cpp/DirectionalGreedyStrategy.cpp
    src/cpp/AggressiveGreedyStrategy.cpp
    src/cpp/GameController.cpp
    src/cpp/GameWidget.cpp
    src/cpp/MainWindow.cpp
)

set(HEADERS
    src/include/Location.h
    src/include/Node.h
    src/include/Graph.h
    src/include/Entity.h
    src/include/Pacman.h
    src/include/Monster.h
    src/include/GreedyStrategy.h
    src/include/DistanceGreedyStrategy.h
    src/include/HeuristicGreedyStrategy.h
    src/include/DirectionalGreedyStrategy.h
    src/include/AggressiveGreedyStrategy.h
    src/include/GameController.h
    src/include/GameWidget.h
    src/include/MainWindow.h
)

add_executable(PacmanGame ${SOURCES} ${HEADERS})

target_link_libraries(PacmanGame Qt5::Widgets)

# Include directories
target_include_directories(PacmanGame PRIVATE src)
