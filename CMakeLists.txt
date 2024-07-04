cmake_minimum_required(VERSION 3.5)

project(EchoCAD VERSION 0.1 LANGUAGES CXX)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCASCADE_DIR "E:/Projects/Project-EchoCAD/OCCT-7_8_1-install/cmake")
set(PYTHON_DIR "${CMAKE_BINARY_DIR}/python")
set(PYTHONOCC_INSTALL_DIRECTORY ${PYTHON_DIR}/Lib/site-packages)

# Include the file to download and extract Python
include(cmake/setupPython.cmake) 

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

message(STATUS "Python3 interpreter: ${Python3_EXECUTABLE}")
message(STATUS "Python include directory: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python library release: ${Python3_LIBRARY_RELEASE}")

# Include directories and link directories
include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_RELEASE}) 

# Find Qt and OpenGL packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Core5Compat Test 3DCore 3DExtras 3DInput 3DRender)
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
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Define project resources
set(PROJECT_RESOURCES ${CMAKE_SOURCE_DIR}/resources/resources.qrc) 

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

pybind11_add_module(PyEchoCAD 
    src/core/pybind11_bindings.cpp 
    src/core/cadmodule.cpp
    src/core/cadmodule.h
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/IOcctViewer.cpp
    src/gui/IOcctViewer.h
    src/gui/OcctGlTools.cpp
    src/gui/OcctGlTools.h
            ${PROJECT_SOURCES}
)

# Link pybind11 and Python libraries
target_link_libraries(PyEchoCAD PRIVATE pybind11::module)
target_link_libraries(PyEchoCAD PRIVATE ${PYTHON_LIBRARIES})
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(PyEchoCAD PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
    # Link OpenCASCADE libraries
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(PyEchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(PyEchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Link OpenCASCADE libraries
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(EchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(EchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Link Qt and OpenGL libraries
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
target_link_libraries(EchoCAD PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(EchoCAD PRIVATE ${PYTHON_LIBRARIES})

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

# 生成配置头文件
message(STATUS "Generating config.h")
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)
# 包含生成的配置头文件
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})

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
