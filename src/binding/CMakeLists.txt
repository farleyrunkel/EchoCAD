# Add a Python module using pybind11
# Specify the list of source files for creating the Python binding module
pybind11_add_module(PyEchoCAD 
    ${CMAKE_CURRENT_SOURCE_DIR}/../binding/pybind11_bindings.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/../gui/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../gui/mainwindow.h

    ${CMAKE_CURRENT_SOURCE_DIR}/../core/gptprocessor.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/gptprocessor.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/../core/cadmodule.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/cadmodule.h

    ${CMAKE_CURRENT_SOURCE_DIR}/../gui/OcctGlTools.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../gui/OcctGlTools.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../gui/IOcctViewer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../gui/IOcctViewer.h
)

# Link pybind11 and Python libraries
# Link the pybind11 module
target_link_libraries(PyEchoCAD PRIVATE pybind11::module)

# Link the Python libraries
target_link_libraries(PyEchoCAD PRIVATE ${PYTHON_LIBRARIES})


# Link OpenGL libraries
target_link_libraries(PyEchoCAD PRIVATE ${OPENGL_LIBRARIES})

# Link additional Qt libraries
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Link OpenCASCADE libraries
# Loop through the list of OpenCASCADE libraries and link both debug and optimized versions
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(PyEchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(PyEchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()
