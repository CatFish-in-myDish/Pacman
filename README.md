<h1 align="center">Graph-Based Pacman Survival Game</h1>
  <p align="center">
    <strong>A C++ (Qt5) implementation of Pacman powered by graph theory, advanced AI pathfinding, and computational geometry</strong>
  </p>

## Feature Highlights

- **6 Distinct AI Ghosts** — A\* Pathfinding, Manhattan Heuristic, APSP Ambush, Predictive Pursuit, Cooperative Pinch, and Hybrid Random
- **Power Pellets & Frightened Mode** — Eat power pellets to frighten ghosts; frightened ghosts backtrack and can be eaten for bonus points
- **Chain Lightning Ability** — Spend score to slow ghosts using a closest-pair divide & conquer algorithm
- **Ghost Territory System** — QuickHull convex hull detects encirclement and dynamically adjusts speeds
- **Dynamic Role Assignment** — Ghosts switch between Chase, Normal, Ambush, and Scatter modes every tick
- **Ghost Eating** — Catch frightened ghosts for 200 bonus points; eaten ghosts respawn at their start position
- **Toroidal Map Wrapping** — Opposite maze edges are connected, allowing seamless wrap-around movement
- **Detailed Ghost & Wall Rendering** — Ghosts rendered with classic shapes; walls use rounded corners for a polished look
- **Multi-Round Progression** — Clear all pellets and power pellets to advance rounds with cumulative scoring

## Game Overview

### Objective

Eat **every pellet and power pellet** on the maze while avoiding six ghosts, each driven by a unique AI algorithm. Grab **power pellets** to frighten the ghosts and eat them for bonus points. Use the **Chain Lightning** ability strategically to slow down pursuing ghosts and survive long enough to clear the board.

### How to Play

1. **Launch** the game — you'll see the maze with pellets on every walkable tile and power pellets near corners
2. **Press any arrow key** to start — Pacman begins moving and ghosts activate
3. **Eat pellets** (+10 points each) to build your score
4. **Eat power pellets** (+50 points) to activate **Frightened Mode** — all ghosts turn blue and backtrack
5. **Eat frightened ghosts** (+200 points each) — they respawn at their starting positions
6. **Avoid normal ghosts** — contact with any non-frightened ghost ends the game
7. **Use Chain Lightning** (press `Z`) when your score ≥ 100 to slow the two closest ghosts
8. **Clear all pellets and power pellets** to win the round and advance to the next

### Controls

| Key             | Action                                                     |
|-----------------|------------------------------------------------------------|
| `↑` `↓` `←` `→` | Move Pacman (also starts the game on first press)          |
| `Z`             | Chain Lightning — costs 100 score, slows 2 closest ghosts  |
| `R`             | Restart — full reset after Game Over, next round after Win |

### Game States

| State         | Trigger                             | Display                                   |
|---------------|-------------------------------------|-------------------------------------------|
| **Playing**   | First arrow key press               | Active maze with HUD                      |
| **You Win!**  | All pellets & power pellets eaten   | Green overlay (48pt), auto-advances in 2s |
| **Game Over** | Non-frightened ghost catches Pacman | Red overlay (48pt), press `R` to restart  |

## Ghost Algorithm Deep Dive

The game features **six ghosts**, each employing a **unique pathfinding strategy**, creating diverse and challenging pursuit patterns. Ghost behaviour is further modified by a **dynamic role assignment system** and a **frightened mode** triggered by power pellets.

### The Six Ghosts

| Ghost  | Colour | Start Position | Strategy          | Personality                                                                       |
|--------|--------|----------------|-------------------|-----------------------------------------------------------------------------------|
| **M1** | Red    | (2, 2)         | A\* Pathfinding   | Optimal pursuer — always finds the shortest path                                  |
| **M2** | Pink   | (52, 2)        | Heuristic Greedy  | Manhattan-guided — efficient axis-aligned chaser                                  |
| **M3** | Cyan   | (2, 58)        | APSP Ambush       | Predictive Hunter — uses precomputed DP shortest paths                            |
| **M4** | Orange | (52, 58)       | Aggressive Greedy | Predictive — targets where Pacman *will* be                                       |
| **M5** | Teal   | (28, 58)       | Pinch Greedy      | Cooperative — flanks Pacman from the opposite side of the closest ghost           |
| **M6** | Purple | (28, 2)        | Random (Hybrid)   | Unpredictable — randomly mixes aggressive, distance, APSP, and scatter behaviours |

