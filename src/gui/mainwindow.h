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

#include <Standard_WarningsDisable.hxx>
#include <Standard_WarningsRestore.hxx>
#include <Standard_Version.hxx>

#include <Qsci/qsciscintilla.h>
#include <Qsci/QsciLexerPython.h>

#include "Scene.h"
#include "GPTProcessor.h"
#include "IOcctViewer.h"
#include "ilineedit.h"
#include "PythonInterpreter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *theParent = nullptr);

    void setPythonInterpreter(PythonInterpreter* thePython);

    IOcctWidget* viewer() const { return mViewer; }

private:
    void setConnects();
    void setupMainUi(QSplitter* theSplitter);
    void setupPythonEditor(QWidget* theEditor);
    void setupOcctViewer(IOcctWidget* theViewer);
    void setupMenuBar(QMenuBar* theMenuBar);

    void setGPTProcessor(GptProcessor* theGptProcessor);

    QWidget* createInputLine(QWidget* theParent);

private slots:
    void onExecuteButtonClicked();
    void onPredictionReady(const QString &thePrediction);

    QString extractPythonCode(const QString& theText);

    void executePythonCode(const QString& theCode);

private:
    ILineEdit *mLineEdit;
    GptProcessor *mGptProcessor;

    IOcctWidget* mViewer;
    QSplitter* mSplitter;

    QsciScintilla* mEditor;

    QPushButton* mSplitterButtons[2];

    PythonInterpreter* mPythonInterpreter;
};

#endif // MAINWINDOW_H
