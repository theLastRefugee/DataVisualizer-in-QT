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
#include <QPair>

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


class Get2UserThread:public QThread
{
    Q_OBJECT

 signals:
    void changed(int);

public:
    void run() override;

    int user_id1;
    int user_id2;
    // pair的第二个参数表示location id
    QVector<QPair<SingleTime, int> > user_id1_vector;
    QVector<QPair<SingleTime, int> > user_id2_vector;


};


class Get2LocationThread:public QThread
{
    Q_OBJECT

 signals:
    void changed(int);

public:
    void run() override;

    int location_id1;
    int location_id2;

    // 第二个参数表示count
    QMap<SingleTime, int>  location_id1_map;
    QMap<SingleTime, int>  location_id2_map;
};

#endif // THREAD_FUNCTION_H
