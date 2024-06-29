cmake_minimum_required(VERSION 3.5)

project(EchoCAD VERSION 0.1 LANGUAGES CXX)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Manually set Python header and library paths
set(PYTHON_INCLUDE_DIR "C:/Users/95439/scoop/apps/python/3.12.3/include")
set(PYTHON_LIBRARY "C:/Users/95439/scoop/apps/python/3.12.3/libs/python312.lib")

# Check if the Python include directory exists
if(NOT EXISTS ${PYTHON_INCLUDE_DIR})
    message(FATAL_ERROR "Python include directory not found: ${PYTHON_INCLUDE_DIR}")
endif()

# Check if the Python library exists
if(NOT EXISTS ${PYTHON_LIBRARY})
    message(FATAL_ERROR "Python library not found: ${PYTHON_LIBRARY}")
endif()

# Include directories and link directories
include_directories(${PYTHON_INCLUDE_DIR})
link_directories(${PYTHON_LIBRARY})

# Find Qt and OpenGL packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Test 3DCore 3DExtras 3DInput 3DRender)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
find_package(OpenGL REQUIRED)

# Find OpenCASCADE package
find_package(OpenCASCADE REQUIRED)

# Include the src directory and all its subdirectories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB SUBDIRS RELATIVE ${SRC_DIR} ${SRC_DIR}/*) 
foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${SRC_DIR}/${subdir})
        include_directories(${SRC_DIR}/${subdir})
    endif()
endforeach() 

include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11)

# Define project resources
set(PROJECT_RESOURCES ${CMAKE_SOURCE_DIR}/resources.qrc) 

# Define project source files
set(PROJECT_SOURCES
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/tests/itestwidget.cpp
    src/tests/itestwidget.h
    src/gui/initializer.cpp
    src/gui/initializer.h
    src/3d/grid.h 
    src/3d/grid.cpp
    src/3d/scene.h 
    src/3d/scene.cpp
    src/core/gptprocessor.h 
    src/core/gptprocessor.cpp
    src/core/OcctEntity.h 
    src/core/OcctEntity.cpp
    src/gui/ioverlaywidget.h 
    src/gui/ioverlaywidget.cpp
    src/gui/OcctGlTools.cpp
    src/gui/OcctGlTools.h
    src/gui/IOcctViewer.cpp
    src/gui/IOcctViewer.h 
    src/helper/Python_wrapper.h      
    src/helper/Viewer.h   
    src/helper/Viewer.cpp
    src/helper/ViewerInteractor.h   
    src/helper/ViewerInteractor.cpp
)

# Add Windows specific resources
if(WIN32)
    list(APPEND PROJECT_RESOURCES ${CMAKE_SOURCE_DIR}/resources/resources.rc)
endif()

# Add executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EchoCAD
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
    )
else()
    if(ANDROID)
        add_library(EchoCAD SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(EchoCAD
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link OpenCASCADE libraries
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(EchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(EchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Link Qt and OpenGL libraries
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender )
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(EchoCAD PRIVATE ${OPENGL_LIBRARIES})

# Link pybind11 and Python libraries
target_link_libraries(EchoCAD PRIVATE pybind11::module ${PYTHON_LIBRARY})

# Set bundle identifier and other properties for macOS and Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EchoCAD)
endif()

set_target_properties(EchoCAD PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install target
include(GNUInstallDirs)
install(TARGETS EchoCAD
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EchoCAD)
endif()
