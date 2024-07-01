
#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

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
    // suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
    bool isCoreProfile = true;
    QSurfaceFormat aGlFormat;
    aGlFormat.setDepthBufferSize(24);
    aGlFormat.setStencilBufferSize(8);
    if (isCoreProfile) { aGlFormat.setVersion(4, 5); }
    aGlFormat.setProfile(isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(aGlFormat);
#endif

    try {
        // Initialize Python interpreter
        py::scoped_interpreter guard{};

        // Import the PyEchoCAD module
        py::module echocad = py::module::import("PyEchoCAD");

        // Create an instance of the CadModule class
        auto module = echocad.attr("CadModule")();

        // Call methods defined in CadModule from Python
        module.attr("initialize")();

        auto test = module.attr("test")();
        qDebug() << "Test: " << QString::fromStdString(py::str(test).cast<std::string>());

        auto box = module.attr("create_box")(1.0, 2.0, 3.0);
        qDebug() << "Created box shape in Python: " << QString::fromStdString(py::str(box).cast<std::string>());
    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error: " << QString::fromStdString(e.what());
    }
    catch (const std::exception& e) {
        qDebug() << "C++ exception: " << QString::fromStdString(e.what());
    }


    MainWindow aMainWindow;
    aMainWindow.resize(1250, 800);
    aMainWindow.show();

    return aQApp.exec();
}
