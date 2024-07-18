#include "ilineedit.h"

ILineEdit::ILineEdit(QWidget *parent) : QLineEdit(parent),
									   mButtonLeft(nullptr),
									   mButtonRight(nullptr)
{
    // Set maximum height of the line edit
    setMaximumHeight(40);

    // Create left button and set properties
    mButtonLeft = new QPushButton("", this);
    mButtonLeft->setFixedSize(30, 30); // Set button size
    mButtonLeft->setObjectName("circleButton");

    // Create right button and set properties
    mButtonRight = new QPushButton("", this);
    mButtonRight->setFixedSize(30, 30); // Set button size
    mButtonRight->setObjectName("circleButton");

    // Create horizontal layout
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(5, 0, 5, 0); // Set layout margins
    hLayout->setSpacing(5);                  // Set spacing between widgets

    // Add widgets to layout
    hLayout->addWidget(mButtonLeft);
    hLayout->addStretch(1);
    hLayout->addWidget(mButtonRight);

    leftButton()->hide();
    rightButton()->hide();
    // Set the layout for the line edit
    setLayout(hLayout);
}

// Get left button
QPushButton *ILineEdit::leftButton() const { 
    return mButtonLeft; 
}

// Get right button
QPushButton *ILineEdit::rightButton() const { 
    return mButtonRight; 
}
