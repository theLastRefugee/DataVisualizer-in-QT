#ifndef THREAD_FUNCTION_H
#define THREAD_FUNCTION_H

#include "QFileDialog"
#include "QDebug"
#include "QMessageBox"
#include <QVector>
#include <QPaintEvent>
#include <QTextCodec>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QProgressBar>
#include <filevisualizer.h>

#include <global_variable.h>
#include <timer.h>

class ReadThread:public QThread
        /* read the file */
{
    Q_OBJECT
signals:
    void changed(int);

public:
    QString filename;
    void run() override;


};

class ProgressBarThread:public QThread
        /* show the progress of reading file */
{
    Q_OBJECT

signals:
    void changed(int);

public:
    void run() override;
    QProgressBar *progress_bar;


};

#endif // THREAD_FUNCTION_H
