#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qstandarditemmodel.h"
#include "QString"
#include "qlineedit.h"
#include "qstandarditemmodel.h"
#include "qdebug.h"
#include <QScrollBar>
#include "QMessageBox"
#include "QFileDialog"
#include <QShortcut>
#include <QInputDialog>
#include "procfile.h"
#include <QTextStream>
namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void AddRecoderForRAW(QString txt);
	void AddRecoderForTRANS(QString txt);
private slots:
    void on_actionOpen_triggered();

    void on_TranstableView_clicked(const QModelIndex &index);

    void on_RawtableView_clicked(const QModelIndex &index);

    void on_RawtableView_activated(const QModelIndex &index);

    void on_TranstableView_activated(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_TranslineEdit_returnPressed();

    void on_actionSave_triggered();

    void on_actionOutport_triggered();

    void on_actionImport_triggered();

    void on_action_Export_triggered();

    void on_action_Import_triggered();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
