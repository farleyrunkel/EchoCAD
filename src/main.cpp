
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

    {
        py::scoped_interpreter guard{};

        // Use your Pybind11 module
        py::module echocad = py::module::import("PyEchoCAD");

         //Create an instance of CadModule
        CadModule cadModule;

         //Call methods defined in CadModule from Python
        echocad.attr("CadModule")().attr("initialize")();
       //auto test =  echocad.attr("CadModule")().attr("test")();
       //std::cout << "Test: " << py::str(test) << std::endl;
        // auto box = echocad.attr("CadModule")().attr("create_box")(1.0, 2.0, 3.0);
        //std::cout << "Created box shape in Python: " << py::str(box) << std::endl;
    }


    MainWindow aMainWindow;
    aMainWindow.resize(1250, 800);
    aMainWindow.show();

    return aQApp.exec();
}
