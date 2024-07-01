#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include "IOcctViewer.h"
#include "CadModule.h"

namespace py = pybind11;


PYBIND11_MODULE(PyEchoCAD, m) {
    m.doc() = "Python bindings for Open CASCADE Technology";

    // ×¢²á TopoDS_Shape ÀàÐÍ
    py::class_<TopoDS_Shape>(m, "TopoDS_Shape");

    py::class_<CadModule>(m, "CadModule")
        .def(py::init<>())
        .def("initialize", &CadModule::initialize)
		.def("test", &CadModule::test)
        .def("create_box", &CadModule::createBox, py::arg("x"), py::arg("y"), py::arg("z"), "Create a box shape given its dimensions");

    py::class_<IOcctWidget, QOpenGLWidget, AIS_ViewController>(m, "IOcctWidget")
        .def(py::init<QWidget*>(), py::arg("theParent") = nullptr)
        .def("Viewer", &IOcctWidget::Viewer)
        .def("View", &IOcctWidget::View)
        .def("Context", &IOcctWidget::Context)
        .def("getGlInfo", &IOcctWidget::getGlInfo)
        .def("minimumSizeHint", &IOcctWidget::minimumSizeHint)
        .def("sizeHint", &IOcctWidget::sizeHint)
        .def("OnSubviewChanged", &IOcctWidget::OnSubviewChanged);
}

