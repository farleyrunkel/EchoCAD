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
    myCentralLayout->setContentsMargins(0, 0, 0, 0);
    myCentralLayout->setSpacing(2);
    myCentralWidget->setLayout(myCentralLayout);

    setStyleSheet();
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
    //connect(item, &IJupyterItem::clicked, [this, item]() {
    //        activeItem = item;
    //    });

    QWidget* p = reinterpret_cast<QWidget*>(item);
    myCentralLayout->addWidget(p);
    mItems.push_back(item);
}
