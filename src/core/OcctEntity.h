#ifndef OCCTENTITY_H
#define OCCTENTITY_H

#include <Qt3DCore/QEntity>
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <Poly_Triangulation.hxx>
#include <Qt3DCore/QGeometry>
#include <QByteArray>

class OcctEntity : public Qt3DCore::QEntity {
    Q_OBJECT

public:
    explicit OcctEntity(Qt3DCore::QEntity* parent = nullptr);
    ~OcctEntity();

    void addShape(const TopoDS_Shape& shape);


    void addLine(const TopoDS_Shape& shape);

    void addFace(const TopoDS_Face& face);

    QByteArray getVertexData(const Handle(Poly_Triangulation)& triangulation);
    QByteArray getIndexData(const Handle(Poly_Triangulation)& triangulation);
    Qt3DCore::QGeometry* createGeometry(const QByteArray& vertexData, const QByteArray& indexData, int vertexCount, int indexCount);
    void createFaceEntity(Qt3DCore::QGeometry* geometry);
};

#endif // OCCTENTITY_H
