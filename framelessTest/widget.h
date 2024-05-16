#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "framelessWidget/framelesswidget.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class Widget;
}
QT_END_NAMESPACE

class Widget : public framelessWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private:
    Ui::Widget* ui;
};
#endif // WIDGET_H