### Strategy Breakdown

#### A\* Pathfinding (M1 — Red Ghost)

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

#### Heuristic Greedy (M2 — Pink Ghost)

Evaluates all neighbours and picks the one with **minimum Manhattan distance** to Pacman.

```cpp
// Manhattan distance: d = |dx| + |dy| (with toroidal wrapping)
Location HeuristicGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Simple yet effective for corridor-heavy mazes
- Handles toroidal wrapping for accurate distance calculation

#### APSP Ambush (M3 — Cyan Ghost)

Predicts Pacman's future position and uses a **precomputed All-Pairs Shortest Path (APSP)** table to intercept him optimally.

```cpp
// Target 4 tiles ahead, O(1) optimal lookup in DP table
Location AllPairShortestPath::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Calculates APSP table via BFS from every tile (once at startup)
- Predicts intersection points 4 tiles ahead of Pacman
- Guarantees the absolute shortest path to the predicted ambush point

#### Aggressive Greedy (M4 — Orange Ghost)

The most dangerous ghost — it targets **where Pacman will be**, not where he is.

```cpp
// Predicts Pacman's position 2 steps ahead
// predicted = ((pos + dir*2) % SIZE + SIZE) % SIZE
Location AggressiveGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Projects Pacman's position 2 ticks into the future based on current direction
- Uses modular arithmetic for wrapping: `((x + dir*2) % WIDTH + WIDTH) % WIDTH`
- Selects neighbour closest (Euclidean) to the **predicted** position

#### Pinch Greedy (M5 — Teal Ghost)

A **cooperative flanking** strategy that coordinates with other ghosts to surround Pacman.

```cpp
// Finds the opposite side of Pacman relative to the closest other ghost
Location PinchGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Identifies the **closest other ghost** to Pacman
- Computes the point **diametrically opposite** that ghost relative to Pacman
- Moves greedily toward the flanking point using Euclidean distance (toroidal)
- Falls back to direct chase if no other ghost is available
- Creates natural **pincer / encirclement** behaviour with minimal computation — **O(N)** per frame

#### Random / Hybrid (M6 — Purple Ghost)

A **non-deterministic hybrid** strategy that randomly selects one of four behaviours each tick.

```cpp
// 25% chance each: Aggressive greedy, Distance greedy, APSP, or Scatter/Flee
Location RandomStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- **25% Aggressive greedy** — direct chase toward Pacman's current position
- **25% Distance greedy** — minimise Euclidean distance to Pacman
- **25% APSP optimal** — precomputed shortest-path intercept
- **25% Avoid ghosts** — maximise total distance from all other ghosts (spread-out)
- Creates emergent, unpredictable movement patterns — feels "alive" vs. deterministic chasers

#### Backtrack Strategy (Frightened Mode — All Ghosts)

When a **power pellet** is eaten, all ghosts enter **FRIGHTENED** mode and switch to this strategy.

```cpp
// Retrace recorded path history in reverse (LIFO), then flee
Location BacktrackStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- **Phase 1 — Backtrack:** Pops positions from the ghost's path history deque (LIFO), retracing its steps backward
- **Phase 2 — Flee:** Once history is exhausted, chooses the neighbour that **maximises Euclidean distance** from Pacman
- Path history is capped at 15 recorded positions
- Creates the characteristic "panicked wobbling" motion before purposeful fleeing
- **O(1)** per frame — extremely lightweight

#### Directional Greedy (Reserve Strategy)

An additional pursuit strategy available in the codebase for future use or custom configurations.

```cpp
Location DirectionalGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Evaluates neighbours based on directional distance heuristics
- Provides alternative chasing behaviour

#### Euclidean Distance Greedy (Reserve Strategy)

While the default game uses the 6 ghosts above, the codebase includes additional pursuit strategies.

```cpp
// Evaluates raw straight-line distance to Pacman
Location DistanceGreedyStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Uses straight-line Euclidean distance for evaluation
- More susceptible to getting stuck behind walls than A* or Heuristic

