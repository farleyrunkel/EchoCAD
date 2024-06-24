#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scene.h"
#include <QLineEdit>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onExecuteButtonClicked();

private:
    Scene *scene;
    QLineEdit *input;
    QPushButton *executeButton;
};

#endif // MAINWINDOW_H
