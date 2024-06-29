
#include <QApplication>
#include <QSurfaceFormat>

#include <Standard_Version.hxx>

#include "MainWindow.h"


int main(int theNbArgs, char** theArgVec)
{
    QApplication aQApp(theNbArgs, theArgVec);

    QCoreApplication::setApplicationName("OCCT Qt Viewer sample");
    QCoreApplication::setOrganizationName("OpenCASCADE");
    QCoreApplication::setApplicationVersion(OCC_VERSION_STRING_EXT);

#ifdef __APPLE__
    // suppress Qt warning "QCocoaGLContext: Falling back to unshared context"
    bool isCoreProfile = true;
    QSurfaceFormat aGlFormat;
    aGlFormat.setDepthBufferSize(24);
    aGlFormat.setStencilBufferSize(8);
    if (isCoreProfile) { aGlFormat.setVersion(4, 5); }
    aGlFormat.setProfile(isCoreProfile ? QSurfaceFormat::CoreProfile : QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(aGlFormat);
#endif

    MainWindow aMainWindow;
    aMainWindow.resize(1250, 800);
    aMainWindow.show();
    return aQApp.exec();
}
