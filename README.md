# Graph Pathfinding Server (Student Project)

This is a simple client-server application that finds the shortest path between two vertices in an undirected graph using Breadth-First Search (BFS).

> Created as a second-year university project. The goal was to practice network programming, threading, and basic graph algorithms in C++.  
> I relied on various online resources and examples during development — this is not an original or production-level solution.

## Features

- TCP server using low-level POSIX sockets
- Multithreaded request handling (`std::thread`, `std::mutex`)
- Graph loaded from file (as edge list)
- BFS for shortest path
- Simple caching for repeated queries
- Command-line client for sending requests
- Graph generator script (linear graph)


Files
server.cpp — main server logic

client.cpp — sends a request to the server

generate_graph.cpp — creates a simple test graph (1000 nodes)

graph.csv — example generated graph



## How to Build & Run

> Tested with old GCC (C++11) on Linux virtual machine.

### Build

```bash
g++ server.cpp -o server -pthread
g++ client.cpp -o client
g++ generate_graph.cpp -o generate_graph
