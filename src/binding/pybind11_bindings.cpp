#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <QOpenGLWidget>
#include "ModelEditor.h"
#include "CadModule.h"
#include <TopoDS_Shape.hxx>
#include <AIS_ViewController.hxx>

namespace py = pybind11;

PYBIND11_MODULE(echocad, m) {
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
    py::class_<echocad::ModelEditor, QOpenGLWidget, AIS_ViewController>(m, "IOcctWidget")
        .def(py::init<QWidget*>(), py::arg("theParent") = nullptr)
        .def("viewer", &echocad::ModelEditor::Viewer)
        .def("view", &echocad::ModelEditor::View)
        .def("context", &echocad::ModelEditor::Context)
        .def("getglinfo", &echocad::ModelEditor::getGlInfo)
        .def("minimumsizehint", &echocad::ModelEditor::minimumSizeHint)
        .def("sizehint", &echocad::ModelEditor::sizeHint)
        .def("onsubviewchanged", &echocad::ModelEditor::OnSubviewChanged)
        .def("display", &echocad::ModelEditor::Display);

    // bind function display using lambda and qt qapp to get main window
    m.def("display", [](const TopoDS_Shape& shape) {
        // import sys module
        py::module sys = py::module::import("sys");
        // get viewer from sys module
        auto viewer = sys.attr("viewer");
        // tarnsform viewer to IOcctWidget
        echocad::ModelEditor* occt = py::cast<echocad::ModelEditor*>(viewer);
        // display shape in viewer
        occt->Display(shape);
	});
}
