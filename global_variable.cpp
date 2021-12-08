#include "global_variable.h"

int FileLineCount = 0; // count the line read


bool USE_DEBUG = false;

int FileLineTotal = 1502536; // this is hard coded for gowalla.csv

// user's interset
bool use_location_id = 0;
bool use_time = 0;
bool use_latitude = 0;
bool use_longitude = 0 ;



QVector<SingleUser> *AllUsers=new QVector<SingleUser>();
