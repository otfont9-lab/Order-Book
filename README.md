# C++ Order Matching Engine

A high-performance order matching engine engineered from scratch in C++. This project simulates electronic trading infrastructure with a strong emphasis on low-latency execution and efficient data structures.

## Overview
In financial markets, matching engines serve as the critical infrastructure responsible for executing buy and sell orders fairly and instantaneously. This project models core exchange mechanics, focusing on price-time priority and high-efficiency indexing.

## Key Features
* **Price-Time Priority Execution**: Automatically matches bids and asks based on price competitiveness and chronological arrival.
* **Low-Latency Cancellations**: Employs an optimized indexing strategy to handle order deletions instantly without traversing the entire order book.
* **Performance Benchmarked**: Stress-tested to process hundreds of thousands of simulated orders seamlessly under optimized compilation settings.

## Tech Stack
* **Language**: C++17
* **Data Structures**: Standard Template Library (STL) containers tailored for rapid sorting and lookup operations (`std::map`, `std::list`, `std::unordered_map`).

## How to Run
To compile and execute the benchmark performance test:

```bash
g++ -O3 -std=c++17 trader_book.cpp -o matching_engine
./matching_engine
