#include "OcctEntity.h"
#include <Qt3DCore/QGeometry>
#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QAttribute>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <Poly_Triangulation.hxx>
#include <QDebug>


OcctEntity::OcctEntity(Qt3DCore::QEntity* parent)
    : Qt3DCore::QEntity(parent) {
}

OcctEntity::~OcctEntity() {
}

void OcctEntity::addShape(const TopoDS_Shape& shape) {
    qDebug() << "Start addShape";

    try {
        // Mesh the shape
        qDebug() << "Meshing shape";
        BRepMesh_IncrementalMesh(shape, 0.1);

        qDebug() << "TopExp_Explorer";

        for (TopExp_Explorer explorer(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
            const TopoDS_Face& face = TopoDS::Face(explorer.Current());
            addFace(face);
        }
    }
    catch (Standard_Failure& e) {
        qWarning() << "Exception:" << e.GetMessageString();
    }

    qDebug() << "End addShape";
}

void OcctEntity::addLine(const TopoDS_Shape& shape) {
    qDebug() << "Start addShape";

    try {
        // Manually create the vertex data for a diagonal line
        QByteArray vertexData;
        vertexData.resize(2 * 3 * sizeof(float)); // 2 vertices, 3 coordinates (x, y, z) each
        float* vertexDataPtr = reinterpret_cast<float*>(vertexData.data());

        // First vertex at origin (0, 0, 0)
        *vertexDataPtr++ = -110.0f;
        *vertexDataPtr++ = -110.0f;
        *vertexDataPtr++ = -110.0f;

        // Second vertex at (1, 1, 1) - diagonal line
        *vertexDataPtr++ = 210.0f;
        *vertexDataPtr++ = 110.0f;
        *vertexDataPtr++ = 110.0f;

        // Manually create the index data for the line
        QByteArray indexData;
        indexData.resize(2 * sizeof(unsigned int)); // 2 indices
        unsigned int* indexDataPtr = reinterpret_cast<unsigned int*>(indexData.data());

        // Indexing the two vertices
        *indexDataPtr++ = 0;
        *indexDataPtr++ = 1;

        // Creating the geometry
        Qt3DCore::QGeometry* geometry = new Qt3DCore::QGeometry(this);

        // 设置顶点缓冲区
        Qt3DCore::QBuffer* vertexBuffer = new Qt3DCore::QBuffer();
        vertexBuffer->setData(vertexData);

        Qt3DCore::QAttribute* positionAttribute = new Qt3DCore::QAttribute();
        positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
        positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
        positionAttribute->setVertexSize(3);
        positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
        positionAttribute->setBuffer(vertexBuffer);
        positionAttribute->setByteStride(3 * sizeof(float));
        positionAttribute->setCount(2);

        geometry->addAttribute(positionAttribute);

        // 设置索引缓冲区
        Qt3DCore::QBuffer* indexBuffer = new Qt3DCore::QBuffer();
        indexBuffer->setData(indexData);

        Qt3DCore::QAttribute* indexAttribute = new Qt3DCore::QAttribute();
        indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
        indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
        indexAttribute->setBuffer(indexBuffer);
        indexAttribute->setCount(2);

        geometry->addAttribute(indexAttribute);

        Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer();
        geometryRenderer->setGeometry(geometry);
        geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

        Qt3DCore::QEntity* lineEntity = new Qt3DCore::QEntity(this);
        lineEntity->addComponent(geometryRenderer);

        Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial(lineEntity);
        material->setAmbient(Qt::blue);
        lineEntity->addComponent(material);

        qDebug() << "Finished adding line";
    }
    catch (Standard_Failure& e) {
        qWarning() << "Exception:" << e.GetMessageString();
    }

    qDebug() << "End addShape";
}


void OcctEntity::addFace(const TopoDS_Face& face) {
    TopLoc_Location location;
    Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

    if (triangulation.IsNull()) {
        qDebug() << "Triangulation is null, skipping face";
        return;
    }

    QByteArray vertexData = getVertexData(triangulation);
    QByteArray indexData = getIndexData(triangulation);
    int vertexCount = triangulation->NbNodes();
    int indexCount = triangulation->NbTriangles() * 3;

    Qt3DCore::QGeometry* geometry = createGeometry(vertexData, indexData, vertexCount, indexCount);
    createFaceEntity(geometry);

    qDebug() << "Finished processing face";
}

QByteArray OcctEntity::getVertexData(const Handle(Poly_Triangulation)& triangulation) {

    QByteArray vertexData;
    vertexData.resize(triangulation->NbNodes() * 3 * sizeof(float));
    float* vertexDataPtr = reinterpret_cast<float*>(vertexData.data());

    for (int i = 1; i <= triangulation->NbNodes(); ++i) {
        const gp_Pnt& pnt = triangulation->Node(i);
        *vertexDataPtr++ = static_cast<float>(pnt.X());
        *vertexDataPtr++ = static_cast<float>(pnt.Y());
        *vertexDataPtr++ = static_cast<float>(pnt.Z());
    }

    return vertexData;
}

QByteArray OcctEntity::getIndexData(const Handle(Poly_Triangulation)& triangulation) {

    QByteArray indexData;
    indexData.resize(triangulation->NbTriangles() * 3 * sizeof(unsigned int));
    unsigned int* indexDataPtr = reinterpret_cast<unsigned int*>(indexData.data());

    for (int i = 1; i <= triangulation->NbTriangles(); ++i) {
        const Poly_Triangle& triangle = triangulation->Triangle(i);
        Standard_Integer n1, n2, n3;
        triangle.Get(n1, n2, n3);
        *indexDataPtr++ = static_cast<unsigned int>(n1 - 1);
        *indexDataPtr++ = static_cast<unsigned int>(n2 - 1);
        *indexDataPtr++ = static_cast<unsigned int>(n3 - 1);
    }

    return indexData;
}

inline Qt3DCore::QGeometry* OcctEntity::createGeometry(const QByteArray& vertexData, const QByteArray& indexData, int vertexCount, int indexCount) {
    Qt3DCore::QGeometry* geometry = new Qt3DCore::QGeometry(this);

    // 设置顶点缓冲区
    Qt3DCore::QBuffer* vertexBuffer = new Qt3DCore::QBuffer();
    vertexBuffer->setData(vertexData);

    Qt3DCore::QAttribute* positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(vertexCount);

    geometry->addAttribute(positionAttribute);

    // 设置索引缓冲区
    Qt3DCore::QBuffer* indexBuffer = new Qt3DCore::QBuffer();
    indexBuffer->setData(indexData);

    Qt3DCore::QAttribute* indexAttribute = new Qt3DCore::QAttribute();
    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(indexCount);

    geometry->addAttribute(indexAttribute);

    return geometry;
}

#include <QColor>
#include <QRandomGenerator>

void OcctEntity::createFaceEntity(Qt3DCore::QGeometry* geometry) {
    Qt3DRender::QGeometryRenderer* geometryRenderer = new Qt3DRender::QGeometryRenderer();
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);

    Qt3DCore::QEntity* faceEntity = new Qt3DCore::QEntity(this);
    faceEntity->addComponent(geometryRenderer);

    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial(faceEntity);

    // 生成随机颜色
    QColor randomColor = QColor::fromRgb(
        QRandomGenerator::global()->bounded(256),
        QRandomGenerator::global()->bounded(256),
        QRandomGenerator::global()->bounded(256)
    );

    material->setDiffuse(randomColor);
    material->setAmbient(randomColor);

    faceEntity->addComponent(material);
}
