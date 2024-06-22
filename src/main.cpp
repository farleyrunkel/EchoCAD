#include "gui/initializer.h"
#include "gui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Initializer initializer;
    initializer.initialize(&app);

    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
