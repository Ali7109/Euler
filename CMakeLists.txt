cmake_minimum_required(VERSION 3.10)

# Set the project name
project(EulerLang)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
)

# Add the executable
add_executable(EulerLang ${SOURCES})

# Optionally specify the output directory for executables
set_target_properties(EulerLang PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
