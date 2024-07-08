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
    emit logMessage("DLL imported successfully:\n" + theDll);
}


void PythonInterpreter::executeScript(const QString& script) {

    emit logMessage("Executing script:\n" + script);

    try {
        // Convert the script to a pybind11 string
        py::str python_code_pystr(script.toStdString());

        // Execute the Python code      
        py::exec(python_code_pystr);

        emit logMessage("Executed successfully:\n");

    }
    catch (const py::error_already_set& e) {
        qDebug() << "Python error: " << e.what();
        emit logMessage("Python error : \n" + QString(e.what()));
    }
    catch (const std::exception& e) {
        qDebug() << "Standard exception: " << e.what();
    }
    catch (...) {
        qDebug() << "Unknown error occurred";
    }
}
