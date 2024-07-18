#ifndef ILINEEDIT_H
#define ILINEEDIT_H

#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QWidget>

class ILineEdit : public QLineEdit {
    Q_OBJECT
public:
    ILineEdit(QWidget *parent = nullptr);

    QPushButton *leftButton() const;

    QPushButton *rightButton() const;

signals:
    void focused(bool isFocused);

protected:
    void focusInEvent(QFocusEvent* event) override {
        QLineEdit::focusInEvent(event);
        emit focused(true);
    }

    void focusOutEvent(QFocusEvent* event) override {
        QLineEdit::focusOutEvent(event);
        emit focused(false);
    }

private:
    QPushButton *mButtonLeft;
    QPushButton *mButtonRight;
    QLineEdit *m_inputLine;
};

#endif // ILINEEDIT_H
