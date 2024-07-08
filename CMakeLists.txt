cmake_minimum_required(VERSION 3.5)

project(EchoCAD VERSION 0.1 LANGUAGES CXX)

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenCASCADE_DIR "E:/Projects/Project-EchoCAD/OCCT-7_8_1-install/cmake")
set(QScintilla_DIR "E:/Temp/untitled3/out/install/x64-Debug/lib/cmake/QScintilla")

set(PYTHON_DIR "${CMAKE_BINARY_DIR}/python")
set(PYTHONOCC_INSTALL_DIRECTORY ${PYTHON_DIR}/Lib/site-packages)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rdparty)

message(STATUS "pybind11_INCLUDE_DIR: ${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11/include")

# Include the file to download and extract Python
include(cmake/setupPython.cmake)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

if(Python3_FOUND)
	message(STATUS "Python3 found")
    message(STATUS "Python3 interpreter: ${Python3_EXECUTABLE}")
    message(STATUS "Python include directory: ${Python3_INCLUDE_DIRS}")
    message(STATUS "Python library release: ${Python3_LIBRARY_RELEASE}")
else()
	message(FATAL_ERROR "Python3 not found")
endif()

# Find Qt and OpenGL packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Test )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Core5Compat Test 3DCore 3DExtras 3DInput 3DRender )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
find_package(OpenGL REQUIRED)

# Find OpenCASCADE package
find_package(OpenCASCADE REQUIRED)
find_package(QScintilla REQUIRED)

if(QScintilla_FOUND)
    message(STATUS "QScintilla found") 
    message(STATUS "QScintilla_INCLUDE_DIRS: ${QScintilla_INCLUDE_DIRS}")
    message(STATUS "QScintilla library:" QScintilla::qscintilla2_qt${QT_VERSION_MAJOR}d)

else()
    message(FATAL_ERROR "QScintilla not found")
endif()

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

set_target_properties(EchoCAD PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Set bundle identifier and other properties for macOS and Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.EchoCAD)
endif()

# Include the src directory and all its subdirectories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB SUBDIRS RELATIVE ${SRC_DIR} ${SRC_DIR}/*) 
foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${SRC_DIR}/${subdir})
        target_include_directories(EchoCAD PRIVATE ${SRC_DIR}/${subdir})
    endif()
endforeach() 

# Include directories
target_include_directories(EchoCAD PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11/include) 
target_include_directories(EchoCAD PRIVATE ${OpenCASCADE_INCLUDE_DIR})
target_include_directories(EchoCAD PRIVATE ${Python3_INCLUDE_DIRS})
target_include_directories(EchoCAD PRIVATE ${QScintilla_INCLUDE_DIRS})

# Generating config.h
message(STATUS "Generating config.h")
configure_file(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_BINARY_DIR}/config.h)
target_include_directories(EchoCAD PRIVATE ${CMAKE_BINARY_DIR})

# Link OpenCASCADE libraries
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(EchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(EchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Link Qt and OpenGL libraries
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test )
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
target_link_libraries(EchoCAD PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(EchoCAD PRIVATE ${PYTHON_LIBRARIES})

target_link_libraries(EchoCAD PRIVATE ${Python3_LIBRARY_RELEASE}) 

target_link_libraries(EchoCAD PRIVATE debug QScintilla::qscintilla2_qt${QT_VERSION_MAJOR}d)
target_link_libraries(EchoCAD PRIVATE optimized QScintilla::qscintilla2_qt${QT_VERSION_MAJOR})

# Install target
include(GNUInstallDirs)
install(TARGETS EchoCAD
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# copy QScintilla to cmake binary directory
add_custom_command(TARGET EchoCAD POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${QScintilla_BINARY_DIRS}
	$<TARGET_FILE_DIR:EchoCAD>
)

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EchoCAD)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/src/binding)
