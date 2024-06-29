#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QApplication>
#include <QSurfaceFormat>
#include <QAction>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>

#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <Standard_Version.hxx>

#include "Scene.h"
#include "GPTProcessor.h"
#include "IOcctViewer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupMainUi();
    void setupOcctViewer();
    void setupMenuBar();

    QWidget* createInputLine(QWidget* parent);

private slots:
    void onExecuteButtonClicked();
    void onPredictionReady(const QString &prediction);

private:
    QLineEdit *input;
    QPushButton *executeButton;
    GPTProcessor *gptProcessor;

    IOcctWidget* mViewer;
};

#endif // MAINWINDOW_H
