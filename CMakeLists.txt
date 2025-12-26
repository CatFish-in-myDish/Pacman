cmake_minimum_required(VERSION 3.10)
project(PacmanGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Enable automoc for Qt meta-object compilation
set(CMAKE_AUTOMOC ON)

# Source files
set(SOURCES
    cpp-src/main.cpp
    cpp-src/Location.cpp
    cpp-src/Node.cpp
    cpp-src/Graph.cpp
    cpp-src/Entity.cpp
    cpp-src/Pacman.cpp
    cpp-src/Monster.cpp
    cpp-src/DistanceGreedyStrategy.cpp
    cpp-src/HeuristicGreedyStrategy.cpp
    cpp-src/DirectionalGreedyStrategy.cpp
    cpp-src/AggressiveGreedyStrategy.cpp
    cpp-src/GameController.cpp
    cpp-src/GameWidget.cpp
    cpp-src/MainWindow.cpp
)

# Header files
set(HEADERS
    cpp-src/Location.h
    cpp-src/Node.h
    cpp-src/Graph.h
    cpp-src/Entity.h
    cpp-src/Pacman.h
    cpp-src/Monster.h
    cpp-src/GreedyStrategy.h
    cpp-src/DistanceGreedyStrategy.h
    cpp-src/HeuristicGreedyStrategy.h
    cpp-src/DirectionalGreedyStrategy.h
    cpp-src/AggressiveGreedyStrategy.h
    cpp-src/GameController.h
    cpp-src/GameWidget.h
    cpp-src/MainWindow.h
)

# Create executable
add_executable(PacmanGame ${SOURCES} ${HEADERS})

# Link Qt5
target_link_libraries(PacmanGame Qt5::Widgets)

# Include directories
target_include_directories(PacmanGame PRIVATE cpp-src)
