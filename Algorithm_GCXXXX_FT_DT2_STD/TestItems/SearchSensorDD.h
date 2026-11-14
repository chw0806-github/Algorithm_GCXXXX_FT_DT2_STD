#ifndef SEARCHSENSORDD_H
#define SEARCHSENSORDD_H

#include "CPBaseTestItem.h"
#include "Params/algorithmitems.h"
extern AlgorithmItems Algorithm_all[Parallel_num];

class SearchSensorDD: public CPBaseTestItem
{
public:

    SearchSensorDD(int site_num,int otp_dd_total_limit_,int col_start,
                   int row_start,int crop_x_,int crop_y_):
        otp_dd_total_limit(otp_dd_total_limit_),
        col_start(col_start),row_start(row_start),
        crop_x(crop_x_),crop_y(crop_y_)
    {
        name = "SearchSensorDD";
        site_=site_num;
        Algorithm_all[site_].set_site(site_);
    }

    ~SearchSensorDD(){
        Algorithm_all[site_].Free_All_Buf();
    }

private:
    int otp_dd_total_limit;
    int col_start;
    int row_start;
    int crop_x;
    int crop_y;


public:

};

#endif // SEARCHSENSORDD_H
