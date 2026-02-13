# Graph-Based Pacman Survival Game

A C++ implementation of a Pacman survival game using Qt5 for the UI and graph-based pathfinding algorithms. The game features four distinct CPU-controlled monsters — one using **A\* pathfinding** for optimal pursuit and three using different Greedy algorithms — chasing Pacman through a classic Pacman maze.

## Game Overview

**Objective**: Survive as long as possible while being chased by four intelligent monsters!

- **Grid Size**: Classic 28×31 Pacman maze with tunnel wrapping on row 14
- **Controls**: Arrow keys to move Pacman
- **Goal**: Maximize survival time by evading the monsters
- **Game Over**: When any monster catches Pacman


## Greedy Algorithms

Each monster uses a distinct greedy algorithm to pursue Pacman:

### 1. **A\* Pathfinding (Red Monster - M1)** ⭐ *Upgraded from Greedy*
- **Algorithm**: A\* (A-Star) search with Manhattan distance heuristic
- **Strategy**: Computes the guaranteed shortest path from monster to Pacman using a min-heap priority queue
- **Behavior**: Always takes the optimal route — never gets stuck in U-shaped walls or dead ends
- **Cost Function**: `F = G + H` where G = actual path cost, H = Manhattan distance heuristic
- **Complexity**: O(b^d) with optimal pathfinding, compared to O(1) greedy
- **Data Structure**: Min-heap priority queue + closed set for explored nodes
- **Improvement**: Replaces the previous Euclidean distance greedy strategy that could get trapped by walls

### 2. **Heuristic Greedy (Pink Monster - M2)**
- **Algorithm**: Manhattan distance heuristic
- **Strategy**: Minimizes grid-based distance (sum of axis distances)
- **Behavior**: Better at navigating maze corners, slightly different pathing than M1
- **Formula**: `d = |dx| + |dy|`

### 3. **Directional Greedy (Cyan Monster - M3)**
- **Algorithm**: Major axis prioritization
- **Strategy**: Identifies the axis with larger distance gap and prioritizes closing that gap first
- **Behavior**: More predictable movement patterns, tends to align on one axis before switching
- **Logic**: If `|dx| > |dy|`, prioritize horizontal movement; otherwise prioritize vertical

### 4. **Aggressive Greedy (Orange Monster - M4)**
- **Algorithm**: Predictive positioning
- **Strategy**: Predicts Pacman's future position (2 steps ahead) based on current direction, then moves toward predicted location
- **Behavior**: Anticipates player movement, can cut off escape routes
- **Advanced**: Uses velocity-based prediction for interception

### Algorithm Comparison

| Monster | Color  | Algorithm      | Distance Type | Prediction | Maze Navigation |
|---------|--------|----------------|---------------|------------|-----------------|
| M1      | Red    | **A\* Search** | Manhattan (H) | No         | **Optimal path**|
| M2      | Pink   | Greedy         | Manhattan     | No         | Grid-aligned    |
| M3      | Cyan   | Greedy         | Axis-weighted | No         | Axis-priority   |
| M4      | Orange | Greedy         | Euclidean     | Yes (2-step)| Intercepting   |

## 🔧 Technical Details

### Graph Representation
- **Nodes**: Each walkable cell in the maze is a graph node
- **Edges**: Nodes are connected to their valid neighbors (up, down, left, right)
- **Tunnel**: Row 14 has special tunnel edges connecting left and right sides
- **Data Structure**: `unordered_map<Location, Node*>` for O(1) node lookup

### Movement System
- **Update Rate**: 4 updates per second (250ms interval)
- **Toroidal Wrapping**: Only on tunnel row (row 14), not entire grid
- **Collision Detection**: Position-based, checked every frame
- **Input Handling**: Direction persists until changed (classic Pacman style)

### Object-Oriented Design
- **Entity**: Abstract base class for all movable objects (Pacman, Monsters)
- **Strategy Pattern**: Greedy algorithms implemented as interchangeable strategies
- **Separation of Concerns**: Game logic (Controller) separated from rendering (Widget)

## Requirements

- **C++ Compiler**: Supporting C++17 or later
- **CMake**: Version 3.10 or higher
- **Qt5**: Widgets module
- **OS**: Linux, macOS, or Windows

## Building and Running

### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential cmake qt5-default libqt5widgets5
```

**Fedora:**
```bash
sudo dnf install gcc-c++ cmake qt5-qtbase-devel
```

**macOS (using Homebrew):**
```bash
brew install cmake qt@5
```

### Build Instructions

1. **Clone or navigate to the project directory:**
```bash
cd pacman
```

2. **Create build directory:**
```bash
mkdir build
cd build
```

3. **Generate build files with CMake:**
```bash
cmake ..
```

4. **Compile the project:**
```bash
make
```

5. **Run the game:**
```bash
./PacmanGame
```

### Quick Build (One-liner)
```bash
mkdir -p build && cd build && cmake .. && make && ./PacmanGame
```

## How to Play

1. **Start the game**: The game window will open showing the maze
2. **Begin playing**: Press any arrow key to start the timer and begin movement
3. **Move Pacman**: Use arrow keys (↑ ↓ ← →) to navigate
4. **Survive**: Avoid all four monsters for as long as possible
5. **Game Over**: When caught, your survival time is displayed
6. **Restart**: Press `R` to restart after game over

### Controls
- `↑` - Move Up
- `↓` - Move Down
- `←` - Move Left
- `→` - Move Right
- `R` - Restart game (after game over)

## Visual Design

- **Pacman**: Yellow circle with animated mouth
- **Monsters**: Classic ghost shape with wavy feet
  - Red (M1): A\* Pathfinding (optimal path)
  - Pink (M2): Heuristic Greedy
  - Cyan (M3): Directional Greedy
  - Orange (M4): Aggressive Greedy
- **Walls**: Blue blocks forming the classic Pacman maze
- **Background**: Black
- **Status Bar**: Shows survival time at the top

## Algorithm Justification

### Why Greedy Algorithms?

Greedy algorithms are ideal for this Pacman implementation because:

1. **Real-time Performance**: O(n) complexity where n = number of neighbors (max 4), ensuring smooth gameplay
2. **Believable AI**: Creates engaging, somewhat-predictable monster behavior that feels fair to players
3. **Variety**: Different greedy heuristics create distinct monster personalities
4. **Local Optimization**: Choosing the locally best move at each step without global pathfinding overhead
5. **No Backtracking Needed**: Monsters continuously pursue; they don't need to reconsider past decisions

### Greedy vs. Other Approaches

| Approach | Time Complexity | Monster Behavior | Used? |
|----------|----------------|--|------|
| **A\* Pathfinding** | O(b^d) | Optimal shortest path | ✅ M1 (Red) |
| **Greedy** | O(neighbors) ≈ O(1) | Aggressive local pursuit | ✅ M2, M3, M4 |
| Random | O(1) | Unpredictable | ❌ Too easy |
| Dynamic Programming | O(n²) | Optimal strategy | ❌ Too slow |


## Known Limitations

1. **No pellets/scoring**: Survival time is the only metric (not classic Pacman gameplay)
2. **No power pellets**: Monsters cannot be eaten
3. **Fixed difficulty**: Monsters don't speed up over time
4. **Static maze**: No level progression
5. **Partial toroidal wrapping**: Only tunnel on row 14, not full wraparound
