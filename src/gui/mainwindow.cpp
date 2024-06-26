#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCalendarWidget>
#include <QScrollArea>
#include "ioverlaywidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    scene(new Scene()),
    gptProcessor(new GPTProcessor(this)),
    executeButton(nullptr)
{
    setupMainUi();
    setupConnects();
}

void MainWindow::setupMainUi() {
    resize(1260, 800);
    auto wid = new IOverlayWidget;
    setCentralWidget(wid);

    auto layout = new QVBoxLayout(wid);

    auto container = QWidget::createWindowContainer(scene);

    wid->setWidget(container);
    wid->setLayout(layout);


   auto w =  createInputLine(this);     w->setFixedHeight(29);
   layout->addWidget(w);

}

void MainWindow::setupConnects() {

    if (executeButton != nullptr)
        connect(executeButton, &QPushButton::clicked, this, &MainWindow::onExecuteButtonClicked);
    connect(gptProcessor, &GPTProcessor::predictionReady, this, &MainWindow::onPredictionReady);
}

void MainWindow::consnect() {


}
QWidget* MainWindow::createInputLine(QWidget* parent) {

    QWidget* widget = new QWidget(parent);
    input = new QLineEdit(widget);
    executeButton = new QPushButton("Execute", widget);

    auto inputLayout = new QHBoxLayout(widget);
    inputLayout->addWidget(input);
    inputLayout->addWidget(executeButton);

    return widget;
}


void MainWindow::onExecuteButtonClicked() {
    QString userInput = input->text();
    gptProcessor->processInput(userInput);
}

void MainWindow::onPredictionReady(const QString &prediction) {
    qDebug() << "Prediction:" << prediction;
    // 这里可以处理预测结果，比如更新UI等
    input->setText(prediction);
}
