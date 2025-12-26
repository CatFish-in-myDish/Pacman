# 🎮 Graph-Based Pacman Survival Game (Review 1)

## Project Title
**Graph-Based Pacman Survival Game with Multiple Greedy AI Monsters**

---

## 1. Objective
Design and implement a **Java-based Pacman-style game** using **Graph data structures**, where the **human player controls Pacman** and **four CPU-controlled monsters** attempt to catch Pacman using **four different Greedy algorithm strategies**.

The objective of **Review 1** is to:
- Build a **working playable prototype (POC)**
- Model the game using **graphs**
- Implement **Greedy algorithms** for CPU gameplay
- Explain **class design, graph modeling, and greedy logic**

Future paradigms such as **Divide & Conquer, Dynamic Programming, and Backtracking** must *not* be implemented now, but the design should be **extensible** for future reviews.

---

## 2. Game Description

### Gameplay
- The game is played on a **50 × 50 grid**.
- The grid follows **Snake-style wrap-around mechanics**:
  - Moving left from column `0` moves to column `49`
  - Moving right from column `49` moves to column `0`
  - Moving up from row `0` moves to row `49`
  - Moving down from row `49` moves to row `0`
- This creates a **toroidal (wrap-around) undirected graph**.

### Roles
- **Pacman (Human Player)**
  - Controlled using **keyboard arrow keys**
  - Objective: **survive as long as possible**

- **Monsters (CPU – 4 total)**
  - Each monster uses a **different Greedy strategy**
  - Objective: **catch Pacman**

### End Condition
- The game is **time-based survival**
- The game ends when **any monster reaches Pacman**
- Survival time is displayed as the score

---

## 3. Graph Representation
- Each grid cell represents a **node**
- Each node connects to:
  - Up, Down, Left, Right neighbors
  - Wrap-around neighbors (toroidal edges)
- Graph Type:
  - **Undirected**
  - **Unweighted (unit cost edges)**

### Implementation Notes
- Use an **Adjacency List** representation
- Clearly explain:
  - Grid-to-graph mapping
  - Wrap-around edge creation
  - Why this graph is suitable for pathfinding

---

## 4. Greedy Algorithm Design (Core Requirement)

Implement **four distinct Greedy algorithms**, one for each monster.

Each monster:
- Considers **only the current state**
- Chooses the **locally optimal next move**
- Does **not** use memory or future planning

### Greedy Strategies
1. **Monster 1 – Pure Distance Greedy**
   - Chooses the neighboring node with minimum distance to Pacman

2. **Monster 2 – Heuristic Greedy**
   - Uses Manhattan distance as a heuristic to approach Pacman

3. **Monster 3 – Directional Greedy**
   - Prioritizes reducing row or column difference first

4. **Monster 4 – Aggressive Greedy**
   - Predicts Pacman’s next likely move and intercepts greedily

For each strategy:
- Explain the logic
- Explain why it is greedy
- Provide high-level time complexity

---

## 5. GUI Requirements
- Use **Qt5**
- No images required
- Simple visual representation:
  - `P` → Pacman
  - `M1, M2, M3, M4` → Monsters
- Focus on:
  - Playability
  - Clarity
  - Correct movement logic

---

## 6. Software Design & Class Structure

Use **Object-Oriented Design** principles.

### Suggested Classes
- `GameController` – Manages game loop and state
- `Graph` – Stores nodes and edges
- `Node` – Represents a grid cell
- `Pacman` – Player logic and movement
- `Monster` – Base monster class
- `GreedyStrategy` – Interface / abstract class
- `GreedyStrategy1/2/3/4` – Concrete greedy implementations
- `GameUI` – GUI rendering and keyboard input

Explain:
- Responsibility of each class
- Interaction between classes
- Why the design supports future algorithm extensions

---

## 7. Why Greedy Works Here
Include an academic justification:
- Monsters require **real-time decision making**
- Greedy algorithms:
  - Are computationally efficient
  - Provide fast local decisions
  - Work well for pursuit problems
- Explain why greedy is suitable for dynamic chasing scenarios

---

## 8. Review 1 Evaluation Alignment

Ensure the implementation satisfies:
- ✔ Choice of Game & Model
- ✔ Working POC
- ✔ Graph Representation
- ✔ Greedy Algorithm Implementation
- ✔ Team Project Readiness (modular design)

---

## 9. Constraints
- Do **NOT** implement:
  - Dynamic Programming
  - Divide & Conquer
  - Backtracking
- Clearly mention that:
  - The architecture is **future-ready** for these paradigms

---

## 10. Output Expectations
Provide:
- Complete Java source code
- Algorithm explanations
- Class design explanation
- Instructions to compile and run the game

---

## 11. C++ Implementation with Qt5

This project has been implemented in **C++ with Qt5** for a native desktop experience. The C++ version maintains the same architecture and game logic as the Java version.

### Compilation and Run Instructions (C++)

#### Prerequisites
- C++ compiler with C++17 support (GCC 7+ or Clang 5+)
- CMake 3.10 or higher
- Qt5 development libraries
  - Ubuntu/Debian: `sudo apt-get install qt5-default qtbase5-dev`
  - Fedora: `sudo dnf install qt5-qtbase-devel`
  - macOS: `brew install qt@5`

#### Build Steps
```bash
# Navigate to project directory
cd /path/to/project

# Create build directory
mkdir build
cd build

# Generate build files with CMake
cmake ..

# Compile the project
make

# Run the game
./PacmanGame
```

### C++ Implementation Details
- **Source Directory**: `cpp-src/`
- **Build System**: CMake
- **C++ Standard**: C++17
- **UI Framework**: Qt5 Widgets
- **Memory Management**: RAII with proper destructors and smart pointer patterns

### Architecture Differences from Java
- Uses standard C++ containers (`std::vector`, `std::unordered_map`)
- Qt5 `QWidget` and `QPainter` for graphics (instead of JavaFX Canvas)
- `QTimer` for game loop (instead of AnimationTimer)
- Manual memory management with proper cleanup in destructors
- Strategy pattern with raw pointers (owned by Monster class)

---
