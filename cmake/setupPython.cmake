# cmake/setupPython.cmake

# Specify the Python version to find (equal to or greater than 3.9)
set(PYTHON_VERSION "3.9" CACHE STRING "Python version to find")

# Check if Python version is valid
if(${PYTHON_VERSION} VERSION_LESS "3.9")
    message(FATAL_ERROR "Python version must be 3.9 or greater")
endif()

# Find Python interpreter
find_package(Python3 ${PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)

# Check if Python interpreter is found
if(NOT Python3_Interpreter_FOUND OR NOT Python3_Development_FOUND)
    message(FATAL_ERROR "Python ${PYTHON_VERSION} interpreter or development files not found")
endif()

# Check if the Python directory exists in the build directory
if(NOT EXISTS ${PYTHON_DIR})
    # Create the Python directory in the build directory
    file(MAKE_DIRECTORY ${PYTHON_DIR})

    # Determine the root directory of the found Python installation
    get_filename_component(PYTHON_ROOT ${Python3_EXECUTABLE} DIRECTORY)
    message(STATUS "Python root directory: ${PYTHON_ROOT}")

    # Copy the entire Python directory to the build directory
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${PYTHON_ROOT}" "${PYTHON_DIR}")
else()
    message(STATUS "Python directory already exists: ${PYTHON_DIR}")
endif()

# Copy Python directory to installation directory
set(PYTHON_DIR_BASENAME python-${PYTHON_VERSION})
install(DIRECTORY ${PYTHON_DIR}/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PYTHON_DIR_BASENAME})

# Set environment variables
set(PYTHONHOME ${PYTHON_DIR} CACHE INTERNAL "Path to Python home directory")
set(PYTHONLIBS  ${PYTHON_DIR}/Lib CACHE INTERNAL "Path to Python library directory")

# Display information
message(STATUS "PythonHOME set to: ${PYTHONHOME}")
message(STATUS "PythonLIBS set to: ${PYTHONLIBS}")
message(STATUS "Python executable set to: ${Python3_EXECUTABLE}")
