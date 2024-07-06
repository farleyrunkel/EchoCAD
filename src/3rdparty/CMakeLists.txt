# # Set OCCT cmake parameters

# # Specify the path to the OCCT source directory
# set(OCCT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/occt CACHE PATH "Path to OCCT source directory")

# # Print the value of OCCT_DIR for debugging purposes
# message(STATUS "OCCT_DIR: ${OCCT_DIR}")

# # Include the OCCT source directory as a subdirectory
# add_subdirectory(${OCCT_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11)

# if set qscintilla not add_subdirectory
if(NOT QScintilla_DIR)
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rdparty/QScintilla)
endif()