#include "systeminstall.h"
#include "ui_systeminstall.h"

/*
 * */
SystemInstall::SystemInstall(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SystemInstall)
{
    ui->setupUi(this);

    QPalette palette;
    QPixmap pixmap("images/back.png");
    palette.setBrush(backgroundRole(),QBrush(pixmap));
    setPalette(palette);
}

SystemInstall::~SystemInstall()
{
    delete ui;
}

