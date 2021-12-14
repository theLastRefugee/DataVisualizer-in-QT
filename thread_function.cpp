#include "thread_function.h"
#include "QCoreApplication"

/* thread used to read file */
void ReadThread::run()
{
    QFile our_file(filename);

    if(!our_file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       qDebug()<<"Can't open the file!";

   }

    while(!our_file.atEnd() and FileLineCount < FileLineTotal)
    {
        QByteArray line = our_file.readLine();
        QString str_line(line);

        FileLineCount ++;// count the line which has been read

        if(str_line.contains("\n"))
        {
            str_line = str_line.replace("\n" , "");
        }
        QStringList str_list = str_line.split(',');
        int user_id = str_list[0].toInt();



        int location_id = str_list[1].toInt();
        QString time = str_list[2];
        float latitude = str_list[3].toFloat();
        float longitude = str_list[4].toFloat();

        user_upper = qMax(user_id, user_upper);

        user_lower = qMin(user_id, user_lower);

        LocationUpper = qMax(LocationUpper, location_id);
        LocationLower = qMin(LocationLower, location_id);

        LatitudeUpper = qMax(LatitudeUpper, latitude);
        LatitudeLower = qMin(LatitudeLower, latitude);

        LongitudeUpper = qMax(LongitudeUpper, longitude);
        LongitudeLower = qMin(LongitudeLower, longitude);

         TimeLower = qMin(TimeLower, SingleTime(time) );
         TimeUpper = qMax(TimeUpper, SingleTime(time) );

        /* TODO: add time limit */



        AllUsers->append(SingleUser(user_id, location_id, time, latitude, longitude));


      }
}


void ProgressBarThread::run()
{

    float i=1;

    while(FileLineCount < FileLineTotal)
    {

            float progress_ratio = float(FileLineCount) / float(FileLineTotal) * 100;

        if (progress_ratio >= 20 * i)
 {
            qDebug()<<"progress:"<<progress_ratio;

        progress_bar->setValue(progress_ratio);
        progress_bar->update();

        QCoreApplication::processEvents();
        i++;
    }

        if(USE_DEBUG)
        {
            if (FileLineCount%10000==0)
            {
                qDebug()<<"progress bar output:"<<FileLineCount;
            }
        }

    }

}

void Get2UserThread::run()
{
    FileLineCount = 0;
    for(int i=0; i<AllUsers->length(); ++i)
    {
        SingleUser tmp_usr = AllUsers->at(i);
        if(tmp_usr.user_id == user_id1 )
        {
            QPair<SingleTime, int> one_pair;
            one_pair.first = tmp_usr.time;
            one_pair.second = tmp_usr.location_id;
            user_id1_vector.append(one_pair);
        }
        else if(tmp_usr.user_id == user_id2)
        {
            QPair<SingleTime, int> one_pair;
            one_pair.first = tmp_usr.time;
            one_pair.second = tmp_usr.location_id;
            user_id2_vector.append(one_pair);
        }

        FileLineCount += 1;
    }

}

void Get2LocationThread::run()
{
    FileLineCount = 0;

    for(int i=0; i<AllUsers->length(); ++i)
    {
        SingleUser tmp_usr = AllUsers->at(i);
        SingleTime tmp_time = tmp_usr.time;
        if(tmp_usr.location_id == location_id1 )
        {
           if (location_id1_map.contains(tmp_time) )
           {
               location_id1_map.insert(tmp_time,location_id1_map.value(tmp_time)+1 );
           }
           else
           {
                location_id1_map.insert(tmp_time, 1 );
           }

        }
        else if(tmp_usr.location_id == location_id2)
        {
            if (location_id2_map.contains(tmp_time) )
            {
                location_id2_map.insert(tmp_time,location_id2_map.value(tmp_time)+1 );
            }
            else
            {
                 location_id2_map.insert(tmp_time, 1 );
            }
        }

        FileLineCount += 1;
    }
}
