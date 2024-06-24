// Grid.h
#ifndef GRID_H
#define GRID_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DCore/QAttribute>
#include <Qt3DCore/QBuffer>

class Grid : public Qt3DCore::QEntity {
public:
    Grid(Qt3DCore::QEntity *parent = nullptr);

private:
    void init(int step = 100, int gridSize = 5000);
    Qt3DCore::QGeometry *geometry;
    Qt3DRender::QGeometryRenderer *geometryRenderer;
    Qt3DCore::QBuffer *vertexDataBuffer;
    Qt3DCore::QBuffer *indexDataBuffer;
};

#endif // GRID_H
