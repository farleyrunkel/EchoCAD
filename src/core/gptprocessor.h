#ifndef GPTPROCESSOR_H
#define GPTPROCESSOR_H

#include <QObject>
#include <QString>
#include <QProcess>

class GptProcessor : public QObject {
    Q_OBJECT

public:
    explicit GptProcessor(QObject *parent = nullptr);
    ~GptProcessor();

signals:
    void predictionReady(const QString &prediction);

public slots:
    void processInput(const QString &input);

private slots:
    void handleProcessOutput();

private:
    QProcess *process;
};

#endif // GPTPROCESSOR_H
