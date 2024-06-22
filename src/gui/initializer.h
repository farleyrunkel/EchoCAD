#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QByteArray>

class Initializer
{
public:
    void initialize(QApplication *app = qApp);

private:
    void setApplicationInfo();
    void setEnvironmentVariables();
    void loadTranslations();
};

#endif // INITIALIZER_H
