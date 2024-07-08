#include "PythonInterpreter.h"

#include <QDebug>

PythonInterpreter::PythonInterpreter() : guard{} {
  connect(this, &PythonInterpreter::dllNeeded, this, &PythonInterpreter::exportDLL);
}

PythonInterpreter::~PythonInterpreter() {
    // Cleanup code if needed
}

void PythonInterpreter::exportDLL(const QString& theDll) {
    py::module::import("os").attr("add_dll_directory")(theDll.toStdString());
}


void PythonInterpreter::executeScript(const QString& script) {

    emit logMessage(script);

    try {
        // Convert the script to a pybind11 string
        py::str python_code_pystr(script.toStdString());

        // Execute the Python code
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
}
