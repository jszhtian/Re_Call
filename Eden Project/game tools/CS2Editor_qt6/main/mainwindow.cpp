#include "mainwindow.h"
#include "ui_mainwindow.h"
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
QStandardItemModel *RAWString_Model = new QStandardItemModel();
QStandardItemModel *NEWString_Model = new QStandardItemModel();
QString OpenedFile;
void Init()
{
	RAWString_Model->clear();
	NEWString_Model->clear();
	RAWString_Model->setHorizontalHeaderItem(0, new QStandardItem(QString("原始文本")));
	NEWString_Model->setHorizontalHeaderItem(0, new QStandardItem(QString("翻译文本")));
}

QStringList *ExportRAWList(QStandardItemModel *Model)
{
    QStringList* result=new QStringList;
    int count=Model->rowCount();
    for(int i=0;i<count;++i)
    {
        result->append("//"+Model->item(i)->text());
    }
    return result;
}

QStringList *ExportNEWList(QStandardItemModel *Model)
{
    QStringList* result=new QStringList;
    int count=Model->rowCount();
    for(int i=0;i<count;++i)
    {
        result->append(Model->item(i)->text());
    }
    return result;
}


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
    ui->RawtableView->setModel(RAWString_Model);
    ui->TranstableView->setModel(NEWString_Model);
    ui->RawtableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->RawtableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TranstableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->RawtableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TranstableView->setSelectionMode(QAbstractItemView::SingleSelection);
    QObject::connect(ui->TranstableView->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->RawtableView->verticalScrollBar(), SLOT(setValue(int)));
    QObject::connect(ui->RawtableView->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->TranstableView->verticalScrollBar(), SLOT(setValue(int)));
    ui->TranstableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::AddRecoderForRAW(QString txt)
{
	RAWString_Model->appendRow(new QStandardItem(txt));
}

void MainWindow::AddRecoderForTRANS(QString txt)
{
	NEWString_Model->appendRow(new QStandardItem(txt));
}




void MainWindow::on_actionOpen_triggered()
{
    Init();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("打开文件");
    fileDialog->setNameFilter("CST文件(*.cst)");
    fileDialog->setDirectory(".");
    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path=fileDialog->selectedFiles()[0];
        OpenedFile=path;
        bool isOK;
        QString cp = QInputDialog::getText(NULL, "CodePage",
                                                           "输入代码页(Shift-JIS,GB18030-0,UTF-8,UTF-16 etc...)",
                                                           QLineEdit::Normal,
                                                           "Shift-JIS",
                                                           &isOK);
        if(!isOK)
        {
            QMessageBox::critical(NULL, "Error", "未输入代码页！");
            return;
        }
        QStringList *result=CSTOpen(path,cp);
        if(result==nullptr)
        {
            QMessageBox::critical(NULL, "Error", "发生错误!");
        }
		for (QStringList::const_iterator iter = result->constBegin(); iter != result->constEnd(); ++iter)
		{
			AddRecoderForRAW((*iter));
			AddRecoderForTRANS((*iter));
		}
		ui->RawtableView->resizeColumnsToContents();
		ui->TranstableView->resizeColumnsToContents();
		if (result != nullptr)
		{
			delete result;
		}
        setWindowTitle("CSTFile:"+OpenedFile);
        
    }
}

void MainWindow::on_TranstableView_clicked(const QModelIndex &index)
{
    ui->RawtableView->selectRow(index.row());
    ui->RawlineEdit->setText((RAWString_Model->item(index.row()))->text());
    if(((RAWString_Model->item(index.row()))->text())!=((NEWString_Model->item(index.row()))->text()))
    {
        ui->TranslineEdit->setText((NEWString_Model->item(index.row()))->text());
    }
    else
    {
        ui->TranslineEdit->clear();
    }
}

void MainWindow::on_RawtableView_clicked(const QModelIndex &index)
{
    ui->TranstableView->selectRow(index.row());
    ui->RawlineEdit->setText((RAWString_Model->item(index.row()))->text());
    if(((RAWString_Model->item(index.row()))->text())!=((NEWString_Model->item(index.row()))->text()))
    {
        ui->TranslineEdit->setText((NEWString_Model->item(index.row()))->text());
    }
    else
    {
        ui->TranslineEdit->clear();
    }
}

