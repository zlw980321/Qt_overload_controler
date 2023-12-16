#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include "newbutton.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
     * 1.要从底层开始设置鼠标追踪
     * 2.ui文件中把pushButton提升为newButton类（newButton为自定义QPushButton继承类）
     */
    this->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->pushButton->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

