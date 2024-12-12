#ifndef MYBUTTON_H
#define MYBUTTON_H


#include "mainwindow.h"
#include <QPainter>
class MyButton : public QWidget
{
    Q_OBJECT
public:

    explicit MyButton(QWidget *parent = nullptr);

    void setImage(QString normal, QString cancel);

protected:

    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void enterEvent(QEvent* ev);
    void leaveEvent(QEvent* ev);
    void paintEvent(QPaintEvent* ev);

signals:
    void clicked();

private:
    QPixmap m_normal;
    QPixmap m_press;
    QPixmap m_cancel;
    QPixmap m_current;
};

#endif // MYBUTTON_H
