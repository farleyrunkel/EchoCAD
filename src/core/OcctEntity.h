#ifndef OCCTENTITY_H
#define OCCTENTITY_H

#include <Qt3DCore/QEntity>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>

class OcctEntity : public Qt3DCore::QEntity {
    Q_OBJECT

public:
    explicit OcctEntity(Qt3DCore::QEntity* parent = nullptr);
    ~OcctEntity();


    void addShape(const TopoDS_Shape& shape);
};

#endif // OCCTENTITY_H
