// itestwidget.h

#ifndef ITESTWIDGET_H
#define ITESTWIDGET_H

#include <QWidget>
#include <Qt3DCore>
#include <Qt3DExtras>
#include <QScrollArea>

class ITestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ITestWidget(QWidget *parent = nullptr);
    ~ITestWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Qt3DExtras::Qt3DWindow *m_view;
    Qt3DCore::QEntity *m_rootEntity;
    Qt3DRender::QCamera *m_camera;

    void setup3DWindow();
    void setupCamera();
    void setupMainUi();
    void setupConnections();
    void createLineEntity(const QVector<QVector3D> &points, Qt3DExtras::QPhongMaterial *material);
    void createGridLines(int step, int gridSize);
    void createZAxis();
    void onResize(const QSize &newSize);

signals:
    void resizeSignal(const QSize &newSize);
};

#endif // ITESTWIDGET_H
