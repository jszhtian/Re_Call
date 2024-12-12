#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>
#include<QMovie>

namespace Ui {
class waitDialog;
}

class waitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit waitDialog(QWidget *parent = 0);
    ~waitDialog();



private:
    Ui::waitDialog *ui;
    QMovie *mov;
};

#endif // WAITDIALOG_H
