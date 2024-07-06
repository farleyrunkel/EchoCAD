#include "PythonInterpreter.h"
#include <QDebug>

PythonInterpreter::PythonInterpreter() : guard{} {
    // Initialization code if needed
}

PythonInterpreter::~PythonInterpreter() {
    // Cleanup code if needed
}

void PythonInterpreter::executeScript(const std::string& script) {
    try {
        // Convert the script to a pybind11 string
        py::str python_code_pystr(script);

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
