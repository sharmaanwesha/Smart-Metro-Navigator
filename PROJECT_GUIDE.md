# Delhi Metro System - Project Guide

This document explains the structure, control flow, and logic of the Metro System project. It is designed to help all group members understand how the code works.

## 📂 Project Structure

The project is split into 4 modular files to make it clean and manageable:

1.  **`1.cpp` (Main Entry Point)**
    *   Contains the `main()` function.
    *   Handles the **Main Menu** loop.
    *   Connects the User Authentication and Graph Logic.
    *   *Think of this as the "Controller".*

2.  **`Graph.h` (The Core Logic)**
    *   Contains the `Graph_M` class.
    *   Stores the Metro Map using an **Adjacency List** (`map<string, Vertex>`).
    *   Implements **Dijkstra's Algorithm** for finding shortest paths.
    *   Handles "Smart Search" for stations.

3.  **`User.h` (User Management)**
    *   Handles **Sign Up** and **Sign In**.
    *   Stores users in a `unordered_map` (fast lookups).
    *   Validates password strength.

4.  **`Utils.h` (Helpers)**
    *   Contains utility functions like `waitForEnter()` (to fix flickering).
    *   Defines ANSI Color codes for the UI.

---

## 🔄 Control Flow

1.  **Start**: Program begins in `main()` inside `1.cpp`.
2.  **Initialization**:
    *   `Graph_M g` is created.
    *   `createMetroMap(g)` populates it with stations and edges.
3.  **Authentication Phase**:
    *   Calls `displayAuthMenu()` from `User.h`.
    *   Loops until the user successfully signs in or exits.
4.  **Main Application Loop**:
    *   Displays the **Main Menu**.
    *   Based on user input (1-8), calls methods from `Graph_M` (like `displayMap`, `getShortestPath`).
    *   Uses `waitForEnter()` to pause so the user can read the output.

---

## 🧠 Key Data Structures

### 1. The Graph (`Graph_M`)
We use a **Hash Map based Adjacency List**:
```cpp
map<string, Vertex> vtces;
```
*   **Why Map?** It allows us to use Station Names (Strings) directly as IDs. No need to convert "Noida" to `0` or `1`.
*   **Vertex**: Each vertex holds a list of neighbors (`nbrs`) and the distance to them.

### 2. Dijkstra's Algorithm
Used for finding the shortest path.
*   **Priority Queue (`priority_queue`)**: We use a "Min Heap" to always explore the closest station first.
*   **Complexity**: $O(E \log V)$, which is very fast.

---
