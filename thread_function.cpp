#include "thread_function.h"
#include "QCoreApplication"

/* thread used to read file */
void ReadThread::run()
{
    QFile our_file(filename);

    if(!our_file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       qDebug()<<"Can't open the file!"<<Qt::endl;

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
