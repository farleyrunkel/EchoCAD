#include "CadModule.h"

CadModule::CadModule() {
    // Initialize module
}

TopoDS_Shape CadModule::createBox(double x, double y, double z) {
    BRepPrimAPI_MakeBox boxMaker(x, y, z);
    return boxMaker.Shape();
}




