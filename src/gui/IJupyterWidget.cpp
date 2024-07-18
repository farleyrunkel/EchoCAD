#include "IJupyterWidget.h"

IJupyterWidget::IJupyterWidget(QWidget* parent) : QScrollArea(parent)
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    myCentralWidget = new QWidget(this);
    setWidget(myCentralWidget);

    myCentralLayout = new QVBoxLayout(myCentralWidget);
    myCentralLayout->setAlignment(Qt::AlignTop);
    myCentralLayout->setContentsMargins(0, 0, 1, 0);
    myCentralLayout->setSpacing(2);
    myCentralWidget->setLayout(myCentralLayout);

    setStyleSheet();
}

void IJupyterWidget::setActiveStyleSheet() {

}

void IJupyterWidget::setStyleSheet() {

    QScrollArea::setStyleSheet(R"(
        IJupyterWidget {
            background-color: #010409;
            border: 0px;
        }
    )");
    myCentralWidget->setStyleSheet(R"(
        QWidget {
            background-color: #010409;
            border: 0px;
        }
    )");
}

void IJupyterWidget::addItem(IJupyterItem* item)
{
    connect(item, &IJupyterItem::itemClicked, [this, item]() {

    // for all item in myCentralLayout
        for (int i = 0; i < myCentralLayout->count(); i++) {
        	IJupyterItem* it = reinterpret_cast<IJupyterItem*>(myCentralLayout->itemAt(i)->widget());
			if (it != item) {
                it->setStyleSheets();
                if (it->isReadOnly()) {
                    it->setCodeStyleSheets();
                }
			}
            else {
                activeItem = item;
                it->setActiveStyleSheets();
            }
        }
        });

    QWidget* p = reinterpret_cast<QWidget*>(item);

    // add item behind active item
    if (activeItem != nullptr) {
		// myCentralLayout->removeWidget(p);
		myCentralLayout->insertWidget(myCentralLayout->indexOf(activeItem) + 1, p);
	}
    else
    {
        myCentralLayout->addWidget(p);
    }

    emit item->itemClicked(item);
}


