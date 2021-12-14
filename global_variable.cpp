#include "global_variable.h"

int FileLineCount = 0; // count the line read

int user_upper=0;
int user_lower=0;
int LocationUpper=0;
int LocationLower=0;
float LatitudeUpper=0;
float LatitudeLower=0;
float LongitudeUpper=0;
float LongitudeLower=0;
SingleTime TimeLower(2021, 11, 13); // use my birthday to init
SingleTime TimeUpper(1949, 10, 1); // use the natiaonal holiday to init


bool USE_DEBUG = false;

int FileLineTotal = 1502536; // this is hard coded for gowalla.csv

// user's interset
bool use_location_id = 0;
bool use_time = 0;
bool use_latitude = 0;
bool use_longitude = 0 ;



QVector<SingleUser> *AllUsers=new QVector<SingleUser>();
