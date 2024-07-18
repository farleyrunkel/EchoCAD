#ifndef IJUPYTERITEM_H
#define IJUPYTERITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QStyleOption>

#include <QFocusEvent>
#include <QResizeEvent>
#include <Qsci/qsciscintilla.h>
#include <Qsci/QsciLexerPython.h>
#include "StyleManager.h"
#include "ilineedit.h"


class IJupyterItem : public QWidget
{
    Q_OBJECT
public:
    enum Type
    {
        Code = 0,
        Note = 1,
        Chat = 2
    };

    IJupyterItem(QWidget* parent = nullptr);
    IJupyterItem(const QString& theText, QWidget* parent = nullptr);
    IJupyterItem(Type theType, const QString& theText = "", QWidget* parent = nullptr);

    void setActiveStyleSheets();

    void setStyleSheets();
    void setCodeStyleSheets();

    bool isReadOnly() const {
		return myLineEdit->isReadOnly();
	}

    QString text() {
        return myLineEdit->text();
    }

signals:
    void textEdited(IJupyterItem* item);
    void itemClicked(IJupyterItem* item);

private:
    void setMainUi();


    void setupQsciScintilla() {
        mEditor = new QsciScintilla(this);
    }

    void paintEvent(QPaintEvent* event) {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

    void mousePressEvent(QMouseEvent* event);

    void setPythonLexer();

private:

    QsciScintilla* mEditor;
    QHBoxLayout* aLayout;

    ILineEdit* myLineEdit;
    QLabel* myColorLabel;

    QLabel* myTagLabel;

    // foreground color
    QString  myColor ;
    // background color
    QColor*  myBackgroundColor ;

    Type myType;
};


#endif //  IJUPYTERITEM_H
