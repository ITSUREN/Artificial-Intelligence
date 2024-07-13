# 🧩 8 Puzzle Problem
- A problem with 9 tiles listed (1-8) and a blank tile (0), that can move Left (L/⬅️), Right (R/➡️), Up (U/⬆️) and Down (D/⬇️):
    1. ⬅️➡️⬆️⬇️ if blank tile is at center position (2,2) on the table.
    2. ⬅️⬆️ or ⬅️⬇️ or ➡️⬆️ or ➡️⬇️ if on the 4 extreme corners.
    3. ⬅️⬆️⬇️ or ➡️⬆️⬇️ or ⬅️➡️⬆️ or ⬅️➡️⬇️ if on mid section of edges.
- A variant of the puzzle probelem. [see also 4 puzzle problem](./4PuzzleProblem.md)

# Problem components
- **States**: a snapshot of the board game
    - **Initial State**: the state from which the algorithm proceeds.
    - **Final State**: the state the algorithm tries to reach.
- **Algorithms Used**:
    1. [Breadth First Search (BFS)](./8PuzzleProblemBFS.md)
    2. Depth First Search 
    3. A* Search  