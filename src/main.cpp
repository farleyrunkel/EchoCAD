#include "MainWindow.h"
#include "config.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>

#include <pybind11/embed.h>

namespace py = pybind11;

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("EchoCAD");
    QApplication::setOrganizationName("EchoCAD");

#ifdef __APPLE__
    // Suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
    bool isCoreProfile = true;
    QSurfaceFormat glFormat;
    glFormat.setDepthBufferSize(24);
    glFormat.setStencilBufferSize(8);
    if (isCoreProfile) {
        glFormat.setVersion(4, 5);
    }
    glFormat.setProfile(isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);
#endif

    MainWindow mainWindow;
    mainWindow.resize(1250, 800);
    mainWindow.show();

    try {
        // Initialize the Python interpreter
        py::scoped_interpreter guard{};

        // Get OCCT DLL path from config.h
        std::string occt_install_path = OCCT_DLL_PATH;

        // Construct Python code as a string
        std::string python_code = std::string(R"(
import os
os.add_dll_directory(')") + occt_install_path + "')";

        // Convert Python code string to pybind11::str object
        py::str python_code_pystr(python_code);

        // Execute Python code
        py::exec(python_code_pystr);

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

    return app.exec();
}
