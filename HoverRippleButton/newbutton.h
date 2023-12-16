#ifndef NEWBUTTON_H
#define NEWBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

const QColor def(46,22,179);
const QColor hover(247,86,0);

class newButton : public QPushButton
{
public:
    newButton(QString text = nullptr, QWidget *parent=nullptr);
    newButton(QWidget *parent=nullptr);
    ~newButton();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

    void drawBg(QPainter *painter);
    void drawText(QPainter *painter);
    void drawIcon(QPainter *painter);

private:
    bool isHovered = false;
    int textStartX = 0;
    int radius = 10;
    QPoint  mousePoint;//控件内的坐标
};

#endif // NEWBUTTON_H
