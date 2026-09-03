```cmake
cmake_minimum_required(VERSION 3.20)

project(ExpenseBudgetTracker LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executable
add_executable(ExpenseBudgetTracker
    source/*.cpp
    includes/*.h
)

# MSVC compiler settings
if(MSVC)
    target_compile_options(ExpenseBudgetTracker PRIVATE
        /W4
        /WX
        /permissive-
        /EHsc
        /utf-8
    )

    target_compile_definitions(ExpenseBudgetTracker PRIVATE
        UNICODE
        _UNICODE
    )
endif()
```
# Rebuild | cmake -S . -B build
# get EXE | cmake --build build --config Release