#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include"infodialog.h"
#include"waitdialog.h"
#include <QDialog>
#include"qfile.h"
#include"qicon.h"
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QProcess>
#include<qdebug>
#include<QStandardPaths>
#include<QSettings>
#include<QSplashScreen>
#include<QtCore>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	//QFileInfo chkbgm(":/icon/00001.ico");
	//if(chkbgm.exists()&& chkbgm.isFile()){QMessageBox::information(NULL,"Info","Icon Exist!", QMessageBox::Ok);;}
    QMediaPlaylist *playlist=new QMediaPlaylist(this);
    playlist->addMedia(QUrl("qrc:/music/setup.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    QMediaPlayer *music = new QMediaPlayer(this);
//    music->setMedia(QUrl("qrc:/bgm/setup.mp3"));
    music->setPlaylist(playlist);
    music->setVolume(50);
    music->play();
    FileDir=tr("C:\\");
    ui->setupUi(this);
    QString checkStr1="Error";
    QSettings existDirx64("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ＰＲＥＴＴＹ×Ｃ∧ＴＩＯＮ_is1",QSettings::NativeFormat);
    QString rawDirx64=existDirx64.value("UninstallString","Error").toString().remove("unins000.exe").remove("\"");
    QSettings existDirx86("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ＰＲＥＴＴＹ×Ｃ∧ＴＩＯＮ_is1",QSettings::NativeFormat);
    QString rawDirx86=existDirx86.value("UninstallString","Error").toString().remove("unins000.exe").remove("\"");
    //qDebug()<<"Check1:"<<checkStr1.compare(rawDirx64)<<endl;
    //qDebug()<<"Check2:"<<checkStr1.compare(rawDirx86)<<endl;
    if(!(checkStr1.compare(rawDirx64)&&checkStr1.compare(rawDirx86)))
    {
        QMessageBox::information(NULL,"Info","Auto Detect Fail! Please set directory manually.", QMessageBox::Ok);
    }
    if(rawDirx64!="Error")
    {
        FileDir=rawDirx64;
    }
    if(rawDirx86!="Error")
    {
        FileDir=rawDirx86;
    }
    Qt::WindowFlags flags=0;
    flags|=Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setFixedSize(960,540);
    setWindowTitle("安装程序");
    setWindowIcon(QIcon(":/icon/00001.ico"));
//   QFileInfo checkico(":/icon/00001.ico");
//   if(checkico.exists()&&checkico.isFile()){QMessageBox::information(NULL,"Info","Icon Exist!", QMessageBox::Ok);;}
    QPixmap pix;
    pix.load(":/image/bbrd1.bmp");
    ui->main_display->setPixmap(pix.scaled(650,360,Qt::KeepAspectRatio));
    ui->lineEdit->setText(FileDir);
    QPixmap qbut;
    qbut.load(":/icon/00001.ico");
    ui->pushButton_Info->setIcon(qbut);
    ui->pushButton_Info->setIconSize(qbut.rect().size());
}

bool fileExists(QString path)
{
    QFileInfo check_file(path);
    if(check_file.exists()&&check_file.isFile())
    {return true;}
    else
    {return false;}
}

MainWindow::~MainWindow()
{
//    music.stop();
    delete ui;
}

void MainWindow::on_pushButton_Info_clicked()
{
    InfoDialog dlg;
        dlg.show();
        dlg.exec();
}

void MainWindow::on_pushButton_Exit_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushButton_select_clicked()
{
    //FileDir=QFileDialog::getOpenFileName(this, "Select a file to open...", QDir::homePath());
    QFileDialog* openFilePath=new QFileDialog(this,"Select target directory...",FileDir);
    openFilePath->setFileMode(QFileDialog::DirectoryOnly);
    bool filecheck=false;
    do
    {
        FileDir=openFilePath->getExistingDirectory();
        QString tmp;
        tmp=FileDir+"/ＰＲＥＴＴＹ×Ｃ∧ＴＩＯＮ.exe";
        filecheck=fileExists(tmp);
        if((!filecheck)&&(FileDir!=NULL)){QMessageBox::warning(this,"Warning","File not found!", QMessageBox::Ok);}
        if((FileDir==NULL)){break;}
    }
    while(filecheck==false);
    ui->lineEdit->setText(FileDir.replace("/","\\"));
}



void MainWindow::on_pushButton_Install_clicked()
{
    QString tmp;
    bool existcheck=true;
    bool dircheck=true;
    bool setupflag=true;
    tmp=FileDir+"/ＰＲＥＴＴＹ×Ｃ∧ＴＩＯＮ.exe";
    QString check1="uncompcore.exe";
    QString check2="comp.7z";
    QStringList arguments;
    if(!fileExists(tmp)){QMessageBox::warning(this,"Warning","File Not found!", QMessageBox::Ok);setupflag=false;dircheck=false;}
    if(!fileExists(check1)){QMessageBox::warning(this,"Warning","Uncompress Core not found!", QMessageBox::Ok); existcheck=false;setupflag=false;}
    if(!fileExists(check2)){QMessageBox::warning(this,"Warning","Resource file not found!", QMessageBox::Ok); existcheck=false;setupflag=false;}
    if(existcheck==true&&dircheck==true)
    {
        arguments<<check2<<FileDir.replace("/","\\");
        //QMessageBox::information(NULL,"INFO","Path"+FileDir,QMessageBox::Ok);
        //QMessageBox::information(NULL,"INFO","Exec"+check1,QMessageBox::Ok);
        //QMessageBox::information(NULL,"INFO","Comp"+check2,QMessageBox::Ok);
        QProcess *myProcess=new QProcess(this);
        //QMessageBox::information(NULL,"INFO","Start",QMessageBox::Ok);
        myProcess->start(check1,arguments);
        waitDialog *dlg=new waitDialog(this);
        dlg->setWindowModality(Qt::NonModal);
        dlg->show();
        do
        {
            QApplication::processEvents();
        }
        while(myProcess->atEnd());
        myProcess->waitForFinished();
        //myProcess->waitForFinished();
        dlg->close();
        //QMessageBox::information(this,"Info","Exit code: "+QString::number(myProcess->exitCode()),QMessageBox::Ok);
        if(myProcess->exitCode()!=0)
        {
            QMessageBox::warning(this,"Last Error",QString::fromLocal8Bit(myProcess->readAllStandardError()),QMessageBox::Ok);
            QMessageBox::information(this,"Info",QString::fromLocal8Bit(myProcess->readAllStandardOutput()),QMessageBox::Ok);
            QMessageBox::information(this,"Info","Exit code: "+QString::number(myProcess->exitCode()),QMessageBox::Ok);
            setupflag=false;
        }

    }
    else
    {
        setupflag=false;
    }
    if(setupflag==false)
    {
        QMessageBox::warning(this,"Warning","Installation failed!", QMessageBox::Ok); existcheck=false;
    }
    else
    {
         existcheck=false;
        if(QMessageBox::question(this,"INFO","Create shortcut on desktop?", QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        {
            //QMessageBox::information(this,"INFO","Start Create shortcut!", QMessageBox::Ok);
            QString shortcut;
            shortcut=FileDir+"/KRLoader.exe";
            if(!fileExists(shortcut)){QMessageBox::warning(this,"Warning","Something Wrong! Application will be terminated!", QMessageBox::Ok);QApplication::exit();}

            QFile::link(shortcut, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append("PxC_GBK.lnk"));
            //QFile::link(shortcut, "pxc.lnk");
        }
        if(QMessageBox::information(this,"INFO","Installation Success!", QMessageBox::Ok)==QMessageBox::Ok)
        {
            QApplication::exit();
        }
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    FileDir=arg1;
}
