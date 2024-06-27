#include "OcctEntity.h"
#include <Qt3DCore/QGeometry>
#include <Qt3DCore/QBuffer>
#include <Qt3DCore/QAttribute>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <QDebug>

OcctEntity::OcctEntity(Qt3DCore::QEntity* parent)
    : Qt3DCore::QEntity(parent) {
}

OcctEntity::~OcctEntity() {
}

void OcctEntity::addShape(const TopoDS_Shape& shape) {
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
        *vertexDataPtr++ = 110.0f;
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
