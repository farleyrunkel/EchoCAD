#include "PythonInterpreter.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

PythonInterpreter::PythonInterpreter() : guard{} {
  connect(this, &PythonInterpreter::dllNeeded, this, &PythonInterpreter::exportDLL);
  connect(this, &PythonInterpreter::moduleNeeded, this, &PythonInterpreter::loadModule);
}

PythonInterpreter::~PythonInterpreter() {
}

void PythonInterpreter::exportDLL(const QStringList& theDll) {
    py::module_ os  = py::module::import("os");
    for (auto dll : theDll) {
        os.attr("add_dll_directory")(dll.toStdString());
        emit logMessage("DLL imported successfully:\n" + dll);
    }
}

void PythonInterpreter::loadModule(const QString& thePath) {

    emit logMessage("Loading module from path:\n" + thePath);

    QString pydPath = thePath;
    // if pydPath is empty, return
    if (pydPath.isEmpty()) {
		emit logMessage("Path is empty");
		return;
	}
    
    // id pydPath is directory, get the first pyd file
    if (QFileInfo(pydPath).isDir()) {
		QDir dir(pydPath);
		QStringList filters;
		filters << "*.pyd";
		QStringList files = dir.entryList(filters, QDir::Files);
		if (files.size() > 0) {
            pydPath = dir.absoluteFilePath(files[0]);
		}
	}

    try {
        // Import necessary modules
        py::module_ importlib_util = py::module::import("importlib.util");
        py::module_ importlib_machinery = py::module::import("importlib.machinery");

        // Get the directory and module name from the path
        QString dirPath = QFileInfo(pydPath).absolutePath();
        QString moduleName = QFileInfo(pydPath).baseName();
        qDebug() << "Module name: " << moduleName;
        qDebug() << "Directory path: " << dirPath;

        // Set up loader details
        auto loaderDetails = py::make_tuple(importlib_machinery.attr("ExtensionFileLoader"), importlib_machinery.attr("EXTENSION_SUFFIXES"));
        auto toolsFinder = importlib_machinery.attr("FileFinder")(dirPath.toStdString(), loaderDetails);

        // Find the module specification
        auto toolboxSpecs = toolsFinder.attr("find_spec")(moduleName.toStdString());

        // Create a module from the specification
        auto toolbox = importlib_util.attr("module_from_spec")(toolboxSpecs);

        // Execute the module
        toolboxSpecs.attr("loader").attr("exec_module")(toolbox);

        emit logMessage("Module loaded successfully:\n" + moduleName);

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
