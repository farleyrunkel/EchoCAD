#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), scene(new Scene()), gptProcessor(new GPTProcessor(this))
{
    resize(1260, 800);
    auto wid = new QWidget;
    setCentralWidget(wid);

    auto layout = new QVBoxLayout(wid);

    auto container = QWidget::createWindowContainer(scene);
    layout->addWidget(container);

    input = new QLineEdit(this);
    executeButton = new QPushButton("Execute", this);

    auto inputLayout = new QHBoxLayout;
    inputLayout->addWidget(input);
    inputLayout->addWidget(executeButton);

    layout->addLayout(inputLayout);

    connect(executeButton, &QPushButton::clicked, this, &MainWindow::onExecuteButtonClicked);
    connect(gptProcessor, &GPTProcessor::predictionReady, this, &MainWindow::onPredictionReady);
}

void MainWindow::onExecuteButtonClicked() {
    QString userInput = input->text();
    gptProcessor->processInput(userInput);
}

void MainWindow::onPredictionReady(const QString &prediction) {
    qDebug() << "Prediction:" << prediction;
    // 这里可以处理预测结果，比如更新UI等
}
