#include "mainwindow.h"
#include "config.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("EchoCAD");
    QApplication::setOrganizationName("EchoCAD");

#ifdef __APPLE__
    // Suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
    bool isCoreProfile = true;
    QSurfaceFormat glFormat;
    glFormat.setDepthBufferSize(24);
    glFormat.setStencilBufferSize(8);
    if (isCoreProfile) {
        glFormat.setVersion(4, 5);
    }
    glFormat.setProfile(isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);
#endif

    MainWindow mainWindow;
    mainWindow.resize(1250, 800);
    mainWindow.show();

    return app.exec();
}
