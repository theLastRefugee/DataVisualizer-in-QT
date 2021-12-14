#include "filevisualizer.h"
#include "ui_filevisualizer.h"
#include "QCoreApplication"
#include "QMap"
#include "QtAlgorithms"
#include <algorithm>

#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <global_variable.h>
#include <QSplineSeries>


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


    while(FileLineCount <= FileLineTotal)
    {

            float progress_ratio = float(FileLineCount) / float(FileLineTotal) * 100;

//            qDebug()<<"progress:"<<progress_ratio;

        this->ui->progressBar->setValue(progress_ratio);
        this->ui->progressBar->update();

        QCoreApplication::processEvents();


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


    set_default_value();
}

void FileVisualizer::set_default_value()
{
    // set default value
    this->ui->user_id_lower->setText( QString::number(user_lower) );
    this->ui->user_id_upper->setText( QString::number(user_upper) );

    this->ui->latitude_lower->setText( QString::number(LatitudeLower) );
    this->ui->latitude_upper->setText( QString::number(LatitudeUpper) );

    this->ui->longitude_lower->setText( QString::number(LongitudeLower) );
    this->ui->longitude_upper->setText( QString::number(LongitudeUpper) );

    this->ui->time_lower->setText( TimeLower.string());
    this->ui->time_upper->setText( TimeUpper.string());

    this->ui->location_id_lower->setText( QString::number(LocationLower) );
    this->ui->location_id_upper->setText( QString::number(LocationUpper) );

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

    SingleTime time_lower =  SingleTime(this->ui->time_lower->toPlainText() );
    SingleTime time_upper =  SingleTime(this->ui->time_upper->toPlainText() );

    time_lower = qMax(time_lower, TimeLower);
    time_lower = qMin(time_lower, TimeUpper);

    time_upper = qMax(time_upper, TimeLower);
    time_upper = qMin(time_upper, TimeUpper);

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

    qDebug()<<"availabel range:";
    qDebug()<<"location id: "<<LocationLower<<" to "<<LocationUpper;
    qDebug()<<"latitude: "<<LatitudeLower<<" to "<<LatitudeUpper;
    qDebug()<<"longitude: "<<LongitudeLower<<" to "<<LongitudeUpper;

    qDebug()<<"draw range:";
    qDebug()<<"location id: "<<location_id_lower<<" to "<<location_id_upper;
    qDebug()<<"latitude: "<<latitude_lower<<" to "<<latitude_upper;
    qDebug()<<"longitude: "<<longitude_lower<<" to "<<longitude_upper;

    QMap<int, QMap<SingleTime, int> > *poi_data_counter = new QMap<int, QMap<SingleTime, int> >;

    // step 1, count each poi with time
    for(int i=0; i< AllUsers->length() ; ++i )
    {
        SingleUser tmp_usr = AllUsers->at(i);

        if(tmp_usr.location_id>=location_id_lower && tmp_usr.location_id<=location_id_upper &&
           tmp_usr.latitude>=latitude_lower && tmp_usr.latitude<=latitude_upper &&
            tmp_usr.longitude>= longitude_lower && tmp_usr.longitude<= longitude_upper &&
                tmp_usr.time >= time_lower && tmp_usr.time<=time_upper)
          // in the range
        {
            if(poi_data_counter->contains(tmp_usr.location_id))
            // this location has be recorded, update the count by date
            {
                QMap<SingleTime, int> tmpMap = poi_data_counter->value(tmp_usr.location_id);
                if(tmpMap.contains(tmp_usr.time))// date included
                {
                    tmpMap.insert(tmp_usr.time, tmpMap.value(tmp_usr.time)+1 ); // update count
                    poi_data_counter->insert(tmp_usr.location_id, tmpMap); // update map
                }
                else// date not inclueded
                {
                    tmpMap.insert(tmp_usr.time, 1 ); // init count
                    poi_data_counter->insert(tmp_usr.location_id, tmpMap); // update map
                }
            }
            else // this location not recorded
            {
                QMap<SingleTime, int> tmpMap;
                tmpMap.insert(tmp_usr.time, 1);
                poi_data_counter->insert(tmp_usr.location_id , tmpMap);
            }
        }
     }


    /* step 2, drawing */
    /* reference: https://doc.qt.io/qt-5/qtcharts-datetimeaxis-example.html */


    QChart *chart = new QChart();

    //set x axis
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);

    // set y axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Visit counts");
    chart->addAxis(axisY, Qt::AlignLeft);

    // set title
    QString title ="Checking-ins of Locations over Time!  Location id:"+QString::number(location_id_lower)+" to "+QString::number(location_id_upper)+"<br>";
    title += "latitude:"+QString::number(latitude_lower)+" to "+QString::number(latitude_upper)+" ";
    title += "longitude:"+QString::number(longitude_lower)+" to "+QString::number(longitude_upper)+"<br>";
    chart->setTitle(title);

    // draw each line
    QList<int> out_keys = poi_data_counter->keys();

    int max_count = 0;
    QDateTime min_date;
    QDateTime max_date;
    min_date.setDate(QDate(2021, 11, 13));
    max_date.setDate(QDate(1949, 10, 1));

    for(int i=0; i<out_keys.length() ; i++)
    {
//        QLineSeries *series = new QLineSeries();
        QSplineSeries *series = new QSplineSeries();
        int location_id = out_keys.at(i);
        series->setName(QString::number(location_id));

        QMap<SingleTime, int> tmpMap = poi_data_counter->value(location_id); // get map
        QList<SingleTime> in_keys = tmpMap.keys();
        for(int j=0; j<in_keys.length(); ++j)
        {
            SingleTime tmp_key = in_keys.at(j);
            int tmp_value = tmpMap.value(tmp_key);
            QDateTime momentInTime;
            momentInTime.setDate(QDate(tmp_key.year, tmp_key.month , tmp_key.day));
            series->append(momentInTime.toMSecsSinceEpoch(),  tmp_value);
//            qDebug()<<"tmp value:"<<tmp_value;
            max_count = qMax(max_count, tmp_value);
            min_date = qMin(momentInTime, min_date);
            max_date = qMax(momentInTime,max_date);
        }
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignBottom);
    axisY->setRange(0, max_count);
    axisX->setRange(min_date, max_date);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setObjectName("drawing_chart");

    // put on the canvas
    this->ui->drawingLayout->addWidget(chartView);

    this->ui->state_textEdit->setText("Checking-ins over time are shown successfully.");

    QCoreApplication::processEvents();

}


