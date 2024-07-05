

# Define project source files
set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/itestwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/itestwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/initializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/initializer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/3d/grid.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/3d/grid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/3d/scene.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/3d/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/gptprocessor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/gptprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/OcctEntity.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/OcctEntity.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/core/cadmodule.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/core/cadmodule.h

    ${CMAKE_CURRENT_SOURCE_DIR}/gui/ioverlaywidget.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/ioverlaywidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/OcctGlTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/OcctGlTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/IOcctViewer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/IOcctViewer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/ilineedit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/ilineedit.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/stylemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/stylemanager.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/Python_wrapper.h      
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/Viewer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/Viewer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/ViewerInteractor.h   
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/ViewerInteractor.cpp
     
    ${CMAKE_CURRENT_SOURCE_DIR}/binding/pybind11_bindings.cpp
    CACHE INTERNAL "List of source files for the project"
)
