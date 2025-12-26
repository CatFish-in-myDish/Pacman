# Graph-Based Pacman Survival Game - Project Report

## 1. Project Overview
This project implements a **graph-based Pacman survival game** in Java. The game takes place on a **50x50 toroidal grid**, modeled as an undirected graph. The player controls Pacman to survive against 4 CPU monsters, each employing a unique **Greedy Algorithm**.

---

## 2. Compilation and Run Instructions

### Prerequisites
- Java Development Kit (JDK) 8 or higher.
- JavaFX SDK (if not bundled with your JDK).

### Steps
1. Navigate to the source directory:
   ```bash
   cd src
   ```

2. Compile the Java files:
   ```bash
   javac *.java
   ```

3. Run the Game:
   ```bash
   java GameUI
   ```
   *Note: If JavaFX is not in your classpath by default, you may need to add module flags, e.g.:*
   ```bash
   java --module-path /path/to/javafx/lib --add-modules javafx.controls *.java
   ```

---

## 3. Class Structure & Design

The project follows a modular **Object-Oriented Design**:

### Core Data Structures
- **`Location`**: Immutable class representing `(x, y)` coordinates.
- **`Node`**: Represents a cell in the grid. Contains a `Location` and a list of neighbor `Node`s.
- **`Graph`**: Initializes the 50x50 grid and connects nodes. Handles **Toroidal Wrap-around** (edges connecting border nodes to opposite sides).

### Game Entities
- **`Entity`** (Abstract): Base class for moving objects. Stores `location` and `lastDirection`.
- **`Pacman`**: Child of Entity. Movement is controlled by the user (GameController inputs).
- **`Monster`**: Child of Entity. Uses a `GreedyStrategy` to determine its next move.

### Strategy Pattern (Algorithms)
- **`GreedyStrategy`** (Interface): Defines `findNextMove(Graph, Link, Entity)`.
- **`DistanceGreedyStrategy`**: Minimizes Euclidean distance.
- **`HeuristicGreedyStrategy`**: Minimizes Manhattan distance.
- **`DirectionalGreedyStrategy`**: Prioritizes reducing row/column differences based on primary axis.
- **`AggressiveGreedyStrategy`**: Predicts Pacman's future position and targets it.

### Game Logic & UI
- **`GameController`**: Manages the game loop, entity states, collisions, and time tracking.
- **`GameUI`**: JavaFX application that renders the game state to a Canvas and handles keyboard input.

---

## 4. Graph Representation
- **Grid Mapping**: Each cell `(row, col)` corresponds to a **Node**.
- **Connectivity**: Each Node serves adjacent neighbors (Up, Down, Left, Right).
- **Wrap-Around**:
  - `(0, y)` connects to `(49, y)`
  - `(x, 0)` connects to `(x, 49)`
  This creates a regular graph where every node has exactly degree 4.

---

## 5. Greedy Algorithms Explained

### Monster 1: Pure Distance Greedy
- **Logic**: Calculates exact Euclidean distance ($\sqrt{\Delta x^2 + \Delta y^2}$) from neighbors to Pacman.
- **Why Greedy**: Locally optimal choice (shortest immediate step) without looking n-steps ahead.

### Monster 2: Heuristic Greedy
- **Logic**: Uses Manhattan distance ($|\Delta x| + |\Delta y|$).
- **Why Greedy**: Standard heuristic often used in A* pathfinding, but here used greedily for immediate move.

### Monster 3: Directional Greedy
- **Logic**: Determines the "primary axis" of separation (e.g., if X distance is larger, prioritize moving horizontally).
- **Why Greedy**: Tries to "close the gap" on the widest dimension first.

### Monster 4: Aggressive Greedy
- **Logic**: LOOKS AHEAD. Predicts Pacman's position 2 steps in the future based on Pacman's current direction. Targets that predicted spot.
- **Why Greedy**: Greedily minimizes distance to the *predicted* location.

---

## 6. Justification for Greedy Approach
Monsters in a real-time game require **fast decision making**.
- **Efficiency**: $O(1)$ decision time per monster (since neighbors are fixed at 4).
- **Responsiveness**: Immediate reaction to Pacman's moves.
- **Pursuit**: In open grids, greedy pursuit is often sufficient to corner a player, especially with multiple chasers using different heuristics (some cut off paths, some chase directly).

---

## 7. Future Extensibility
- The `GreedyStrategy` interface allows easy addition of **backtracking** (DFS) or **shortest-path** (BFS/Dijkstra) algorithms in the future without changing the `Monster` class.
- The `Graph` class structure supports weighted edges if needed later.
