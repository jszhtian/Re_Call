#ifndef SYSTEMINSTALL_H
#define SYSTEMINSTALL_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SystemInstall; }
QT_END_NAMESPACE

class SystemInstall : public QMainWindow
{
    Q_OBJECT

public:
    SystemInstall(QWidget *parent = nullptr);
    ~SystemInstall();

private:
    Ui::SystemInstall *ui;
};
#endif // SYSTEMINSTALL_H