void MainWindow::on_RawtableView_activated(const QModelIndex &index)
{
    ui->TranstableView->selectRow(index.row());
    ui->RawlineEdit->setText((RAWString_Model->item(index.row()))->text());
    if(((RAWString_Model->item(index.row()))->text())!=((NEWString_Model->item(index.row()))->text()))
    {
        ui->TranslineEdit->setText((NEWString_Model->item(index.row()))->text());
    }
    else
    {
        ui->TranslineEdit->clear();
    }
}

void MainWindow::on_TranstableView_activated(const QModelIndex &index)
{
    ui->RawtableView->selectRow(index.row());
    ui->RawlineEdit->setText((RAWString_Model->item(index.row()))->text());
    if(((RAWString_Model->item(index.row()))->text())!=((NEWString_Model->item(index.row()))->text()))
    {
        ui->TranslineEdit->setText((NEWString_Model->item(index.row()))->text());
    }
    else
    {
        ui->TranslineEdit->clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QItemSelectionModel *select = ui->TranstableView->selectionModel();
    int SelectTarget=-1;
    if(select->hasSelection())
    {
        SelectTarget=select->selectedRows().at(0).row();
    }
    QString text=ui->TranslineEdit->text();
    if (SelectTarget!=-1&&!text.isEmpty())
    {
        NEWString_Model->setItem(SelectTarget,new QStandardItem(text));
    }
    ui->TranstableView->resizeColumnsToContents();
    ui->TranstableView->selectRow(SelectTarget);
}

void MainWindow::on_TranslineEdit_returnPressed()
{
    QItemSelectionModel *select = ui->TranstableView->selectionModel();
    int SelectTarget=-1;
    if(select->hasSelection())
    {
        SelectTarget=select->selectedRows().at(0).row();
    }
    QString text=ui->TranslineEdit->text();
    if (SelectTarget!=-1&&!text.isEmpty())
    {
        NEWString_Model->setItem(SelectTarget,new QStandardItem(text));
    }
    ui->TranstableView->resizeColumnsToContents();
    ui->TranstableView->selectRow(SelectTarget);
    if(SelectTarget+1<ui->TranstableView->model()->rowCount())
    {
        ui->TranstableView->selectRow(SelectTarget+1);
        ui->RawtableView->selectRow(SelectTarget+1);
        ui->RawlineEdit->setText((RAWString_Model->item(SelectTarget+1))->text());
        if(((RAWString_Model->item(SelectTarget+1))->text())!=((NEWString_Model->item(SelectTarget+1))->text()))
        {
            ui->TranslineEdit->setText((NEWString_Model->item(SelectTarget+1))->text());
        }
        else
        {
            ui->TranslineEdit->clear();
        }

    }
    else
    {
        QMessageBox::information(NULL,"信息","已到达列表结尾",QMessageBox::Ok);
    }
}

void MainWindow::on_actionSave_triggered()
{
    QStringList* newtext=ExportNEWList(NEWString_Model);
    bool res=CSTSave(OpenedFile,newtext);
    if(res!=true)
    {
        QMessageBox::critical(NULL, "Error", "发生错误!");
    }
    delete newtext;
}

void MainWindow::on_actionOutport_triggered()
{
    if(RAWString_Model->rowCount()==0)
        {
            QMessageBox::information(NULL, "导出", "你没有打开文件！");
            return;
        }
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle("导出文件");
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->setNameFilter("UTF-8纯文本(*.txt)");
        fileDialog->setDirectory(".");
        if(fileDialog->exec() == QDialog::Accepted)
        {
             QString path=fileDialog->selectedFiles()[0];
             QStringList* NEWList=ExportNEWList(NEWString_Model);
             QStringList* RAWList=ExportRAWList(RAWString_Model);
             ExportUTF8(RAWList,NEWList,path);
             QMessageBox::information(NULL, "导出", "导出完成！");
             delete NEWList;
             delete RAWList;
        }
        else
        {
            QMessageBox::information(NULL, "导出", "你没有导出任何文件！");
        }
        delete fileDialog;
}

QStringList *GetRAWStringList(QStringList *RAWlist)
{
    QStringList* result=new QStringList;
    QStringList::const_iterator iter;
    for(iter=RAWlist->constBegin();iter!=RAWlist->constEnd();++iter)
    {
        if((*iter).length()==0)
        {
            continue;
        }
        if((*iter).indexOf("@")==0)
        {
            continue;
        }
        if((*iter).indexOf("//")==0)
        {
            result->append((*iter).mid(2));
        }
    }
    return result;
}

QStringList *GetNEWStringList(QStringList *RAWlist)
{
    QStringList* result=new QStringList;
    QStringList::const_iterator iter;
    for(iter=RAWlist->constBegin();iter!=RAWlist->constEnd();++iter)
    {
        if((*iter).length()==0)
        {
            continue;
        }
        if((*iter).indexOf("@")==0)
        {
            continue;
        }
        if((*iter).indexOf("//")==0)
        {
            continue;
        }
        result->append(*iter);
    }
    return result;
}

void MainWindow::on_actionImport_triggered()
{
    if(RAWString_Model->rowCount()==0)
        {
            QMessageBox::information(NULL, "导入", "你没有打开文件！");
            return;
        }
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setWindowTitle("导入文件");
        fileDialog->setNameFilter("UTF-8纯文本(*.txt)");
        fileDialog->setDirectory(".");
        if(fileDialog->exec() == QDialog::Accepted)
        {
            QString path=fileDialog->selectedFiles()[0];
            QStringList* list=utf8textproc(path);
            QStringList* NEWList=GetNEWStringList(list);
            QStringList* RAWList=GetRAWStringList(list);
            int count=NEWList->size();
            if(NEWList->size()!=RAWList->size())
            {
                QMessageBox::critical(NULL,"导入","导入文本量不等，请检查文本数据！");
            }
            else
            {
                for(int i=0;i<count;++i)
                {
                    if(((RAWString_Model->item(i))->text())==(RAWList->at(i)))
                    {
                        NEWString_Model->setItem(i,new QStandardItem(NEWList->at(i)));
                    }
                }
                QMessageBox::information(NULL, "导入", "导入完成！");
            }
            delete NEWList;
            delete RAWList;
        }
        delete fileDialog;
}

void MainWindow::on_action_Export_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("打开文件");
    fileDialog->setNameFilter("CST文件(*.cst)");
    fileDialog->setDirectory(".");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    if(fileDialog->exec() == QDialog::Accepted)
    {
        bool isOK;
        QString cp = QInputDialog::getText(NULL, "CodePage","输入代码页(Shift-JIS,GB18030-0,UTF-8,UTF-16 etc...)",QLineEdit::Normal,"Shift-JIS",&isOK);
        if(!isOK)
        {
            QMessageBox::critical(NULL, "Error", "未输入代码页！");
            return;
        }
        QStringList Filelist=fileDialog->selectedFiles();
        for(QString Name : Filelist)
        {
            QStringList *result=CSTOpen(Name,cp);
            QStringList *raw=new QStringList;
            for(auto itr=result->begin();itr!=result->end();++itr)
            {
                raw->append("//"+*itr);
            }
            ExportUTF8(raw,result,Name+".txt");
            delete result;
            delete raw;
        }

    }
	QMessageBox::information(NULL,"Info","操作完成！");
    delete fileDialog;
}

void MainWindow::on_action_Import_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("打开文件");
    fileDialog->setNameFilter("CST文件(*.cst)");
    fileDialog->setDirectory(".");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    if(fileDialog->exec() == QDialog::Accepted)
    {
        QStringList Filelist=fileDialog->selectedFiles();
        for(QString Name : Filelist)
        {
            QStringList* list=utf8textproc(Name+".txt");
            QStringList* NEWList=GetNEWStringList(list);
            bool res=CSTSave(Name,NEWList);
            if(res!=true)
            {
                QMessageBox::critical(NULL, "Error", "发生错误!");
            }
        }

    }
	QMessageBox::information(NULL, "Info", "操作完成！");
    delete fileDialog;
}
