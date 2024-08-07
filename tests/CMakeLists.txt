# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(test1 LANGUAGES CXX)

message(STATUS "Start config project " ${PROJECT_NAME})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)

qt_add_executable(test1
    testregexp.cpp
)

set_target_properties(test1 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Define platform-specific libraries
set(PLATFORM_LIBS)
if(WIN32)
    list(APPEND PLATFORM_LIBS ws2_32)
endif()

# Link libraries
target_link_libraries(test1 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Test
    Qt6::Widgets
    ${PLATFORM_LIBS}
)

install(TARGETS test1
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET test1
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
