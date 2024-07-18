# Add a Python module using pybind11
# Specify the list of source files for creating the Python binding module
message(STATUS "Creating Python binding module using pybind11")

# Find all source and header files in all subdirectories under the src directory
file(GLOB PROJECT_SUBSOURCES
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.h"
)

pybind11_add_module(echocad MODULE

    ${PROJECT_SUBSOURCES}
)

# Include directories
target_include_directories(echocad PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/pybind11/include) 
target_include_directories(echocad PRIVATE ${Qt${QT_VERSION_MAJOR}PrintSupport_INCLUDE_DIRS})
target_include_directories(echocad PRIVATE ${OpenCASCADE_INCLUDE_DIR})
target_include_directories(echocad PRIVATE ${Python3_INCLUDE_DIRS})
target_include_directories(echocad PRIVATE ${QScintilla_INCLUDE_DIR})

# Link OpenCASCADE libraries
foreach (LIB ${OpenCASCADE_LIBRARIES}) 
  target_link_libraries(echocad PRIVATE debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(echocad PRIVATE optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Link Qt and OpenGL libraries
target_link_libraries(echocad PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(echocad PRIVATE Qt${QT_VERSION_MAJOR}::3DCore Qt${QT_VERSION_MAJOR}::3DExtras Qt${QT_VERSION_MAJOR}::3DInput Qt${QT_VERSION_MAJOR}::3DRender)
target_link_libraries(echocad PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(echocad PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
target_link_libraries(echocad PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(echocad PRIVATE ${PYTHON_LIBRARIES})
target_link_libraries(echocad PRIVATE ${Python3_LIBRARY_RELEASE})
target_link_libraries(echocad PRIVATE debug qscintilla2_qt${QT_VERSION_MAJOR}d)
target_link_libraries(echocad PRIVATE optimized qscintilla2_qt${QT_VERSION_MAJOR})
