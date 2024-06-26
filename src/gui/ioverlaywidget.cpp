#include "IOverlayWidget.h"

IOverlayWidget::IOverlayWidget(QWidget *parent)
    : QScrollArea(parent)
{
    // Initialize the scroll area properties if needed
    setWidgetResizable(true);
}

void IOverlayWidget::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    if (widget()) {
        widget()->setGeometry(0, 0, viewport()->width(), viewport()->height());
    }
}
