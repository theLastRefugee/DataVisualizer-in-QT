#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "QMessageBox"
#include <QVector>
#include <QPaintEvent>
#include <QTextCodec>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    my_file_visualizer = new FileVisualizer(this);
    my_file_visualizer->hide();// hide initially

}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_pushButton_clicked()
{
    QStringList infoList;

    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("open a file."),
                "",
                tr("table files (*.csv)"));


    if (fileName.isEmpty())
           {
               QMessageBox::warning(this, "Warning!", "Fail to open the table file!", QMessageBox::Yes);
           }
    else
    {

    give_filename_to_file_manager(fileName);

    this->ui->textEdit->setText(fileName);

    }

}


void MainWindow::on_exitButton_clicked()
{
    exit(0);
}


void MainWindow::on_checkBox_longitude_stateChanged(int arg1)
{
    use_longitude = this->ui->checkBox_longitude->isChecked();
}


void MainWindow::on_checkBox_locationid_stateChanged(int arg1)
{
    use_location_id = this->ui->checkBox_locationid->isChecked();
    if(USE_DEBUG)
    {
        qDebug()<<"location id box checked";
    }

}



void MainWindow::on_checkBox_time_stateChanged(int arg1)
{
    use_time = this->ui->checkBox_time->isChecked();
}




void MainWindow::on_checkBox_latitude_stateChanged(int arg1)
{
    use_latitude = this->ui->checkBox_latitude->isChecked();
}


void MainWindow::on_readButton_clicked()
{
     //循环读取txt并打印
     //使用thread读取文件
    this->hide(); // first hide
    my_file_visualizer->read_file();
}