/*
 * function: compare 2 users' visit
*/
void FileVisualizer::on_compare2userButton_clicked()
{
    int user_id1 = this->ui->user_id1->toPlainText().toInt();
    int user_id2 = this->ui->user_id2->toPlainText().toInt();

    FileLineCount = 0;
    // find the user id
    Get2UserThread *new_thread = new Get2UserThread;
    new_thread->user_id1 = user_id1;
    new_thread->user_id2 = user_id2;

    new_thread->run();

      this->ui->progressBar->setValue(0);
    QCoreApplication::processEvents();
    while(FileLineCount <= FileLineTotal)
    {

            float progress_ratio = float(FileLineCount) / float(FileLineTotal) * 100;


        this->ui->progressBar->setValue(progress_ratio);
        this->ui->progressBar->update();

        QCoreApplication::processEvents();


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
    this->ui->state_textEdit->setText("Finding user id is finished.");

    new_thread->wait();

    /* begin to draw */

    QChart *chart = new QChart();

    //set x axis
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);

    // set y axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Location id");
    chart->addAxis(axisY, Qt::AlignLeft);

    // set title
    QString title ="Difference of two Users' Visit!";

    chart->setTitle(title);


        QLineSeries *series1 = new QLineSeries();
        QLineSeries *series2 = new QLineSeries();
//        QSplineSeries *series = new QSplineSeries();

        series1->setName(QString::number(user_id1));
        series2->setName(QString::number(user_id2));

         QVector<QPair<SingleTime, int> > user_vector1 = new_thread->user_id1_vector;
         QVector<QPair<SingleTime, int> > user_vector2 = new_thread->user_id2_vector;

         int max_count = 0;
         QDateTime min_date;
         QDateTime max_date;
         min_date.setDate(QDate(2021, 11, 13));
         max_date.setDate(QDate(1949, 10, 1));

        for(int j=0; j<user_vector1.length(); ++j)
        {
            SingleTime tmp_key = user_vector1.at(j).first;
            int tmp_value = user_vector1.at(j).second;
            QDateTime momentInTime;
            momentInTime.setDate(QDate(tmp_key.year, tmp_key.month , tmp_key.day));
            series1->append(momentInTime.toMSecsSinceEpoch(),  tmp_value);

            max_count = qMax(max_count, tmp_value);
            min_date = qMin(momentInTime, min_date);
            max_date = qMax(momentInTime,max_date);
        }
        for(int j=0; j<user_vector2.length(); ++j)
        {
            SingleTime tmp_key = user_vector2.at(j).first;
            int tmp_value = user_vector2.at(j).second;
            QDateTime momentInTime;
            momentInTime.setDate(QDate(tmp_key.year, tmp_key.month , tmp_key.day));
            series2->append(momentInTime.toMSecsSinceEpoch(),  tmp_value);

            max_count = qMax(max_count, tmp_value);
            min_date = qMin(momentInTime, min_date);
            max_date = qMax(momentInTime,max_date);
        }

        chart->addSeries(series1);
        series1->attachAxis(axisX);
        series1->attachAxis(axisY);
        chart->addSeries(series2);
        series2->attachAxis(axisX);
        series2->attachAxis(axisY);


    chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignBottom);
    axisY->setRange(0, max_count);
    axisX->setRange(min_date, max_date);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setObjectName("drawing_chart");

    // put on the canvas
    this->ui->drawingLayout->addWidget(chartView);

    this->ui->state_textEdit->setText("Difference of two users' visits are shown successfully.");

    QCoreApplication::processEvents();

}

