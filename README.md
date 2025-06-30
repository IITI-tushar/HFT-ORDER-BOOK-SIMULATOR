# HFT Order Book Simulator

This is a simple command-line application that simulates a high-frequency trading (HFT) order book. It processes a list of buy and sell orders, matches them, and reports the resulting trades.

## Features

- **Order Parsing**: Parses a stream of semi-colon separated orders (e.g., "BUY,AAPL,150.5,100;SELL,AAPL,150.6,50") into an array of `Order` structs.
- **Order Book Management**: Maintains separate order books for buy and sell orders using linked lists for each distinct price level.
- **Graph-Based Matching**: Models potential order matches using a bipartite graph (represented as an edge list), connecting buy orders with sell orders where the buy price is greater than or equal to the sell price.
- **Greedy Trade Execution**: Executes trades using a greedy algorithm on the potential matches and stores the executed trades in a linked list.
- **Market Impact Simulation**: Simulates market impact where large trades can shift the prices in the order book based on trade quantity and market depth.
- **Trade History Analysis**: Computes and displays aggregate trade metrics, such as total traded volume and the average trade price.
- **Robust Error Handling**: Includes error handling to gracefully manage and report invalid input formats.

## Getting Started

### Prerequisites

- A C++ compiler that supports C++11 or later (e.g., g++).

### Compilation

To compile the project, navigate to the root directory and run the following command:

```bash
g++ main.cpp order_book.cpp -o hft_simulator
```

### Running

After successful compilation, you can run the simulator with:

```bash
./hft_simulator
```

The input orders are currently hardcoded in `main.cpp`. You can modify the `input` string to test with different order sequences.

## Code Structure

- **`main.cpp`**: The main entry point of the application. It contains the main loop, initializes the data, and calls the core logic functions.
- **`order_book.h`**: The header file that defines all the necessary data structures (`Order`, `OrderNode`, `PriceLevel`, `Trade`, etc.) and declares the functions used in the simulation.
- **`order_book.cpp`**: The implementation file containing the logic for parsing input, building the order book, matching orders, and printing trades.

## Technical Details

### Data Structures Used

- **`Order`**: Stores the basic information of an order (ID, type, price, quantity) after parsing from the input string.
- **`OrderNode`**: Represents an order within a linked list at a specific price level. This allows for multiple orders at the same price.
- **`PriceLevel`**: Represents a unique price point in the order book. It contains a pointer to a linked list of `OrderNode`s, grouping all orders at that price.
- **`Edge`**: A helper structure used in the matching algorithm to represent a potential trade between a specific buy order and a sell order.
- **`Trade`**: Stores the details of an executed trade (ID, price, quantity).

### Algorithm Analysis

The time and space complexities of the main algorithms are as follows:

- **`parseInput`**
  - **Time Complexity**: `O(N)`, where `N` is the length of the input string. The function performs a single pass over the string to extract order data.
  - **Space Complexity**: `O(M)`, where `M` is the number of orders parsed. This space is used for the `orders` array.

- **`buildOrderBook`**
  - **Time Complexity**: `O(P^2)`, where `P` is the number of unique price levels. The dominant factor is the bubble sort (`sortPriceLevels`) used to order the buy and sell price levels.
  - **Space Complexity**: `O(M)`, where `M` is the total number of orders. This space is used to create the `OrderNode` objects for each order.

- **`matchOrders`**
  - **Time Complexity**: `O(M^2)` in the worst case, where `M` is the number of orders. The `buildGraph` function creates a list of all possible trades, which can be proportional to the square of the number of orders.
  - **Space Complexity**: `O(M^2)` in the worst case. This is for storing the `edges` array, which holds all potential trades.
# HFT-ORDER-BOOK-SIMULATOR
