#include "utils.h"


void print_vector_with_int_pair(QVector<QPair<int,int> > *vec)
{
    for(int i=0; i<vec->size(); ++i)
    {
        qDebug()<<"at"<<i<<" key:"<<vec->at(i).first<<" count:"<<vec->at(i).second;
    }


}
