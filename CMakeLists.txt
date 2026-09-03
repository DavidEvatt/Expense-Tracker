
cmake_minimum_required(VERSION 3.20)

project(ExpenseBudgetTracker LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/source/*.cpp"
)

# Executable
add_executable(ExpenseBudgetTracker
    ${SOURCES}
)

# Include directory
target_include_directories(ExpenseBudgetTracker PRIVATE
    "${CMAKE_SOURCE_DIR}/includes"
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
# Rebuild | cmake -S . -B build
# get EXE | cmake --build build --config Release