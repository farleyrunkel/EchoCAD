#ifndef IJUPYTERWIDGET_H
#define IJUPYTERWIDGET_H

#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>

#include <Qsci/qsciscintilla.h>
#include <Qsci/QsciLexerPython.h>

#include "StyleManager.h"

#include "IJupyterItem.h"

class IJupyterWidget : public QScrollArea
{
    Q_OBJECT
public:
    IJupyterWidget(QWidget* parent = nullptr);

    void setStyleSheet();
    void addItem(IJupyterItem* item);

 //   void deleteItem(IJupyterItem* item)
	//{
	//	auto it = std::find(mItems.begin(), mItems.end(), item);
	//	if (it != mItems.end())
	//	{
	//		mItems.erase(it);
	//		delete item;
	//	}

 //       disconnect(item, &IJupyterItem::clicked, 0, 0);
	//}

private:
    std::vector<IJupyterItem*> mItems;

    QWidget * myCentralWidget;
    QVBoxLayout* myCentralLayout;

    IJupyterItem* activeItem;
};


#endif // IJUPYTERWIDGET_H