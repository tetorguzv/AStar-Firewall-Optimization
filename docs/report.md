# A* Firewall Optimization

## Overview

This project implements the A* (A-Star) algorithm to optimize firewall rule processing. By finding the most efficient sequence of rules for packet filtering, this approach enhances network security and performance. The heuristic-driven search prioritizes paths that minimize rule complexity while maintaining security constraints.

## Features

- Implements A* algorithm for firewall rule sequencing
- Reduces redundant or inefficient rule processing
- Uses heuristics to prioritize security and performance
- Optimized for large-scale rule sets

## How It Works

1. Each firewall rule is represented as a node in a graph.
2. A* searches for the optimal path from the start rule to the goal (ALLOW destination).
3. The heuristic function helps prioritize promising paths while ignoring inefficient ones.
4. The algorithm efficiently selects rules that balance security and network performance.

## Complexity Analysis

- **Worst case:** O((n + e) log n)
- **Optimized case:** Heuristic reduces search space, improving efficiency over uniform-cost search.

## Dependencies

- C++
- Makefile for compilation
- Bash script for testing

## Usage

### Compile the program:
```sh
make
```

### Run the program:
```sh
./firewall_optimizer < input_file
```

### Run test cases:
```sh
./runTests firewall_optimizer tests/
```

### View results:
Check the test output and `.diff` files in the `tests/` directory to validate results.

## Future Enhancements

- Integration with real-world firewall systems
- Advanced heuristics for dynamic network conditions
- Performance benchmarking on large rule sets

## Author

**Mateo Guzman**

