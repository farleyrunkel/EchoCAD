#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "Scene.h"
#include "GPTProcessor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onExecuteButtonClicked();
    void onPredictionReady(const QString &prediction);

private:
    Scene *scene;
    QLineEdit *input;
    QPushButton *executeButton;
    GPTProcessor *gptProcessor;
    void setupMainUi();
    void setupConnects();

    QWidget *createInputLine(QWidget *parent);
    void consnect();
};

#endif // MAINWINDOW_H
