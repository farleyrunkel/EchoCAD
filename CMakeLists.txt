cmake_minimum_required(VERSION 3.5)

project(EchoCAD VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(src/3rdparty)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Test 3DCore 3DExtras 3DInput 3DRender)

# Include the src directory and all its subdirectories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Find all direct subdirectories under the src directory
file(GLOB SUBDIRS RELATIVE ${SRC_DIR} ${SRC_DIR}/*)
foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${SRC_DIR}/${subdir})
        include_directories(${SRC_DIR}/${subdir})
    endif()
endforeach()

set(PROJECT_SOURCES
        src/main.cpp
        src/gui/mainwindow.cpp
        src/gui/mainwindow.h

        src/tests/itestwidget.cpp
        src/tests/itestwidget.h
        src/gui/initializer.cpp
        src/gui/initializer.h

)

set(PROJECT_RESOURCES
        resources.qrc
)

if(WIN32)
    list(APPEND PROJECT_RESOURCES resources/resources.rc)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EchoCAD
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_RESOURCES}
        src/3d/grid.h src/3d/grid.cpp
        src/3d/scene.h src/3d/scene.cpp
        src/core/gptprocessor.h src/core/gptprocessor.cpp
        src/gui/ioverlaywidget.h src/gui/ioverlaywidget.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET EchoCAD APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(EchoCAD SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(EchoCAD
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(OCCT_LINK OpenCASCADE::TKBO OpenCASCADE::TKBRep OpenCASCADE::TKShHealing OpenCASCADE::TKTopAlgo OpenCASCADE::TKGeomBase OpenCASCADE::TKMath)

target_link_libraries(EchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
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

include(GNUInstallDirs)
install(TARGETS EchoCAD
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EchoCAD)
endif()
