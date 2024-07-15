#include "CadModule.h"

CadModule::CadModule() {
    // Initialize module
    initialize();
}

void CadModule::initialize() {
    std::cout << "CAD Module Initialized" << std::endl;
}

TopoDS_Shape CadModule::createBox(double x, double y, double z) {
    return BRepPrimAPI_MakeBox(x, y, z).Shape();
}

