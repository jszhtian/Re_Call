#include "MyButton.h"


MyButton::MyButton(QWidget *parent) : QWidget(parent)
{

}

void MyButton::setImage(QString normal, QString pressed)
{
    // 加载图片
    m_normal.load(normal);
    m_press.load(pressed);
    m_current = m_normal;
    // 设置按钮和图片大小一致
    setFixedSize(m_normal.size());
}

void MyButton::mousePressEvent(QMouseEvent *ev)
{
    // 鼠标被按下, 发射这个自定义信号
    emit clicked();
    m_current = m_press;
    update();  //调用绘制函数，刷新界面
}

void MyButton::mouseReleaseEvent(QMouseEvent *ev)
{
    m_current = m_normal;
    update();
}


void MyButton::enterEvent(QEvent *ev)
{
    m_current = m_press;
    update();
}

void MyButton::leaveEvent(QEvent *ev)
{
    m_current = m_normal;
    update();
}

void MyButton::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_current);
}
