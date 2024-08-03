
#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <QSettings>
#include <QTranslator>

#include "MainWindow.h"
#include "AppModule.h"
#include "GuiApplication.h"
#include "GuiDocument.h"
#include <QVersionNumber>

using namespace echocad;

static int initGui(GuiApplication* guiApp)
{
    if (!guiApp)
        return 1;
    return 1;
}
// Initializes and runs Mayo application
static int runApp(QCoreApplication* qtApp)
{

    // Initialize AppModule
    auto appModule = AppModule::instance();
    //appModule->settings()->setStorage(std::make_unique<QSettingsStorage>());
    //appModule->setRecentFileThumbnailRecorder(&createGuiDocumentThumbnail);

    auto app = appModule->application();

    // Initialize Gui application

    auto guiApp = new GuiApplication(app);
    initGui(guiApp);

    return qtApp->exec();
}

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

    int aResult = runApp(&aQApp);

    settings.beginGroup("WindowPosition");
        settings.setValue("x", aWindow->x());
        settings.setValue("y", aWindow->y());
        settings.setValue("width", aWindow->width());
        settings.setValue("height", aWindow->height());
    settings.endGroup();

    return aResult;
}
