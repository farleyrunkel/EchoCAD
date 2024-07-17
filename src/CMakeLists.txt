

# Find all source and header files in all subdirectories under the src directory
file(GLOB PROJECT_SUBSOURCES
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.h"
)

# Define project source files
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SUBSOURCES}
    CACHE INTERNAL "List of source files for the project"
)

