#include "filevisualizer.h"
#include "ui_filevisualizer.h"
#include "QCoreApplication"

FileVisualizer::FileVisualizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileVisualizer)
{
    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);

    ui->progressBar->setValue(1);

}

FileVisualizer::~FileVisualizer()
{
    delete ui;
}


void FileVisualizer::read_file()
{
    this->show();
    this->repaint();

    QCoreApplication::processEvents();

    ReadThread *read_thread = new ReadThread;

    read_thread->filename = filename; // give file name to read

    // window for progress

    this->ui->progressBar->setValue(0);

    read_thread->start(); // begin to read

    float i=1;

    while(FileLineCount <= FileLineTotal)
    {

            float progress_ratio = float(FileLineCount) / float(FileLineTotal) * 100;

//            qDebug()<<"progress:"<<progress_ratio;

        this->ui->progressBar->setValue(progress_ratio);
        this->ui->progressBar->update();

        QCoreApplication::processEvents();
        i++;


        if(USE_DEBUG)
        {
            if (FileLineCount%10000==0)
            {
                qDebug()<<"progress bar output:"<<FileLineCount;
            }
        }

           if(FileLineCount==FileLineTotal)
           {
               break;
           }
    }
    this->ui->progressBar->setValue(100);
    this->ui->progressBar->update();


    read_thread->wait();

    if(USE_DEBUG)
    {
        qDebug()<<"file read finsihed";
    }
    FileLineCount = 0;

}


void FileVisualizer::on_exitButton_clicked()
{
    this->close();
}

