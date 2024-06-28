#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "Scene.h"
#include "GPTProcessor.h"

#include "IOcctViewer.h"

#include <Standard_WarningsDisable.hxx>
#include <QApplication>
#include <QSurfaceFormat>

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <Standard_WarningsRestore.hxx>

#include <Standard_Version.hxx>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);



private slots:
    void onExecuteButtonClicked();
    void onPredictionReady(const QString &prediction);

private:

    QLineEdit *input;
    QPushButton *executeButton;
    GPTProcessor *gptProcessor;
    void setupMainUi();
    void setupOcctViewer();
    void setupMenuBar();
    void setupConnects();

    QWidget *createInputLine(QWidget *parent);


    IOcctViewer* myViewer;
};

#endif // MAINWINDOW_H
