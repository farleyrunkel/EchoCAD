// itestwidget.cpp

#include "itestwidget.h"
#include <QHBoxLayout>
#include <QDebug>

ITestWidget::ITestWidget(QWidget *parent) : QWidget(parent) {

    setupMainUi();
    setup3DWindow();

    // Create grid lines and z-axis
    createGridLines(100, 5000);

    createZAxis();

    setupCamera();

    setupConnections();
}

ITestWidget::~ITestWidget() {
    delete m_view;
}

void ITestWidget::setupConnections() {
    // Connect resize event to slot for handling resize
    connect(this, &ITestWidget::resizeSignal, this, &ITestWidget::onResize);
}

void ITestWidget::setupMainUi() {

    setLayout(new QHBoxLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);
}

void ITestWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    emit resizeSignal(event->size()); // Emit resize signal with new size
}

void ITestWidget::onResize(const QSize &newSize) {
    qDebug() << "ITestWidget::onResize: " << newSize;
    m_view->resize(newSize); // Resize the Qt3DWindow with the new size
}

void ITestWidget::setupCamera() {
    m_camera = m_view->camera();
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

void ITestWidget::setup3DWindow() {
    m_view = new Qt3DExtras::Qt3DWindow();
    m_rootEntity = new Qt3DCore::QEntity();
    m_view->defaultFrameGraph()->setClearColor(QColor(Qt::darkGray));
    m_view->setRootEntity(m_rootEntity);

    layout()->addWidget(QWidget::createWindowContainer(m_view));
}

void ITestWidget::createLineEntity(const QVector<QVector3D> &points, Qt3DExtras::QPhongMaterial *material) {
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

void ITestWidget::createGridLines(int step, int gridSize) {
    Qt3DExtras::QPhongMaterial *xAxisMaterial = new Qt3DExtras::QPhongMaterial();
    xAxisMaterial->setAmbient(Qt::lightGray);

    Qt3DExtras::QPhongMaterial *yAxisMaterial = new Qt3DExtras::QPhongMaterial();
    yAxisMaterial->setAmbient(Qt::lightGray);

    // Prepare points for X and Y axes
    QVector<QVector3D> xAxisPoints;
    QVector<QVector3D> yAxisPoints;

    for (int i = -gridSize; i <= gridSize; i += step) {
        xAxisPoints.append(QVector3D(i, -gridSize, 0.0f));
        xAxisPoints.append(QVector3D(i, gridSize, 0.0f));

        yAxisPoints.append(QVector3D(-gridSize, i, 0.0f));
        yAxisPoints.append(QVector3D(gridSize, i, 0.0f));
    }

    // Create X and Y grid lines
    createLineEntity(xAxisPoints, xAxisMaterial);
    createLineEntity(yAxisPoints, yAxisMaterial);
}

void ITestWidget::createZAxis() {
    Qt3DExtras::QPhongMaterial *zAxisMaterial = new Qt3DExtras::QPhongMaterial();
    zAxisMaterial->setAmbient(Qt::green);

    // Create Z-axis line
    createLineEntity({QVector3D(0, 0, -10000), QVector3D(0, 0, 10000)}, zAxisMaterial);
}
