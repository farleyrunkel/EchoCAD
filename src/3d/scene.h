#ifndef SCENE_H
#define SCENE_H

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>

class Scene : public Qt3DExtras::Qt3DWindow {
    Q_OBJECT

public:
    explicit Scene(QScreen *screen = nullptr);

    void setupLight();
    ~Scene();

    Qt3DCore::QEntity* rootEntity() const;

private:
    Qt3DCore::QEntity *m_rootEntity;
    void setupCamera();
    Qt3DRender::QCamera *m_camera;
    Qt3DCore::QEntity* createLineEntity(const QVector<QVector3D> &points, Qt3DExtras::QPhongMaterial *material);
    Qt3DCore::QEntity* createZAxis();
    Qt3DCore::QEntity* createXAxis();
    Qt3DCore::QEntity* createYAxis();

    void temp();
};

#endif // SCENE_H
