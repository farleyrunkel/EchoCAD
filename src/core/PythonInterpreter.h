#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include <QObject> 
#include <QString>

#include <pybind11/embed.h>
#include <QStringList>

namespace py = pybind11;

class PythonInterpreter : public QObject {
    Q_OBJECT

public:
    PythonInterpreter();
    ~PythonInterpreter();

    void exportDLL(const QStringList& theDll);

    void executeScript(const QString& script);

signals:
	void logMessage(const QString& theMessage);
    void dllNeeded(const QStringList& theDll);


private:
    py::scoped_interpreter guard;

};

#endif // PYTHONINTERPRETER_H
