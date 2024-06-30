#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "CadModule.h"

namespace py = pybind11;

PYBIND11_MODULE(PyEchoCAD, m) {
    m.doc() = "Python bindings for Open CASCADE Technology";

    py::class_<CadModule>(m, "CadModule")
        .def(py::init<>())
        .def("initialize", &CadModule::initialize)
		.def("test", &CadModule::test)
        .def("create_box", &CadModule::createBox, py::arg("x"), py::arg("y"), py::arg("z"), "Create a box shape given its dimensions");
}
