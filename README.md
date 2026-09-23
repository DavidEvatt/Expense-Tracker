# Expense Tracker

A command-line expense and budget tracking application written in **C++17** with **SQLite** for persistent data storage.

The goal of this project is to provide a simple, lightweight way to record income and expenses while practicing real-world C++ concepts such as object-oriented programming, data structures, database integration, sorting, input validation, and CMake-based builds.

## Features

- Add income and expenses
- Store transaction names, amounts, dates, and transaction types
- Organize transactions by month
- Sort transactions chronologically
- Load saved transactions when the application starts
- Automatically save transactions to a SQLite database
- Persistent local data storage
- Command-line interface
- Input validation
- Colored terminal output
- CMake build system
- C++17 standard

## Tech Stack

- **C++17**
- **SQLite3**
- **CMake 3.20+**

## Project Structure

```text
Expense-Tracker/
│
├── Data/
│   └── myDataBase.db
│
├── includes/
│   ├── ActionValidator.h
│   ├── Item.h
│   ├── MenuManager.h
│   ├── PrettyColors.h
│   ├── sqlite3.c
│   └── sqlite3.h
│
├── source/
│   ├── Item.cpp
│   ├── MenuManager.cpp
│   └── main.cpp
│
├── CMakeLists.txt
└── .gitignore
```

The application is separated into several components:

- **`Item`** — Represents an individual financial transaction.
- **`MenuManager`** — Handles the command-line menu and user interaction.
- **`ActionValidator`** — Handles validation of user input and menu actions.
- **`PrettyColors`** — Provides terminal formatting and color utilities.
- **`main.cpp`** — Initializes the application, database, and main program loop.
- **SQLite3** — Handles persistent storage of financial transactions.

## Data Storage

Each transaction contains information including:

| Field | Description |
|---|---|
| `name` | Name of the transaction |
| `amount` | Transaction amount |
| `month` | Transaction month |
| `day` | Transaction day |
| `year` | Transaction year |
| `inc` | Whether the transaction is income |
| `occurs` | Occurrence information |

The database is automatically opened when the program starts. Existing transactions are loaded into memory and organized by month.

When the application exits, transactions are written back to the SQLite database.

## Building

### Requirements

Before building the project, make sure you have:

- [CMake](https://cmake.org/)
- A C++17-compatible compiler

### Clone the Repository

```bash
git clone https://github.com/DavidEvatt/Expense-Tracker.git
cd Expense-Tracker
```

### Configure CMake

```bash
cmake -S . -B build
```

### Build

For a Release build:

```bash
cmake --build build --config Release
```

The project produces an executable named:

```text
ExpenseTracker.exe
```

The project's CMake configuration uses C++17 and enables `/W4`, `/permissive-`, `/EHsc`, and UTF-8 support when compiling with MSVC. 

## Running

After building the project, the executable can be run from the build output directory.

For a typical Visual Studio generator:

```bash
.\build\Release\ExpenseTracker.exe
```

The program will initialize the SQLite database, load existing transactions, and then open the main menu.

## Database

The SQLite database is stored locally in:

```text
Data/myDataBase.db
```

The application creates the required `items` table automatically if it does not already exist.

This means no external database server is required. Everything needed for local data persistence is contained within the application.

## Architecture

The application follows a simple separation-of-responsibilities approach.

```text
                 ┌─────────────────┐
                 │     main.cpp    │
                 │ Application     │
                 │ Initialization  │
                 └────────┬────────┘
                          │
                          ▼
                 ┌─────────────────┐
                 │  MenuManager    │
                 │ User Interface  │
                 └────────┬────────┘
                          │
                          ▼
                 ┌─────────────────┐
                 │      Item       │
                 │  Transactions   │
                 └────────┬────────┘
                          │
                          ▼
                 ┌─────────────────┐
                 │    SQLite3      │
                 │ Persistent Data │
                 └─────────────────┘
```

Transactions are pulled straight from the data base.

## Current Status

This project is currently under active development.

The current focus is building a solid foundation for a larger personal finance application, including:

- Reliable transaction management
- Persistent database storage
- Improved budgeting functionality
- Cleaner application architecture
- Future expansion toward a more complete personal finance system

## Future Plans

Some planned improvements include:

- [ ] Edit existing transactions
- [ ] Delete transactions
- [ ] Monthly spending summaries
- [ ] Budget limits
- [ ] Balance calculations
- [ ] Transaction categories (Color Coded)
- [ ] Linux support
- [ ] Improved command-line interface
- [ ] Export/import functionality
- [ ] Some kind of User Interface

## Why I Built This

I built this project to have a way to breakdown my budget. I use a google sheets but my visibility is limited.
I am planning on using this application personally so that is keeps receiving quality of life and UI Updates.

## Author

**David Evatt + Ian Turkia**

GitHub: [DavidEvatt](https://github.com/DavidEvatt)

## License
This project is currently provided without a specified open-source license.
