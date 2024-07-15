#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <QOpenGLWidget>
#include "IOcctWidget.h"
#include "CadModule.h"
#include <QApplication>

namespace py = pybind11;

PYBIND11_MODULE(PyEchoCAD, m) {
    m.doc() = "Python bindings for EchoCAD";

    py::class_<QOpenGLWidget>(m, "QOpenGLWidget");
    py::class_<AIS_ViewController>(m, "AIS_ViewController");
    py::class_<TopoDS_Shape>(m, "TopoDS_Shape");

    // CadModule binding
    py::class_<CadModule>(m, "CadModule")
        .def(py::init<>())
        .def("initialize", &CadModule::initialize)
        .def("test", &CadModule::test)
        .def("create_box", &CadModule::createBox, py::arg("x"), py::arg("y"), py::arg("z"), "Create a box shape given its dimensions");

    // IOcctWidget binding
    py::class_<IOcctWidget, QOpenGLWidget, AIS_ViewController>(m, "IOcctWidget")
        .def(py::init<QWidget*>(), py::arg("theParent") = nullptr)
        .def("Viewer", &IOcctWidget::Viewer)
        .def("View", &IOcctWidget::View)
        .def("Context", &IOcctWidget::Context)
        .def("getGlInfo", &IOcctWidget::getGlInfo)
        .def("minimumSizeHint", &IOcctWidget::minimumSizeHint)
        .def("sizeHint", &IOcctWidget::sizeHint)
        .def("OnSubviewChanged", &IOcctWidget::OnSubviewChanged)
        .def("Display", &IOcctWidget::Display);

    // bind function display using lambda and qt qapp to get main window
    m.def("display", [](const TopoDS_Shape& shape) {
        // import sys module
        py::module sys = py::module::import("sys");
        // get viewer from sys module
        auto viewer = sys.attr("viewer");
        // tarnsform viewer to IOcctWidget
        IOcctWidget* occt = py::cast<IOcctWidget*>(viewer);
        // display shape in viewer
        occt->Display(shape);
	});
}
