#include "filevisualizer.h"
#include "ui_filevisualizer.h"
#include "QCoreApplication"
#include "QMap"
#include "QtAlgorithms"
#include <algorithm>

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

    read_thread->start(); // begin to read with a thread

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
    this->ui->state_textEdit->setText("File reading is finished.");

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


/* function used for comparison*/
bool compare_for_top10(const QPair<int,int> & item1, const  QPair<int,int> & item2)
 {
     return item1.second > item2.second;
 }


/* implement the function: show top 10 POI */
void FileVisualizer::on_top10Button_clicked()
{
    // get user id range
    int user_id_lower = this->ui->user_id_lower->toPlainText().toInt();
    int user_id_upper = this->ui->user_id_upper->toPlainText().toInt();
    int total_length = AllUsers->length();

    QVector<SingleUser> *interested_users = new QVector<SingleUser>;
    QMap<int, int> *poi_counter = new QMap<int, int>;


    // get interesetd users
    // and use map to store
    for(int i=0; i<total_length; ++i)
    {
        SingleUser tmp_usr = AllUsers->at(i);
        if(tmp_usr.user_id>=user_id_lower && tmp_usr.user_id <=user_id_upper)
        {
            interested_users->append(tmp_usr);

            if(poi_counter->contains(tmp_usr.location_id))
            {
                poi_counter->insert(tmp_usr.location_id,  poi_counter->value(tmp_usr.location_id) + 1);
            }
            else
            {
                poi_counter->insert( tmp_usr.location_id,  1);
            }

        }
        else if(tmp_usr.user_id > user_id_upper)
        {
            break;
        }
    }


   /* find the top 10 poi */

    QList<int> keys = poi_counter->keys();

    // pair 1: location id, par 2: count
    QVector<QPair<int, int> > *top10_vector = new QVector<QPair<int, int> > ;

    // all add to vector
    for(int i=0; i<keys.size(); ++i)
    {
        int location_id = keys.at(i);
        int count =  poi_counter->value(keys.at(i));
        top10_vector->append(QPair<int,int>(location_id, count) );
    }

    // sort
    std::sort(top10_vector->begin(), top10_vector->end(), compare_for_top10);

    if (USE_DEBUG)
    {
        print_vector_with_int_pair(top10_vector);
    }

    QVector<QPair<int, int> > *top10_vector_for_draw = new QVector<QPair<int, int> > ;

    if(top10_vector->size()>=10)// more than 10, so we can draw 10
    {
        for(int i=0; i<10; ++i)
        {
            top10_vector_for_draw->append(top10_vector->at(i));
        }
    }
    else // fewer than 10
    {
        for(int i=0; i<top10_vector->size(); ++i)
        {
            top10_vector_for_draw->append(top10_vector->at(i));
        }
    }

    draw_barchart_for_top10(top10_vector_for_draw);


}


void FileVisualizer::draw_barchart_for_top10(QVector<QPair<int, int> > *top10_vector)
{

}
