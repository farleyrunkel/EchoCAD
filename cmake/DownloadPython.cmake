
set(PYTHON_VERSION "3.12.0")
set(PYTHON_DIR "${CMAKE_BINARY_DIR}/python")

# Set download URL and extract command based on the operating system
if (WIN32)
    set(PYTHON_URL "https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-embed-amd64.zip")
    set(PYTHON_ARCHIVE "${CMAKE_BINARY_DIR}/python-${PYTHON_VERSION}-embed-amd64.zip")
    set(PYTHON_EXTRACT_COMMAND ${CMAKE_COMMAND} -E tar xvf ${PYTHON_ARCHIVE})
else()
    set(PYTHON_URL "https://www.python.org/ftp/python/${PYTHON_VERSION}/Python-${PYTHON_VERSION}.tgz")
    set(PYTHON_ARCHIVE "${CMAKE_BINARY_DIR}/Python-${PYTHON_VERSION}.tgz")
    set(PYTHON_EXTRACT_COMMAND ${CMAKE_COMMAND} -E tar xzf ${PYTHON_ARCHIVE})
endif()

# Check if the Python archive already exists
if(NOT EXISTS ${PYTHON_ARCHIVE})
    # Download Python archive
    file(DOWNLOAD ${PYTHON_URL} ${PYTHON_ARCHIVE}
         SHOW_PROGRESS)
else()
    message(STATUS "Python archive already exists: ${PYTHON_ARCHIVE}")
endif()

# Check if the Python exe already exists
if(NOT EXISTS ${PYTHON_DIR})
    # Create download directory
    file(MAKE_DIRECTORY ${PYTHON_DIR})
    # Extract Python archive
    execute_process(
        COMMAND ${PYTHON_EXTRACT_COMMAND}
        WORKING_DIRECTORY ${PYTHON_DIR}
        RESULT_VARIABLE result
    )

    # Check extraction result
    if(result)
        message(FATAL_ERROR "Failed to extract Python archive")
    endif()
else()
    message(STATUS "Python already extracted to: ${PYTHON_DIR}")
endif()

# Set environment variables
set(PYTHONHOME ${PYTHON_DIR} CACHE INTERNAL "Path to Python home directory")
set(PYTHONPATH ${PYTHON_DIR}/Lib CACHE INTERNAL "Path to Python library directory")

# Display information
message(STATUS "Python downloaded from: ${PYTHON_URL}")
message(STATUS "Python installed to: ${PYTHON_DIR}")
message(STATUS "PythonHOME set to: ${PYTHONHOME}")
message(STATUS "PythonPATH set to: ${PYTHONPATH}")
