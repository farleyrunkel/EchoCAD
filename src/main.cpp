
#include "gui/initializer.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    Initializer initializer;
    initializer.initialize(&application);

    MainWindow mainWindow;

    mainWindow.show();

    return application.exec();
}

