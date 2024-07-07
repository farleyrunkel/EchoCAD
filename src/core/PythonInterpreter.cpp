#include "PythonInterpreter.h"

#include <QDebug>

PythonInterpreter::PythonInterpreter() : guard{} {
  connect(this, &PythonInterpreter::dllNeeded, this, &PythonInterpreter::exportDLL);
}

PythonInterpreter::~PythonInterpreter() {
    // Cleanup code if needed
}

void PythonInterpreter::exportDLL(const QString& theDll) {
    // Convert QString to std::string
    std::string dllPath = theDll.toStdString();

    // Construct Python code as a string
    std::string python_code = R"(
// Import dll path
import os
os.add_dll_directory(')" + dllPath + R"('))";

    // Execute the constructed Python script
    executeScript(python_code);
}


void PythonInterpreter::executeScript(const std::string& script) {

    emit logMessage(QString::fromStdString(script));

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
