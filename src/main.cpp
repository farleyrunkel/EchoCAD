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

        // 将 QApplication 实例传递给 Python
        py::module sys = py::module::import("sys");
        sys.attr("main_window") = py::cast(&aMainWindow);

        // Create a Python code
        std::string str = R"(
import PyEchoCAD
import sys

main_window = sys.main_window
    )";

        // Execute Python code
        py::exec(py::str(str));
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
