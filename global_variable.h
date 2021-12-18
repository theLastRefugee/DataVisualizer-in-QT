#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H

#include <QVector>

class SingleTime;

extern int FileLineCount;
extern int FileLineTotal;

extern int user_upper;// the maximum id of the user in the entire file
extern int user_lower;// the minimum id of the user in the entire file
extern int LocationUpper;
extern int LocationLower;
extern float LatitudeUpper;
extern float LatitudeLower;
extern float LongitudeUpper;
extern float LongitudeLower;
extern SingleTime TimeLower;
extern SingleTime TimeUpper;


extern bool USE_DEBUG;
extern bool use_location_id;
extern bool use_time;
extern bool use_latitude;
extern bool use_longitude ;

class SingleTime
{
public:
    int year;
    int month;
    int day;
    QString detail_time;

    SingleTime(){}

    SingleTime(int y, int m, int d):year(y), month(m), day(d){}

    SingleTime(QString t)
    {
        QStringList time_list = t.split('-');
        year = time_list[0].toInt();
        month = time_list[1].toInt();
        QString tmp_day = time_list[2];
        if(tmp_day.length()<=2) // just the day, no detail time
        {
            day = tmp_day.toInt();
        }
        else //with detail time
        {
            time_list = time_list[2].split('T');
            day = time_list[0].toInt();
            detail_time = time_list[1];
        }

    }

    QString string()
    {
        QString tmp=QString::number(year)+'-'+QString::number(month)+'-'+QString::number(day);
        return tmp;
    }
    /* 为了map能用，需要重载小于号 */
    inline bool operator < (SingleTime const &data) const
    {
        if(year != data.year)
        {
            return year < data.year;
        }
        else if(month != data.month)
        {
            return month < data.month;
        }
        else
        {
            return day < data.day;
        }
    }

    inline bool operator > (SingleTime const &data) const
    {
        if(year != data.year)
        {
            return year > data.year;
        }
        else if(month != data.month)
        {
            return month > data.month;
        }
        else
        {
            return day > data.day;
        }
    }

    inline bool operator <= (SingleTime const &data) const
    {
        if(year != data.year)
        {
            return year < data.year;
        }
        else if(month != data.month)
        {
            return month < data.month;
        }
        else
        {
            return day <= data.day;
        }
    }

    inline bool operator >= (SingleTime const &data) const
    {
        if(year != data.year)
        {
            return year > data.year;
        }
        else if(month != data.month)
        {
            return month > data.month;
        }
        else
        {
            return day >= data.day;
        }
    }


};


class SingleUser
{
public:
    int user_id;
    int location_id;
    SingleTime time;
    float latitude;
    float longitude;

    SingleUser(){}

    SingleUser(int userid, int locationid, QString t, float la, float lo)
    {
        user_id = userid;
        location_id = locationid;
        time = SingleTime(t);
        latitude = la;
        longitude = lo;
    }


};


class TimeLocation{
public:
    SingleTime time;
    float latitude;
    float longitude;

    TimeLocation(SingleTime t, float la, float lo): time(t), latitude(la), longitude(lo){  }

    TimeLocation(){}

    /* 按照时间比大小 */
    inline bool operator < (TimeLocation const &data) const
    {
        if(time.year != data.time.year)
        {
            return time.year < data.time.year;
        }
        else if(time.month != data.time.month)
        {
            return time.month < data.time.month;
        }
        else if(time.day != data.time.day)
        {
            return time.day < data.time.day;
        }
        else
        {
            QStringList my_time= time.detail_time.split(':');
            QStringList your_time= data.time.detail_time.split(':');
            if(my_time[0].toInt()!=your_time[0].toInt() )
            {
                return my_time[0].toInt() < your_time[0].toInt();
            }
            else if(my_time[1].toInt()!=your_time[1].toInt() )
            {
                return my_time[1].toInt() < your_time[1].toInt();
            }
            else
            {
                return my_time[2].toInt() < your_time[2].toInt();
            }
        }
    }

    inline bool operator <= (TimeLocation const &data) const
    {
        if(time.year != data.time.year)
        {
            return time.year < data.time.year;
        }
        else if(time.month != data.time.month)
        {
            return time.month < data.time.month;
        }
        else
        {
            return time.day <= data.time.day;
        }
    }

    inline bool operator > (TimeLocation const &data) const
    {
        if(time.year != data.time.year)
        {
            return time.year > data.time.year;
        }
        else if(time.month != data.time.month)
        {
            return time.month > data.time.month;
        }
        else
        {
            return time.day > data.time.day;
        }
    }

    inline bool operator >= (TimeLocation const &data) const
    {
        if(time.year != data.time.year)
        {
            return time.year > data.time.year;
        }
        else if(time.month != data.time.month)
        {
            return time.month > data.time.month;
        }
        else
        {
            return time.day >= data.time.day;
        }
    }
};

class LocationCount{
public:
    float latitude;
    float longitude;
    int count;
    LocationCount(float la, float lo):latitude(la), longitude(lo)
    {
        count = 0;
    }
    LocationCount()
    {
        count = 0;
    }
};

extern QVector<SingleUser> *AllUsers;

#endif // GLOBAL_VARIABLE_H
