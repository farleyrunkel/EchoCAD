# Add a Python module using pybind11
# Specify the list of source files for creating the Python binding module
pybind11_add_module(PyEchoCAD 
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
)

# Link pybind11 and Python libraries
# Link the pybind11 module
target_link_libraries(PyEchoCAD PRIVATE pybind11::module)

# Link the Python libraries
target_link_libraries(PyEchoCAD PRIVATE ${PYTHON_LIBRARIES})

target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# Link OpenGL libraries
target_link_libraries(PyEchoCAD PRIVATE ${OPENGL_LIBRARIES})

# Link additional Qt libraries
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
# Link OpenCASCADE libraries
# Loop through the list of OpenCASCADE libraries and link both debug and optimized versions
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(PyEchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(PyEchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()
