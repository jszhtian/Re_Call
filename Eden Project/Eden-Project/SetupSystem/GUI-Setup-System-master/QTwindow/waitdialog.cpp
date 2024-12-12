#include "waitdialog.h"
#include "ui_waitdialog.h"

waitDialog::waitDialog(QWidget *parent) :
    QDialog(parent,Qt::FramelessWindowHint),
    ui(new Ui::waitDialog)
{
    ui->setupUi(this);
    setWindowTitle("About");
    setWindowIcon(QIcon(":/icon/00001.ico"));
    setFixedSize(493,227);
    mov=new QMovie(":/image/wait.gif");
    mov->setScaledSize(QSize(100,100));
    ui->Movie_label->setMovie(mov);
    mov->start();
}

waitDialog::~waitDialog()
{
    delete ui;
    delete mov;
}
