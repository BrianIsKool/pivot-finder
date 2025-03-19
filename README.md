# Price Analysis with Pivot Calculation

This C++ application fetches historical Bitcoin price data from a PostgreSQL database and performs pivot calculations to identify support and resistance levels. It outputs the calculated low and high levels to CSV files for further analysis.

## Features

- Fetches historical Bitcoin price data from a PostgreSQL database.
- Calculates pivots to identify potential support (low) and resistance (high) levels.
- Outputs the calculated low and high levels to two separate CSV files (`levels_low1.csv` and `levels_high1.csv`).
- The program can be customized to adjust the number of data points queried and the sensitivity of the pivot calculations.
- Supports fast processing of large data sets by utilizing efficient data structures.

## Requirements

To run this program, you'll need:

- **C++ compiler** (C++11 or later recommended)
- **PostgreSQL** with a database containing Bitcoin price data.
- PostgreSQL development libraries and headers installed on your system (`libpq-fe.h`).
- **CMake** for compiling the project (if applicable).

### Dependencies

- PostgreSQL client library (`libpq-fe.h`).

## Setup and Compilation

1. **Install PostgreSQL and PostgreSQL Development Libraries**:

   On Ubuntu/Debian:
   ```bash
   sudo apt-get update
   sudo apt-get install libpq-dev postgresql
