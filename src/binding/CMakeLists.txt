# Add a Python module using pybind11
# Specify the list of source files for creating the Python binding module
message(STATUS "Creating Python binding module using pybind11")

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
    ${CMAKE_SOURCE_DIR}/src/core/PythonInterpreter.cpp 
    ${CMAKE_SOURCE_DIR}/src/core/PythonInterpreter.h

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

# Include the src directory and all its subdirectories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB SUBDIRS RELATIVE ${SRC_DIR} ${SRC_DIR}/*) 
foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${SRC_DIR}/${subdir})
        target_include_directories(PyEchoCAD PRIVATE ${SRC_DIR}/${subdir})
    endif()
endforeach() 

# Include directories
target_include_directories(PyEchoCAD PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11/include) 
target_include_directories(PyEchoCAD PRIVATE ${Qt${QT_VERSION_MAJOR}PrintSupport_INCLUDE_DIRS})
target_include_directories(PyEchoCAD PRIVATE ${OpenCASCADE_INCLUDE_DIR})
target_include_directories(PyEchoCAD PRIVATE ${Python3_INCLUDE_DIRS})
target_include_directories(PyEchoCAD PRIVATE ${QScintilla_INCLUDE_DIR})

target_include_directories(PyEchoCAD PRIVATE ${CMAKE_BINARY_DIR})

# Link OpenCASCADE libraries
foreach (LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(PyEchoCAD PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(PyEchoCAD PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Link Qt and OpenGL libraries
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(PyEchoCAD PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
target_link_libraries(PyEchoCAD PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(PyEchoCAD PRIVATE ${PYTHON_LIBRARIES})
target_link_libraries(PyEchoCAD PRIVATE ${Python3_LIBRARY_RELEASE}) 
target_link_libraries(PyEchoCAD PRIVATE debug qscintilla2_qt${QT_VERSION_MAJOR}d)
target_link_libraries(PyEchoCAD PRIVATE optimized qscintilla2_qt${QT_VERSION_MAJOR})
