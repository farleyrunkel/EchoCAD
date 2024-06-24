#include "scene.h"
#include <Qt3DCore/QTransform>

#include <Qt3DRender/QPointLight>
#include "grid.h"
#include <Qt3DExtras/QOrbitCameraController>

Scene::Scene(QScreen *screen) : Qt3DExtras::Qt3DWindow(screen) {
    m_rootEntity = new Qt3DCore::QEntity();

    // 设置摄像机
    setupCamera();

    defaultFrameGraph()->setClearColor(QColor(Qt::darkGray));
    setRootEntity(m_rootEntity);

    // // 设置前向渲染器
    // Qt3DExtras::QForwardRenderer *forwardRenderer = this->defaultFrameGraph();
    // forwardRenderer->setCamera(m_camera);
    // forwardRenderer->setClearColor(Qt::darkGray);

    // 添加网格背景
    new Grid(m_rootEntity);

    createZAxis();

    // 添加光源
    auto *light = new Qt3DRender::QPointLight(m_rootEntity);
    light->setColor("white");
    light->setIntensity(1);

    auto *lightTransform = new Qt3DCore::QTransform(m_rootEntity);
    lightTransform->setTranslation(QVector3D(10, 10, 10));

    auto *lightEntity = new Qt3DCore::QEntity(m_rootEntity);
    lightEntity->addComponent(light);
    lightEntity->addComponent(lightTransform);

}



Scene::~Scene() {
    delete m_rootEntity;
}

void Scene::setupCamera() {
    m_camera = this->camera();
    m_camera->lens()->setPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    m_camera->setPosition(QVector3D(100, -500, 100));
    m_camera->setUpVector(QVector3D(0, 0, 1));
    m_camera->setViewCenter(QVector3D(0, 0, 0));

    Qt3DExtras::QOrbitCameraController *camController =
        new Qt3DExtras::QOrbitCameraController(m_rootEntity);
    camController->setLinearSpeed(150.0f);
    camController->setLookSpeed(180.0f);
    camController->setCamera(m_camera);
}

Qt3DCore::QEntity* Scene::rootEntity() const {
    return m_rootEntity;
}


void Scene::createLineEntity(const QVector<QVector3D> &points, Qt3DExtras::QPhongMaterial *material) {
    Qt3DCore::QEntity *lineEntity = new Qt3DCore::QEntity(m_rootEntity);
    Qt3DCore::QGeometry *geometry = new Qt3DCore::QGeometry(lineEntity);

    QByteArray bufferArray;
    bufferArray.resize(points.size() * 3 * sizeof(float));
    float *positions = reinterpret_cast<float *>(bufferArray.data());

    for (int i = 0; i < points.size(); ++i) {
        positions[i * 3] = points[i].x();
        positions[i * 3 + 1] = points[i].y();
        positions[i * 3 + 2] = points[i].z();
    }

    Qt3DCore::QBuffer *buffer = new Qt3DCore::QBuffer();
    buffer->setData(bufferArray);

    Qt3DCore::QAttribute *positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(points.size());

    geometry->addAttribute(positionAttribute);

    Qt3DRender::QGeometryRenderer *geometryRenderer = new Qt3DRender::QGeometryRenderer();
    geometryRenderer->setGeometry(geometry);
    geometryRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    lineEntity->addComponent(geometryRenderer);
    lineEntity->addComponent(material);
}

void Scene::createZAxis() {
    Qt3DExtras::QPhongMaterial *zAxisMaterial = new Qt3DExtras::QPhongMaterial();
    zAxisMaterial->setAmbient(Qt::green);

    // Create Z-axis line
    createLineEntity({QVector3D(0, 0, -10000), QVector3D(0, 0, 10000)}, zAxisMaterial);
}

