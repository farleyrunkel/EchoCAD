#include "mainwindow.h"
#include "../tests/itestwidget.h"
#include <QCalendarWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1280, 800);
    setupMainUi();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMainUi()
{
    setupRibbonBar();

    auto t = new ITestWidget;
    setCentralWidget(t);

    // auto a = new QCalendarWidget(t);
    // a->setGeometry(20, 20, width(), 50);
    // setLayout(new QVBoxLayout);
    // layout()->addWidget(a);

   // t->raise();
}

void MainWindow::setupRibbonBar()
{

}
