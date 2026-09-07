cmake_minimum_required(VERSION 3.20)

# Enables both C and C++ support so sqlite3.c compiles properly
project(ExpenseBudgetTracker LANGUAGES C CXX)

# C++ standard settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically find ALL your C++ source files and your SQLite C file
file(GLOB SOURCES 
    "${CMAKE_SOURCE_DIR}/source/Item.cpp"
    "${CMAKE_SOURCE_DIR}/source/main.cpp"
    "${CMAKE_SOURCE_DIR}/source/MenuManager.cpp"
    "${CMAKE_SOURCE_DIR}/includes/sqlite3.c"
)

# Define the Executable
add_executable(ExpenseTracker
    ${SOURCES}
)

# Header include paths
target_include_directories(ExpenseTracker PRIVATE
    "${CMAKE_SOURCE_DIR}/includes"
)

# MSVC compiler flags
if(MSVC)
    target_compile_options(ExpenseTracker PRIVATE
        /W4
        # /WX  <-- Keep commented out so SQLite's warnings don't break your build
        /permissive-
        /EHsc
        /utf-8
    )

    target_compile_definitions(ExpenseTracker PRIVATE
        UNICODE
        _UNICODE
    )
endif()

# ============================================================================
# BUILD COMMANDS (Run these in your command prompt/terminal from the root folder)
# ============================================================================
#
# 1. Clean old cache:   Remove-Item -Recurse -Force build
# 2. Configure:         cmake -S . -B build
# 3. Compile/Build:     cmake --build build --config Release
#
# ============================================================================
