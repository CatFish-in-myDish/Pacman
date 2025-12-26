# Quick Start Guide - C++ Qt5 Pacman Game

## Build and Run

```bash
# From project root directory
cd "/home/chifuyu/College/2nd Year/SEM4/DAA/Project"

# Build (first time only)
mkdir -p build && cd build
cmake ..
make

# Run the game
./PacmanGame
```

## Rebuild After Changes

```bash
cd build
make
./PacmanGame
```

## Clean Build

```bash
rm -rf build
mkdir build && cd build
cmake ..
make
```

## Game Controls

- **↑ ↓ ← →**: Move Pacman
- **R**: Restart after game over
- **First Arrow Key**: Starts the game

## Monster Colors

- **Red (M1)**: Distance Greedy - Minimizes Euclidean distance
- **Pink (M2)**: Heuristic Greedy - Uses Manhattan distance
- **Cyan (M3)**: Directional Greedy - Prioritizes larger axis gap
- **Orange (M4)**: Aggressive Greedy - Predicts your movement

## Project Structure

- `cpp-src/` - All C++ source code
- `src/` - Original Java code (preserved)
- `build/` - Compiled executable
- `CMakeLists.txt` - Build configuration
