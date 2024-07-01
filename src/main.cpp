#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "CadModule.h"

namespace py = pybind11;

int main(int theNbArgs, char** theArgVec)
{
    QApplication aQApp(theNbArgs, theArgVec);

    QApplication::setApplicationName("EchoCAD");
    QApplication::setOrganizationName("EchoCAD");

#ifdef __APPLE__
    // Suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
    bool isCoreProfile = true;
    QSurfaceFormat aGlFormat;
    aGlFormat.setDepthBufferSize(24);
    aGlFormat.setStencilBufferSize(8);
    if (isCoreProfile) {
        aGlFormat.setVersion(4, 5);
    }
    aGlFormat.setProfile(isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(aGlFormat);
#endif

    MainWindow aMainWindow;
    aMainWindow.resize(1250, 800);
    aMainWindow.show();

    try {
        // Initialize the Python interpreter
        py::scoped_interpreter guard{};

        // Create a Python code
        std::string str = R"(
import PyEchoCAD

module = PyEchoCAD.CadModule()

module.initialize()

box = module.create_box(1000.0, 200.0, 3000.0)


    )";

        // Execute Python code
        py::exec(py::str(str));

        // Get the global namespace
        py::object global = py::globals();

        // Retrieve the 'box' object from the global namespace
        py::object box = global["box"];

        qDebug() << "Created box shape in Python: " << QString::fromStdString(py::str(box).cast<std::string>());

        // Convert the Python object to TopoDS_Shape
        TopoDS_Shape boxShape = box.cast<TopoDS_Shape>();

        // Display the boxShape in the OCCT viewer
        Handle(AIS_Shape) aisBox = new AIS_Shape(boxShape);

        // Assuming aMainWindow is an instance of the main window that contains the OCCT viewer
        aMainWindow.viewer()->Context()->Display(aisBox, true);
    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error: " << e.what();
    }
    catch (const std::exception& e) {
        qDebug() << "Standard exception: " << e.what();
    }
    catch (...) {
        qDebug() << "Unknown error occurred";
    }

    return aQApp.exec();
}
