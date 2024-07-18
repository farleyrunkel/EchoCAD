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

    void importDLL(const QStringList& theDll);

    void loadModule(const QString& pydPath);

    void executeScript(const QString& script);

    void setSysVariable(const QString& name, py::object value);

signals:
    void executed(const QString& theResult);

	void logMessage(const QString& theMessage);
    void dllNeeded(const QStringList& theDll);

    void moduleNeeded(const QString& theDll);

private:
    py::scoped_interpreter guard;

};

#endif // PYTHONINTERPRETER_H