void FileVisualizer::on_compare2locationButton_clicked()
{
    int location_id1 = this->ui->location_id1->toPlainText().toInt();
    int location_id2 = this->ui->location_id2->toPlainText().toInt();

    FileLineCount = 0;
    // first, find the location
    Get2LocationThread *new_thread = new Get2LocationThread;
    new_thread->location_id1 = location_id1;
    new_thread->location_id2 = location_id2;

    new_thread->run();

    this->ui->progressBar->setValue(0);
  QCoreApplication::processEvents();

    while(FileLineCount <= FileLineTotal)
    {

            float progress_ratio = float(FileLineCount) / float(FileLineTotal) * 100;


        this->ui->progressBar->setValue(progress_ratio);
        this->ui->progressBar->update();

        QCoreApplication::processEvents();


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
    this->ui->state_textEdit->setText("Finding location id is finished.");
    new_thread->wait();




    /* begin to draw */

    QChart *chart = new QChart();

    //set x axis
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);

    // set y axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Visit counts");
    chart->addAxis(axisY, Qt::AlignLeft);

    // set title
    QString title = "Comparison of two POIs' Visit Counts ";
    chart->setTitle(title);



    int max_count = 0;
    QDateTime min_date;
    QDateTime max_date;
    min_date.setDate(QDate(2021, 11, 13));
    max_date.setDate(QDate(1949, 10, 1));

    QMap<SingleTime, int> location_id1_map = new_thread->location_id1_map;
    QMap<SingleTime, int> location_id2_map = new_thread->location_id2_map;
    QList<SingleTime> location_id1_key = location_id1_map.keys();
    QList<SingleTime> location_id2_key = location_id2_map.keys();

    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();

   for(int j=0; j<location_id1_key.length(); ++j)
   {
       SingleTime tmp_key = location_id1_key.at(j);
       int tmp_value = location_id1_map.value(tmp_key);
       QDateTime momentInTime;
       momentInTime.setDate(QDate(tmp_key.year, tmp_key.month , tmp_key.day));
       series1->append(momentInTime.toMSecsSinceEpoch(),  tmp_value);

       max_count = qMax(max_count, tmp_value);
       min_date = qMin(momentInTime, min_date);
       max_date = qMax(momentInTime,max_date);
   }

   for(int j=0; j<location_id2_key.length(); ++j)
   {
       SingleTime tmp_key = location_id2_key.at(j);
       int tmp_value = location_id2_map.value(tmp_key);
       QDateTime momentInTime;
       momentInTime.setDate(QDate(tmp_key.year, tmp_key.month , tmp_key.day));
       series2->append(momentInTime.toMSecsSinceEpoch(),  tmp_value);

       max_count = qMax(max_count, tmp_value);
       min_date = qMin(momentInTime, min_date);
       max_date = qMax(momentInTime,max_date);
   }

   chart->addSeries(series1);
   series1->attachAxis(axisX);
   series1->attachAxis(axisY);
   chart->addSeries(series2);
   series2->attachAxis(axisX);
   series2->attachAxis(axisY);


chart->legend()->setVisible(true);
chart->legend()->setAlignment(Qt::AlignBottom);
axisY->setRange(0, max_count);
axisX->setRange(min_date, max_date);

QChartView *chartView = new QChartView(chart);
chartView->setRenderHint(QPainter::Antialiasing);
chartView->setObjectName("drawing_chart");

// put on the canvas
this->ui->drawingLayout->addWidget(chartView);

this->ui->state_textEdit->setText("Difference of two users' visits are shown successfully.");

QCoreApplication::processEvents();

}
