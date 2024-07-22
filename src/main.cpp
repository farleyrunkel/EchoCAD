
#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <QSettings>

#include "MainWindow.h"
#include "config.h"

int main(int argc, char** argv) {

    QApplication aQApp(argc, argv);
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

    echocad::MainWindow* aWindow = new echocad::MainWindow;

    // if not exist EchoCAD.conf, create it
    
    QSettings settings("EchoCAD.conf", QSettings::IniFormat);

    settings.beginGroup("WindowPosition");
        int x = settings.value("x", -1).toInt();
        int y = settings.value("y", -1).toInt();
        int width = settings.value("width", -1).toInt();
        int height = settings.value("height", -1).toInt();
    settings.endGroup();

    if (x > 0 && y > 0 && width > 0 && height > 0)
    {
        aWindow->setGeometry(x, y, width, height);
    }

    aWindow->show();

    int aResult = aQApp.exec();

    settings.beginGroup("WindowPosition");
        settings.setValue("x", aWindow->x());
        settings.setValue("y", aWindow->y());
        settings.setValue("width", aWindow->width());
        settings.setValue("height", aWindow->height());
    settings.endGroup();

    return aResult;
}
