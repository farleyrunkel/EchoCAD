#include "initializer.h"

void Initializer::initialize(QApplication *app)
{
    setApplicationInfo();
    setEnvironmentVariables();
    loadTranslations();

    // 如果需要其他初始化操作，可以在这里添加
}

void Initializer::setApplicationInfo()
{
    qApp->setApplicationName("EchoCAD");
    qApp->setApplicationDisplayName("EchoCAD");
}

void Initializer::setEnvironmentVariables()
{
    qputenv("QT3D_RENDERER", QByteArray("opengl"));
}

void Initializer::loadTranslations()
{
    static QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "EchoCAD_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName + ".qm")) {
            qApp->installTranslator(&translator);
            break;
        }
    }
}
