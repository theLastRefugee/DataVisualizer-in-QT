#include "filevisualizer.h"
#include "ui_filevisualizer.h"
#include "QCoreApplication"
#include "QMap"
#include "QtAlgorithms"
#include <algorithm>

#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>

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


/*
 * function: show top 10 POI
 * constrain: user id
*/
void FileVisualizer::on_top10Button_clicked()
{
    // get user id range
    int user_id_lower = this->ui->user_id_lower->toPlainText().toInt();
    int user_id_upper = this->ui->user_id_upper->toPlainText().toInt();

    qDebug()<<"origin lower user id:"<<user_id_lower;
    qDebug()<<"origin upper user id:"<<user_id_upper;


    //value constrain
    user_id_lower = qMax(user_lower, user_id_lower);
    user_id_lower = qMin(user_upper, user_id_lower);

    user_id_upper = qMax(user_lower, user_id_upper);
    user_id_upper = qMin(user_upper, user_id_upper);


    int total_length = AllUsers->length();

    qDebug()<<"total lower user id:"<<user_lower;
    qDebug()<<"total upper user id:"<<user_upper;
    qDebug()<<"lower user id:"<<user_id_lower;
    qDebug()<<"upper user id:"<<user_id_upper;

    QVector<SingleUser> *interested_users = new QVector<SingleUser>;
    // 第一个int表示location id，第二个int表示count，访问数量
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

    draw_barchart_for_top10(top10_vector_for_draw, user_id_lower, user_id_upper);

    qDebug()<<"show finished";

    this->ui->state_textEdit->setText("Top 10 POIs are shown successfully.");
    QCoreApplication::processEvents();
}


void FileVisualizer::draw_barchart_for_top10(QVector<QPair<int, int> > *top10_vector, int user_id_lower, int user_id_upper)
{
    int total_length = top10_vector->length();

    qDebug()<<"Top"+QString::number(total_length);
    QBarSet *set0 = new QBarSet("Top"+QString(total_length));
//        QBarSet *set1 = new QBarSet("John");
//        QBarSet *set2 = new QBarSet("Axel");
//        QBarSet *set3 = new QBarSet("Mary");
//        QBarSet *set4 = new QBarSet("Samantha");

    for(int i=0; i < total_length; ++i)
    {
        *set0 << top10_vector->at(i).second;
    }
//        *set1 << 5 << 0 << 0 << 4 << 0 << 7;
//        *set2 << 3 << 5 << 8 << 19<< 8 << 5;
//        *set3 << 5 << 6 << 7 << 3 << 4 << 5;
//        *set4 << 9 << 7 << 5 << 3 << 1 << 2;



        QBarSeries *series = new QBarSeries();
            series->append(set0);
//            series->append(set1);
//            series->append(set2);
//            series->append(set3);
//            series->append(set4);

            QChart *chart = new QChart();
                chart->addSeries(series);
                if(user_id_lower!=user_id_upper)
                {
                chart->setTitle("Top " +QString::number(total_length) + " Point-of-Interests (POIs) for User "+QString::number(user_id_lower)+" to "+QString::number(user_id_upper)+" !");
                }
                else
                {
                   chart->setTitle("Top " +QString::number(total_length) + " Point-of-Interests (POIs) for User "+QString::number(user_id_lower)+ " !");
                }
                chart->setAnimationOptions(QChart::SeriesAnimations);



        QStringList categories;

        for(int i=0; i < total_length; ++i)
        {
            //这是横坐标
            int location_id =top10_vector->at(i).first;

            categories << QString::number(location_id);
        }

            QBarCategoryAxis *axis = new QBarCategoryAxis();
            axis->append(categories);
            axis->setTitleText("location id");

            chart->createDefaultAxes();//创建默认的左侧的坐标轴（根据 QBarSet 设置的值）
            chart->axisY()->setTitleText("visit counts");

            chart->setAxisX(axis, series);//设置坐标轴


            chart->legend()->setVisible(true); //设置图例为显示状态
            chart->legend()->setAlignment(Qt::AlignBottom);//设置图例的显示位置在底部


            // 画出来
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setObjectName("drawing_chart");

            //放在画布上
            this->ui->drawingLayout->addWidget(chartView);


            QCoreApplication::processEvents();



}


void FileVisualizer::on_clearButton_clicked()
{

        QLayoutItem *child;
        while ((child = this->ui->drawingLayout->takeAt(0)) != 0) {
            delete child;
        }
        qDebug()<<"clear canvas.";
        this->show();
        this->repaint();
        QCoreApplication::processEvents();
}

/*
 * function: checking-in counts along time of POIs
 * constrain: location id, latitude, longitude,
 */
void FileVisualizer::on_checkinButton_clicked()
{

    int location_id_lower = this->ui->location_id_lower->toPlainText().toInt();
    int location_id_upper = this->ui->location_id_upper->toPlainText().toInt();

    float latitude_lower = this->ui->latitude_lower->toPlainText().toFloat();
    float latitude_upper = this->ui->latitude_upper->toPlainText().toFloat();

    float longitude_lower = this->ui->longitude_lower->toPlainText().toFloat();
    float longitude_upper = this->ui->longitude_upper->toPlainText().toFloat();

    location_id_lower = qMax(location_id_lower, LocationLower);
    location_id_lower = qMin(location_id_lower, LocationUpper);

    location_id_upper = qMax(location_id_upper, LocationLower);
    location_id_upper = qMin(location_id_upper, LocationUpper);

    latitude_lower = qMax(latitude_lower, LatitudeLower);
    latitude_lower = qMin(latitude_lower, LatitudeUpper);

    latitude_upper = qMax(latitude_upper, LatitudeLower);
    latitude_upper = qMin(latitude_upper, LatitudeUpper);

    longitude_lower = qMax(longitude_lower, LongitudeLower);
    longitude_lower = qMin(longitude_lower, LongitudeUpper);

    longitude_upper = qMax(longitude_upper, LongitudeLower);
    longitude_upper = qMin(longitude_upper, LongitudeUpper);



   /* TODO: finsih this */


}

