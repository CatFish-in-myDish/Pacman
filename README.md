<p align="center">
  <h1 align="center">Graph-Based Pacman Survival Game</h1>
  <p align="center">
    <strong>A C++ implementation of a Pacman survival game using Qt5 for the UI and graph-based pathfinding algorithms and computational geometry</strong>
  </p>
</p>

## Feature Highlights

- **5 Distinct AI Strategies** — A\* Pathfinding, Manhattan Heuristic, Axis-Weighted Directional, Predictive Pursuit, and Euclidean Greedy
- **Chain Lightning Ability** — Spend score to slow ghosts using a closest-pair divide & conquer algorithm
- **Ghost Territory System** — QuickHull convex hull detects encirclement and dynamically adjusts speeds
- **Dynamic Role Assignment** — Ghosts switch between Chase, Ambush, and Scatter modes every tick
- **Multi-Round Progression** — Clear all pellets to advance rounds with cumulative scoring
- **Classic 28×31 Maze** — Pacman layout with tunnel wrapping

## Game Overview

### Objective

Eat **every pellet** on the 28×31 maze while avoiding four ghosts, each driven by a unique AI algorithm. Use the **Chain Lightning** ability strategically to slow down pursuing ghosts and survive long enough to clear the board.

### How to Play

1. **Launch** the game — you'll see the maze with pellets on every walkable tile
2. **Press any arrow key** to start — Pacman begins moving and ghosts activate
3. **Eat pellets** (+10 points each) to build your score
4. **Avoid ghosts** — contact with any ghost ends the game
5. **Use Chain Lightning** (press `Z`) when your score ≥ 100 to slow the two closest ghosts
6. **Clear all pellets** to win the round and advance to the next

### Controls

| Key | Action |
|-----|--------|
| `↑` `↓` `←` `→` | Move Pacman (also starts the game on first press) |
| `Z` | Chain Lightning — costs 100 score, slows 2 closest ghosts |
| `R` | Restart — full reset after Game Over, next round after Win |

### Game States

| State | Trigger | Display |
|-------|---------|---------|
| **Playing** | First arrow key press | Active maze with HUD |
| **You Win!** | All pellets eaten | Green overlay (48pt), auto-advances in 2s |
| **Game Over** | Ghost catches Pacman | Red overlay (48pt), press `R` to restart |

## Ghost Algorithm Deep Dive

Each of the four ghosts employs a **unique pathfinding strategy**, creating diverse and challenging pursuit patterns. Ghost behavior is further modified by a **dynamic role assignment system** that re-evaluates every game tick.

### The Four Ghosts

| Ghost | Color | Strategy | Personality |
|-------|-------|----------|-------------|
| **M1** | Red | A\* Pathfinding | Optimal pursuer — always finds the shortest path |
| **M2** | Pink | Heuristic Greedy | Manhattan-guided — efficient axis-aligned chaser |
| **M3** | Cyan | Directional Greedy | Axis-dominant — predictable but fast corridor hunter |
| **M4** | Orange | Aggressive Greedy | Predictive — targets where Pacman *will* be |

### Strategy Breakdown

#### A\* Pathfinding (Red Ghost)

The gold standard of pathfinding. Guarantees the **shortest path** on every move.

```cpp
// Min-heap priority queue with F = G + H cost function
// Manhattan distance heuristic (admissible for 4-connected grid)
Location AStarStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Uses `std::priority_queue` with `std::greater` (min-heap)
- Accounts for **toroidal wrapping** in heuristic calculation
- Maintains `gCost` map, `parent` map, and `closedSet`
- Returns the **first step** of the optimal path via backtracking

#### Heuristic Greedy (Pink Ghost)

Evaluates all neighbors and picks the one with **minimum Manhattan distance** to Pacman.

```cpp
// Manhattan distance: d = |dx| + |dy| (with toroidal wrapping)
Location HeuristicGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Simple yet effective for corridor-heavy mazes
- Handles toroidal wrapping for accurate distance calculation

#### Directional Greedy (Cyan Ghost)

Prioritses closing distance along the **major axis** first, creating predictable axis-aligned movement.

```cpp
// Weighted scoring: major_axis_distance × 1000 + minor_axis_distance
Location DirectionalGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Identifies major axis (larger of |dx| vs |dy|)
- 1000× multiplier ensures dominant axis is resolved first
- Produces highly predictable but effective corridor pursuit

#### Aggressive Greedy (Orange Ghost)

The most dangerous ghost — it targets **where Pacman will be**, not where he is.

```cpp
// Predicts Pacman's position 2 steps ahead
// predicted = ((pos + dir*2) % SIZE + SIZE) % SIZE
Location AggressiveGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Projects Pacman's position 2 ticks into the future based on current direction
- Uses modular arithmetic for wrapping: `((x + dir*2) % WIDTH + WIDTH) % WIDTH`
- Selects neighbor closest (Euclidean) to the **predicted** position

