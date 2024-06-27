#ifndef CADMODULE_H
#define CADMODULE_H

#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

class CadModule {
public:
    CadModule();
    void initialize();
    TopoDS_Shape createBox(double x, double y, double z);
};

#endif // CADMODULE_H
