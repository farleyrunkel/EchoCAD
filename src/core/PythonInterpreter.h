#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include <pybind11/embed.h>
#include <string>

namespace py = pybind11;

class PythonInterpreter {
public:
    PythonInterpreter();
    ~PythonInterpreter();

    void executeScript(const std::string& script);

private:
    py::scoped_interpreter guard;
};

#endif // PYTHONINTERPRETER_H
