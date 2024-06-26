#ifndef IOVERLAYWIDGET_H
#define IOVERLAYWIDGET_H

#include <QScrollArea>
#include <QWidget>

class IOverlayWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit IOverlayWidget(QWidget *parent = nullptr);

protected:
    // Override resize event to handle size changes
    void resizeEvent(QResizeEvent *event) override;
};

#endif // IOVERLAYWIDGET_H
