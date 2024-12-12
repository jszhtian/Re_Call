#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    setWindowTitle("About");
    setWindowIcon(QIcon(":/icon/00001.ico"));
    setFixedSize(400,300);
    QPixmap pix;
    pix.load(":/image/about.bmp");
    ui->about_display->setPixmap(pix.scaled(380,214,Qt::KeepAspectRatio));
}

InfoDialog::~InfoDialog()
{
    delete ui;
}
