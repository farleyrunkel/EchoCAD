
%module(package="OCC") Core

%include ../SWIG_files/common/ExceptionCatcher.i

%{
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <cadmodule.h>
%}

class CadModule {
public:
    %feature("autodoc", "1");
    CadModule();
    %feature("autodoc", "1");
    void initialize();
    %feature("autodoc", "1");
    TopoDS_Shape createBox(double x, double y, double z);

    %feature("autodoc", "1");
    std::string test() {
		return "Hello from CadModule!";
	}
};
