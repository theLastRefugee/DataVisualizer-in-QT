#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H

#include <QVector>

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

extern bool USE_DEBUG;
extern bool use_location_id;
extern bool use_time;
extern bool use_latitude;
extern bool use_longitude ;

class SinlgeTime
{
public:
    int year;
    int month;
    int day;
    QString detail_time;

    SinlgeTime(){}

    SinlgeTime(int y, int m, int d):year(y), month(m), day(d){}

    SinlgeTime(QString t)
    {
        QStringList time_list = t.split('-');
        year = time_list[0].toInt();
        month = time_list[1].toInt();
        time_list = time_list[2].split('T');
        day = time_list[0].toInt();
        detail_time = time_list[1];

    }

};


class SingleUser
{
public:
    int user_id;
    int location_id;
    SinlgeTime time;
    float latitude;
    float longitude;

    SingleUser(){}

    SingleUser(int userid, int locationid, QString t, float la, float lo)
    {
        user_id = userid;
        location_id = locationid;
        time = SinlgeTime(t);
        latitude = la;
        longitude = lo;

    }
};


extern QVector<SingleUser> *AllUsers;

#endif // GLOBAL_VARIABLE_H
