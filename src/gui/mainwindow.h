#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QApplication>
#include <QSurfaceFormat>
#include <QAction>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QSplitter>
#include <QTextBrowser>

#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <Standard_Version.hxx>

#include <Qsci/qsciscintilla.h>
#include <Qsci/QsciLexerPython.h>

#include "Scene.h"
#include "GPTProcessor.h"
#include "ModelController.h"
#include "ilineedit.h"
#include "PythonInterpreter.h"
#include "IJupyterWidget.h"

namespace echocad
{

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *theParent = nullptr);


    void setPythonInterpreter(PythonInterpreter* thePython);

    ModelController* viewer() const { return mViewer; }

private:

    void setupMainUi(QSplitter* theSplitter);
    void setupPythonEditor(QWidget* theEditor);
    void setupOcctViewer(ModelController* theViewer);
    void setupMenuBar(QMenuBar* theMenuBar);

    void setGPTProcessor(GptProcessor* theGptProcessor);
    void setConnects();
    void setParameters();

    QWidget* createInputLine(QWidget* theParent);

private slots:
    void onExecuteButtonClicked();
    void onPredictionReady(const QString &thePrediction);

    QString extractPythonCode(const QString& theText);

    void executePythonCode(const QString& theCode);

private:
    ILineEdit *mLineEdit;
    GptProcessor *mGptProcessor;

    ModelController* mViewer;
    QSplitter* mSplitter;
    QTextBrowser* mTextBrowser;

    IJupyterWidget* mEditor;

    QPushButton* mSplitterButtons[2];

    PythonInterpreter* mPythonInterpreter;
};


} // end namespace echocad

#endif // MAINWINDOW_H
