#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scene(new Scene()) {


    resize(1260, 800);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    input = new QLineEdit(this);
    executeButton = new QPushButton("Execute", this);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(input);
    inputLayout->addWidget(executeButton);

    layout->addWidget(QWidget::createWindowContainer(scene));
    layout->addLayout(inputLayout);

    connect(executeButton, &QPushButton::clicked, this, &MainWindow::onExecuteButtonClicked);

    setCentralWidget(centralWidget);
}

void MainWindow::onExecuteButtonClicked() {
    QString userInput = input->text();
    // 处理用户输入
}
