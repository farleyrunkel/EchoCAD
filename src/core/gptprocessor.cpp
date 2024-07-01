#include "gptprocessor.h"
#include <QDebug>

GPTProcessor::GPTProcessor(QObject *parent) : QObject(parent) {
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &GPTProcessor::handleProcessOutput);
}

GPTProcessor::~GPTProcessor() {
    process->deleteLater();
}

void GPTProcessor::processInput(const QString &input) {
    QStringList arguments;
    arguments << "C:/Users/95439/Documents/Github/EchoCAD/python/openai_client.py" << input;

    process->start("python3", arguments);
}

void GPTProcessor::handleProcessOutput() {
    QString output = process->readAllStandardOutput().trimmed();
    emit predictionReady(output);
}
  