#ifndef IJUPYTERITEM_H
#define IJUPYTERITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include <QMouseEvent>
#include <QResizeEvent>
#include <Qsci/qsciscintilla.h>
#include <Qsci/QsciLexerPython.h>
#include "StyleManager.h"

class IJupyterItem : public QWidget
{
    Q_OBJECT
public:
    IJupyterItem(QWidget* parent = nullptr);

private:
    void setMainUi();

    void setStyleSheets();

    void setupQsciScintilla() {

        mEditor = new QsciScintilla(this);
    }

    void resizeEvent(QResizeEvent* event) {
        QWidget::resizeEvent(event);
        updateGeometry();
    }

    void updateGeometry() {
        //// Set the text width of the document to match the width of the text browser
        //mEditor->document()->setTextWidth(width());

        //// Get the document height and set it as the fixed height of the text browser
        //qreal docHeight = document()->size().height();
        //setFixedHeight(static_cast<int>(docHeight));

        //// Call the base class updateGeometry() method
        //QWidget::updateGeometry();
    }

    void setPythonLexer();

private:

    QsciScintilla* mEditor;
    QHBoxLayout* aLayout;

    QLineEdit* myLineEdit;
    QLabel* myColorLabel;

    QLabel* myTagLabel;
};


#endif //  IJUPYTERITEM_H
