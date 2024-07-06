#include "gptprocessor.h"
#include <QDebug>

GptProcessor::GptProcessor(QObject *parent) : QObject(parent) {
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &GptProcessor::handleProcessOutput);
}

GptProcessor::~GptProcessor() {
    process->deleteLater();
}

void GptProcessor::processInput(const QString &input) {
    QStringList arguments;
    arguments << "C:/Users/95439/Documents/Github/EchoCAD/python/openai_client.py" << input;

    process->start("python3", arguments);
}

void GptProcessor::handleProcessOutput() {
    QString output = process->readAllStandardOutput().trimmed();
    emit predictionReady(output);
}
  