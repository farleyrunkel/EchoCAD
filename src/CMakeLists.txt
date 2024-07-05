

# Define project source files
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/mainwindow.h
    ${CMAKE_SOURCE_DIR}/src/tests/itestwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/tests/itestwidget.h
    ${CMAKE_SOURCE_DIR}/src/gui/initializer.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/initializer.h
    ${CMAKE_SOURCE_DIR}/src/3d/grid.h 
    ${CMAKE_SOURCE_DIR}/src/3d/grid.cpp
    ${CMAKE_SOURCE_DIR}/src/3d/scene.h 
    ${CMAKE_SOURCE_DIR}/src/3d/scene.cpp
    ${CMAKE_SOURCE_DIR}/src/core/gptprocessor.h 
    ${CMAKE_SOURCE_DIR}/src/core/gptprocessor.cpp
    ${CMAKE_SOURCE_DIR}/src/core/OcctEntity.h 
    ${CMAKE_SOURCE_DIR}/src/core/OcctEntity.cpp

    ${CMAKE_SOURCE_DIR}/src/core/cadmodule.cpp 
    ${CMAKE_SOURCE_DIR}/src/core/cadmodule.h

    ${CMAKE_SOURCE_DIR}/src/gui/ioverlaywidget.h 
    ${CMAKE_SOURCE_DIR}/src/gui/ioverlaywidget.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/OcctGlTools.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/OcctGlTools.h
    ${CMAKE_SOURCE_DIR}/src/gui/IOcctViewer.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/IOcctViewer.h
    ${CMAKE_SOURCE_DIR}/src/gui/ilineedit.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/ilineedit.h 
    ${CMAKE_SOURCE_DIR}/src/gui/stylemanager.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/stylemanager.h 
    ${CMAKE_SOURCE_DIR}/src/helper/Python_wrapper.h      
    ${CMAKE_SOURCE_DIR}/src/helper/Viewer.h
    ${CMAKE_SOURCE_DIR}/src/helper/Viewer.cpp
    ${CMAKE_SOURCE_DIR}/src/helper/ViewerInteractor.h   
    ${CMAKE_SOURCE_DIR}/src/helper/ViewerInteractor.cpp
     
    ${CMAKE_SOURCE_DIR}/src/binding/pybind11_bindings.cpp
    CACHE INTERNAL "List of source files for the project"
)