#### Scatter Strategy (During Chain Lightning)

During chain lightning, all ghosts retreat toward corner `(0, 0)`.

```cpp
Location ScatterStrategy::findNextMove(Graph *graph, Entity *monster, Entity *target);
```

- Uses Euclidean distance to scatter target
- Overrides normal strategy during lightning events

### Strategy Comparison

| Strategy               | Optimality | Complexity  | Predictability |
|------------------------|------------|-------------|----------------|
| A\* Pathfinding        | Optimal    | O(V log V)  | Low            |
| Heuristic Greedy       | Local      | O(k)        | Medium         |
| APSP Ambush            | Optimal    | O(1) lookup | Low            |
| Aggressive Greedy      | Local      | O(k)        | Low            |
| Pinch Greedy           | Local      | O(N)        | Low            |
| Random (Hybrid)        | Varies     | O(G)        | Very Low       |
| Backtrack (Frightened) | N/A        | O(1)        | Medium         |
| Distance Greedy        | Local      | O(k)        | Medium         |
| Scatter                | N/A        | O(k)        | High           |

> *k = number of neighbours (≤ 4 in a grid maze), N = number of ghosts, G = number of other ghosts*

## Advanced Game Mechanics

### Power Pellets & Frightened Mode

Power pellets are special collectibles placed near maze corners and key locations. Eating one triggers **Frightened Mode**.

| Property             | Value                                                               |
|----------------------|---------------------------------------------------------------------|
| **Pellet Locations** | 8 positions near corners and mid-maze intersections                 |
| **Score Bonus**      | +50 points per power pellet                                         |
| **Effect**           | All ghosts enter FRIGHTENED mode for 60 ticks                       |
| **Ghost Behaviour**  | Ghosts switch to BacktrackStrategy — retrace path then flee         |
| **Ghost Eating**     | Contact with a frightened ghost earns +200 points                   |
| **Ghost Respawn**    | Eaten ghosts teleport back to their spawn position                  |
| **Visual Indicator** | Power pellets blink red and are rendered larger than normal pellets |
| **Win Condition**    | Both regular pellets and power pellets must be cleared to win       |

### Chain Lightning

A powerful offensive ability that punishes clustered ghosts.

| Property             | Value                                                        |
|----------------------|--------------------------------------------------------------|
| **Activation**       | Press `Z`                                                    |
| **Cost**             | 100 score points                                             |
| **Target Selection** | Closest pair of ghosts (divide & conquer algorithm)          |
| **Effect**           | Slows 2 targeted ghosts for 20 ticks (skip every other move) |
| **Mode Override**    | Forces **all** ghosts into SCATTER mode                      |
| **Visual**           | Blue-white lightning bolt connecting affected ghosts         |
| **Duration**         | Lightning visual lasts 8 ticks (~2 seconds)                  |

### Ghost Territory System (Convex Hull)

The game computes a **convex hull** around all ghost positions every frame using the **QuickHull** algorithm.

**When Pacman is inside the ghost hull:**

| Entity | Speed Modifier      |
|--------|---------------------|
| Ghosts | Slowed to **0.9×**  |
| Pacman | Boosted to **1.1×** |

**Algorithm Details:**
- QuickHull divide & conquer: find leftmost/rightmost → partition upper/lower → recurse
- Cross-product test for point sidedness: `(A→B) × (A→P)`
- Point-in-convex-polygon via consistent cross-product winding
- Requires ≥ 3 unique, non-collinear ghost positions for a valid hull
- **Complexity:** O(N log N) average

### Dynamic Role Assignment

Ghost roles are re-evaluated **every game tick** based on proximity to Pacman. **M5 (Pinch)** and **M6 (Random)** are excluded from dynamic role assignment and always operate in **NORMAL** mode using their default strategies.

