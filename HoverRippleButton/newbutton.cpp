#include "newbutton.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
newButton::newButton(QString text, QWidget *parent) : QPushButton (text, parent)
{
    setAttribute(Qt::WA_Hover);
    textStartX = 0;
}

newButton::newButton(QWidget *parent) : QPushButton (parent)
{
    setAttribute(Qt::WA_Hover);
    textStartX = 0;
}

newButton::~newButton()
{

}

void newButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawBg(&painter);
    drawText(&painter);//一定要先填充底层颜色然后写文字
    drawIcon(&painter);

}

void newButton::mouseMoveEvent(QMouseEvent *e)
{
    mousePoint = e->pos();
    //qDebug() << mousePoint;
}

void newButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    isHovered = true;
    radius = 10;
    QTimer *timer=new QTimer;//必须使用指针构造，不然会运行一次就释放
    connect(timer, &QTimer::timeout, [=](){
        if(radius < this->width()*20)
        {
            radius += 10;
        }
        update();
    });
    timer->start(50);//悬浮颜色扩散时间
    update();
}
void newButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    isHovered = false;
    radius = 10;//重置
    update();
}

void newButton::drawBg(QPainter *painter)
{
    int width = this->width();
    int height = this->height();
    painter->save();

    painter->setBrush(def);
    QRect rect(0,0,width,height);
    painter->setPen(Qt::NoPen);
    painter->drawRect(rect);
    if(isHovered)
    {
        //painter->setBrush(hover);
        QPoint point = mousePoint;
        QRadialGradient grad(point, radius);
        grad.setColorAt(0, hover);
        grad.setColorAt(0.9, hover);
        grad.setColorAt(1, def);
        painter->setBrush(grad);
        painter->drawEllipse(point, radius, radius);
    }

    painter->restore();
}
void newButton::drawText(QPainter *painter)
{
    painter->save();
    QRect rect(textStartX,0,this->width(),this->height());
    painter->setPen(Qt::white);
    QFont font = painter->font();
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, text());
    painter->restore();
}
void newButton::drawIcon(QPainter *painter)
{
    painter->save();
    QRect rect(0,
               (this->height()-this->iconSize().height())/2,
               this->iconSize().width(),
               this->iconSize().height()
               );
    QPixmap pixmap = this->icon().pixmap(this->iconSize());
    painter->drawPixmap(rect, pixmap);
    painter->restore();
}

