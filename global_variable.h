#ifndef GLOBAL_VARIABLE_H
#define GLOBAL_VARIABLE_H

#include <QVector>

extern int FileLineCount;
extern int FileLineTotal;
extern bool USE_DEBUG;
extern bool use_location_id;
extern bool use_time;
extern bool use_latitude;
extern bool use_longitude ;

class SingleUser
{
public:
    int user_id;
    int location_id;
    QString time;
    float latitude;
    float longitude;

    SingleUser(){}

    SingleUser(int userid, int locationid, QString t, float la, float lo)
    {
        user_id = userid;
        location_id = locationid;
        time = t;
        latitude = la;
        longitude = lo;

    }
};

extern QVector<SingleUser> *AllUsers;

#endif // GLOBAL_VARIABLE_H