```
Sort eligible ghosts (M1–M4) by squared distance to Pacman (toroidal)
├── Closest        → CHASE mode   (speed: 1.5×)
├── Middle ghosts  → NORMAL mode  (speed: 1.0×, uses default strategy)
└── Farthest       → AMBUSH mode  (speed: 1.25×)
```

Role assignment is **suspended** during Frightened Mode and overridden during Chain Lightning.

| Mode           | Speed | Behaviour                                         |
|----------------|-------|---------------------------------------------------|
| **CHASE**      | 1.5×  | Uses original AI strategy, aggressively pursues   |
| **NORMAL**     | 1.0×  | Uses original AI strategy, standard pursuit       |
| **AMBUSH**     | 1.25× | Uses original AI strategy, moderate pursuit       |
| **SCATTER**    | 1.0×  | Overrides to ScatterStrategy, flees to corner     |
| **FRIGHTENED** | 1.0×  | Overrides to BacktrackStrategy, then flees Pacman |

> **Speed Cap:** Effective speed is hard-capped at **1.5×** — `std::min(speed * territoryMultiplier, 1.5)`

### Toroidal Map Wrapping

The maze implements **toroidal wrapping** — opposite edges of the map are connected, allowing entities to seamlessly wrap around from one side to the other. This affects:
- All pathfinding and distance calculations (A\*, Manhattan, Euclidean)
- Ghost territory convex hull computation
- Entity movement at map boundaries

## Algorithms & Complexity

| Algorithm                   | Used For                              | Time Complexity             | Space Complexity |
|-----------------------------|---------------------------------------|-----------------------------|------------------|
| **A\* Pathfinding**         | Red ghost pathfinding                 | O(V log V)                  | O(V)             |
| **Manhattan Distance**      | Heuristic greedy evaluation           | O(1)                        | O(1)             |
| **Euclidean Distance**      | Distance/Aggressive greedy, territory | O(1)                        | O(1)             |
| **All-Pairs Shortest Path** | APSP ambush & random strategy         | O(V²) precomp / O(1) lookup | O(V²)            |
| **Cooperative Pinch**       | Flanking target calculation           | O(N)                        | O(1)             |
| **Path Backtracking**       | Frightened mode ghost movement        | O(1)                        | O(H)             |
| **QuickHull**               | Ghost territory convex hull           | O(N log N) avg              | O(N)             |
| **Point-in-Polygon**        | Testing if Pacman is inside hull      | O(N)                        | O(1)             |
| **Closest Pair of Points**  | Chain lightning target selection      | O(N log N)                  | O(N)             |
| **Graph BFS/Adjacency**     | Maze representation & traversal       | O(V + E)                    | O(V + E)         |

> *V = walkable tiles, E = edges (~4V), N = number of ghosts (6), H = path history length (≤ 15)*

## Data Structures

| Structure            | Type                                 | Purpose                                           |
|----------------------|--------------------------------------|---------------------------------------------------|
| **Maze Graph**       | `unordered_map<Location, Node*>`     | O(1) node lookup by coordinate                    |
| **Node Adjacency**   | `vector<Node*>`                      | Neighbour list per walkable tile                  |
| **Pellet Set**       | `unordered_set<Location>`            | Fast membership check for pellet collection       |
| **Power Pellet Set** | `unordered_set<Location>`            | Tracks power pellet positions separately          |
| **Path History**     | `deque<Location>`                    | Per-ghost LIFO history for backtracking (cap: 15) |
| **APSP Table**       | `vector<vector<int>>`                | Precomputed all-pairs shortest distances          |
| **A\* Open Set**     | `priority_queue` (min-heap)          | Frontier management for A\* search                |
| **A\* Cost Maps**    | `unordered_map<Location, double>`    | g-cost and parent tracking                        |
| **Convex Hull**      | `vector<Location>`                   | Ghost territory polygon vertices                  |
| **Location Hash**    | `std::hash<Location>` specialisation | XOR-shift hash for unordered containers           |

## Dependencies & Building

This project requires **C++17**, **CMake** (v3.10+), and **Qt5** (Widgets module).

```bash
mkdir build && cd build
cmake ..
make
./Pacman
```

## License

This project is licensed under the **AGPL-3.0 License** — see the [LICENSE](LICENSE) file for details.