#### Scatter Strategy (Nearest 2 Ghosts — during Lightning)

During chain lightning, all ghosts retreat toward corner `(0, 0)`.

```cpp
Location ScatterStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Uses Euclidean distance to scatter target
- Overrides normal strategy during lightning events

### Strategy Comparison

| Strategy | Optimality | Complexity | Predictability | Danger Level |
|----------|-----------|------------|----------------|--------------|
| A\* Pathfinding | ✅ Optimal | O(V log V) | Low | ⭐⭐⭐⭐⭐ |
| Heuristic Greedy | ❌ Local | O(k) | Medium | ⭐⭐⭐ |
| Directional Greedy | ❌ Local | O(k) | High | ⭐⭐ |
| Aggressive Greedy | ❌ Local | O(k) | Low | ⭐⭐⭐⭐ |
| Distance Greedy | ❌ Local | O(k) | Medium | ⭐⭐⭐ |
| Scatter | N/A | O(k) | High | ⭐ |

> *k = number of neighbors (≤ 4 in a grid maze)*

## Advanced Game Mechanics

### Chain Lightning

A powerful offensive ability that punishes clustered ghosts.

| Property | Value |
|----------|-------|
| **Activation** | Press `Z` |
| **Cost** | 100 score points |
| **Target Selection** | Closest pair of ghosts (divide & conquer algorithm) |
| **Effect** | Slows 2 targeted ghosts for 20 ticks (skip every other move) |
| **Mode Override** | Forces **all** ghosts into SCATTER mode |
| **Visual** | Blue-white lightning bolt connecting affected ghosts |
| **Duration** | Lightning visual lasts 8 ticks (~2 seconds) |

### Ghost Territory System (Convex Hull)

The game computes a **convex hull** around all ghost positions every frame using the **QuickHull** algorithm.

**When Pacman is inside the ghost hull:**

| Entity | Speed Modifier |
|--------|---------------|
| Ghosts | Slowed to **0.95×** |
| Pacman | Boosted to **1.15×** |

**Algorithm Details:**
- QuickHull divide & conquer: find leftmost/rightmost → partition upper/lower → recurse
- Cross-product test for point sidedness: `(A→B) × (A→P)`
- Point-in-convex-polygon via consistent cross-product winding
- Requires ≥ 3 unique, non-collinear ghost positions for a valid hull
- **Complexity:** O(N log N) average

### Dynamic Role Assignment

Ghost roles are re-evaluated **every game tick** based on proximity to Pacman:

```
Sort ghosts by squared distance to Pacman (toroidal)
├── Closest 50%  → CHASE mode  (speed: 1.35×)
└── Farthest 50% → AMBUSH mode (speed: 1.15×)
```

During chain lightning, all ghosts switch to **SCATTER mode** (speed: 1.0×).

| Mode | Speed | Behavior |
|------|-------|----------|
| **CHASE** | 1.35× | Uses original AI strategy, aggressively pursues |
| **AMBUSH** | 1.15× | Uses original AI strategy, moderate pursuit |
| **SCATTER** | 1.0× | Overrides to ScatterStrategy, flees to corner |

> **Speed Cap:** Effective speed is hard-capped at **1.5×** — `std::min(speed * territoryMultiplier, 1.5)`

## Algorithms & Complexity

| Algorithm | Used For | Time Complexity | Space Complexity |
|-----------|----------|----------------|-----------------|
| **A\* Pathfinding** | Red ghost pathfinding | O(V log V) | O(V) |
| **Manhattan Distance** | Heuristic greedy evaluation | O(1) | O(1) |
| **Euclidean Distance** | Distance/Aggressive greedy, territory | O(1) | O(1) |
| **QuickHull** | Ghost territory convex hull | O(N log N) avg | O(N) |
| **Point-in-Polygon** | Testing if Pacman is inside hull | O(N) | O(1) |
| **Closest Pair of Points** | Chain lightning target selection | O(N log N) | O(N) |
| **Graph BFS/Adjacency** | Maze representation & traversal | O(V + E) | O(V + E) |

> *V = walkable tiles (~368), E = edges (~4V), N = number of ghosts (4)*

## Data Structures

| Structure | Type | Purpose |
|-----------|------|---------|
| **Maze Graph** | `unordered_map<Location, Node*>` | O(1) node lookup by coordinate |
| **Node Adjacency** | `vector<Node*>` | Neighbor list per walkable tile |
| **Pellet Set** | `unordered_set<Location>` | Fast membership check for pellet collection |
| **A\* Open Set** | `priority_queue` (min-heap) | Frontier management for A\* search |
| **A\* Cost Maps** | `unordered_map<Location, double>` | g-cost and parent tracking |
| **Convex Hull** | `vector<Location>` | Ghost territory polygon vertices |
| **Location Hash** | `std::hash<Location>` specialisation | XOR-shift hash for unordered containers |

## License

This project is licensed under the **AGPL-3.0 License** — see the [LICENSE](LICENSE) file for details.
