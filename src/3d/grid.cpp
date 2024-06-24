#include "grid.h"
#include <Qt3DExtras/QPhongMaterial>

Grid::Grid(Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
    , geometry(new Qt3DCore::QGeometry(this))
    , geometryRenderer(new Qt3DRender::QGeometryRenderer(this))
    , vertexDataBuffer(new Qt3DCore::QBuffer())
    , indexDataBuffer(new Qt3DCore::QBuffer())
{
    init();

    auto *material = new Qt3DExtras::QPhongMaterial(this);
    material->setAmbient(Qt::lightGray);

    this->addComponent(geometryRenderer);
    this->addComponent(material);
}

void Grid::init(int step, int gridSize) {
    const int lines = gridSize * 2 + 1;
    const int verticesCount = lines * 4;

    QByteArray vertexData;
    vertexData.resize(verticesCount * 3 * sizeof(float));
    float *rawVertexArray = reinterpret_cast<float *>(vertexData.data());

    int index = 0;

    for (int i = -gridSize; i <= gridSize; i += step) {

        if (i == 0) {continue; }
        rawVertexArray[index++] = i;
        rawVertexArray[index++] = -gridSize;
        rawVertexArray[index++] = 0;

        rawVertexArray[index++] = i;
        rawVertexArray[index++] = gridSize;
        rawVertexArray[index++] = 0;

        rawVertexArray[index++] = -gridSize;
        rawVertexArray[index++] = i;
        rawVertexArray[index++] = 0;

        rawVertexArray[index++] = gridSize;
        rawVertexArray[index++] = i;
        rawVertexArray[index++] = 0;
    }

    vertexDataBuffer->setData(vertexData);

    auto *positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(verticesCount);

    geometry->addAttribute(positionAttribute);

    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}
